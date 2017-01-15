#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "DrawDebug.h"
#include "ModuleGOManager.h"
#include "FileParser.h"

#include "GameObject.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceShader.h"

#include "Primitive.h"

//TMP
#include "ModuleResourceManager.h"

#include "OpenGL.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */


ModuleRenderer3D::ModuleRenderer3D(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Renderer3D: Creation.");

	name.assign("module_renderer3d");
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	_LOG(LOG_STD, "Renderer3D: Destroying.");
}

// Called before render is available
bool ModuleRenderer3D::init(FileParser* conf)
{
	_LOG(LOG_STD, "Renderer3D: Init.");
	_LOG(LOG_INFO, "Creating 3D Renderer context");
	bool ret = true;

	vsync = conf->getBool("vsync", true);

	//Create context
	context = SDL_GL_CreateContext(app->window->getWindow());
	if (context == nullptr)
	{
		_LOG(LOG_ERROR, "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		GLenum gl = glewInit();

		if (gl != GLEW_OK)
		{
			_LOG(LOG_ERROR, "Glew library could not init %s\n", glewGetErrorString(gl));
			ret = false;
		}
		else
			_LOG(LOG_INFO, "Using Glew %s", glewGetString(GLEW_VERSION));
	}

	if (ret == true)
	{
		// get version info
		_LOG(LOG_INFO,"Vendor: %s", glGetString(GL_VENDOR));
		_LOG(LOG_INFO,"Renderer: %s", glGetString(GL_RENDERER));
		_LOG(LOG_INFO,"OpenGL version supported %s", glGetString(GL_VERSION));
		_LOG(LOG_INFO, "GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

		//Use Vsync
		setVSync(vsync);

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			_LOG(LOG_ERROR, "Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		// Blend for transparency
		//glBlendEquation(GL_FUNC_ADD);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		//lights[0].position = float3::zero;
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		//glEnable(GL_TEXTURE_2D);

		//glShadeModel(GL_SMOOTH);		 // Enables Smooth Shading
	}

	//TODO: Check this
	if (app->isPlaySate())
		;//TODO: get active camera from config

	Event e(Event::eventType::WIN_RESIZE);
	e.point2d.x = app->window->getWidth();
	e.point2d.y = app->window->getHeight();

	app->sendGlobalEvent(e);

	return ret;
}

bool ModuleRenderer3D::start()
{
	_LOG(LOG_STD, "Render3D: Start.");
	bool ret = true;

	glEnable(GL_LIGHTING);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::preUpdate(float dt)
{
	Camera* cam = (app->getGameState() != gameState::EDITOR) ? (activeCamera) : (app->camera->getCamera());  //TODO: When active camera is NULL and play state, strange things happen

	if (cam && cam->projectMatrixChanged)
	{
		updateProjectionMat(cam);
		cam->projectMatrixChanged = false;
	}

	if (cam)
	{
		Color col = cam->background;
		glClearColor(col.r, col.g, col.b, col.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(cam->getGLViewMatrix());


		// light 0 on cam pos
		// TODO: if game is on play should be a lighton the camera or only the lights in the game
		lights[0].SetPos(cam->frustum.pos.x, cam->frustum.pos.y, cam->frustum.pos.z);
	}
	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::postUpdate(float dt)
{
	//TODO: draw all geometry

	if (showGrid)
	{
		//Draw floor grid and world axis
		P_Plane floor(0, 1, 0, 0);
		floor.axis = true;
		floor.color.Set(255, 255, 255);
		floor.Render();
	}

	app->goManager->draw();

	if (/*app->debug*/true) //TODO: change debug system
	{
		beginDebugDraw();
		app->drawDebug();
		endDebugDraw();
	}

	app->editor->drawEditor();

	SDL_GL_SwapWindow(app->window->getWindow());
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::cleanUp()
{
	_LOG(LOG_STD, "Renderer3D: CleanUp.");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::onResize(int width, int height)
{
	Camera* cam = (app->isPlaySate()) ? (activeCamera) : (app->camera->getCamera());

	cam->setAspectRatio((float)width / (float)height);
	glViewport(0, 0, width, height);

	updateProjectionMat(cam);
}

bool ModuleRenderer3D::getVSync() const
{
	return vsync;
}

void ModuleRenderer3D::setVSync(bool vsync)
{
	if (this->vsync != vsync)
	{
		this->vsync = vsync;
		if (SDL_GL_SetSwapInterval(vsync ? 1 : 0) < 0)
			_LOG(LOG_WARN, "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
	}
}

void ModuleRenderer3D::setActiveCamera(Camera* activeCamera)
{
	this->activeCamera = activeCamera;
}

Camera* ModuleRenderer3D::getActiveCamera()const
{
	return activeCamera;
}

void ModuleRenderer3D::updateProjectionMat(Camera* cam)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)cam->getGLProjectMatrix());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::onGlobalEvent(const Event& e)
{
	switch (e.type)
	{
	case Event::eventType::WIN_RESIZE:
		onResize(e.point2d.x, e.point2d.y);
		break;

	case Event::eventType::PLAY:
		editorShowGrid = showGrid;
		showGrid = false;
		break;

	case Event::eventType::PAUSE:
		showGrid = editorShowGrid;
		break;

	case Event::eventType::STOP:
		showGrid = editorShowGrid;
		break;

	default:
		break;
	}
}

//---------------------------------

void ModuleRenderer3D::drawGameObject(GameObject* obj)
{
	if (!obj)
		return;

	Transform* trans = obj->transform;
	std::vector<Component*> meshes = obj->findComponent(MESH);
	std::vector<Component*> mats = obj->findComponent(MATERIAL);

	if (!meshes[0] || !trans)
		return;

	bool selected = (app->goManager->getSelected() == obj);

	//First push transform matrix, remember to pop it at end of draw
	//----------------------
	//SH
	/*glPushMatrix();
	glMultMatrixf(trans->getGlobalTransformGL());*/

	//----------------------

	//Iterate all meshes
	for (uint i = 0; i < meshes.size(); ++i)
	{
		Mesh* mesh = (Mesh*)meshes[i];
		Material* mat = (Material*)mats[i];

		if (mesh)
		{
			ResourceMesh* resMesh = mesh->meshResource;

			if (resMesh)
			{		
				if (mesh->renderWireframe || selected)
					;// drawWireframe(resMesh, selected);

				if (!mesh->renderWireframe)
				{
					Color col = (mat) ? mat->color : Color(0.5f, 0.5f, 0.5f, 1.f);
					glColor4f(col.r, col.g, col.b, col.a);

					Camera* currentCam = (app->getGameState() == gameState::EDITOR) ? app->camera->getCamera() : getActiveCamera();

					uint shID = 0;
					if (mat && mat->shaderResource && mat->shaderResource->shaderID > 0)
						shID = mat->shaderResource->shaderID;
					else
						shID = app->resourceManager->getDefaultShader()->shaderID;

					glUseProgram(shID);

					//----------------------------------
					//Matrices.
					GLuint model = glGetUniformLocation(shID, "model");
					glUniformMatrix4fv(model, 1, GL_FALSE, trans->getGlobalTransformGL()); //NOTE: Might not be the transposed one

					GLuint view = glGetUniformLocation(shID, "view");
					glUniformMatrix4fv(view, 1, GL_FALSE, currentCam->getGLViewMatrix());

					GLuint projection = glGetUniformLocation(shID, "projection");
					glUniformMatrix4fv(projection, 1, GL_FALSE, currentCam->getGLProjectMatrix());

					//Color
					GLint color = glGetUniformLocation(shID, "color");
					if (color != -1)
						glUniform3f(color, col.r, col.g, col.b);

					//Time
					GLuint t = glGetUniformLocation(shID, "time");
					glUniform1f(t, time->elapsedTime());

					//DT
					GLuint deltaTime = glGetUniformLocation(shID, "dt");
					glUniform1f(deltaTime, time->editorDT());

					//----------------------------------
					//Texture.
					ResourceTexture* resTex = (mat) ? mat->textureResource : NULL;
					if (resTex)
					{
						GLint texture = glGetUniformLocation(shID, "ourTexture");
						if (texture != -1)
						{
							glUniform1i(texture, 0);
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, resTex->textureGlID);
						}
					}

					//----------------------------------
					//Buffers
					glEnableClientState(GL_VERTEX_ARRAY);

						//Vertices.
					glEnableVertexAttribArray(0);
					glBindBuffer(GL_ARRAY_BUFFER, resMesh->idVertices);
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

						//UV's
					if (resMesh->numTexCoords > 0)
					{
						glEnableVertexAttribArray(1);
						glBindBuffer(GL_ARRAY_BUFFER, resMesh->idTexCoords);
						glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
					}

						//Normals
					if (resMesh->numNormals > 0)
					{
						glEnableVertexAttribArray(2);
						glBindBuffer(GL_ARRAY_BUFFER, resMesh->idNormals);
						glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
					}

						//Indices
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resMesh->idIndices);

					//----------------------------------

					if (mesh->renderNormals)
						drawNormals(resMesh);

					//----------------------------------

					glDrawElements(GL_TRIANGLES, resMesh->numIndices, GL_UNSIGNED_INT, NULL);

					//----------------------------------

					glDisableVertexAttribArray(0);
					glDisableVertexAttribArray(1);
					glDisableVertexAttribArray(2);
				}

				//Cleaning
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindTexture(GL_TEXTURE_2D, 0);
				//------
				glDisableClientState(GL_NORMAL_ARRAY);
				glDisableClientState(GL_COLOR_ARRAY);
				glDisableClientState(GL_VERTEX_ARRAY);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);

				//==============
				glColor4f(1.f, 1.f, 1.f, 1.f);

				glUseProgram(0);
			}
		}
	}

	//----------------------

	glPopMatrix();

	//----------------------
}

void ModuleRenderer3D::drawWireframe(ResourceMesh* resMesh, bool selected)
{
	if (!resMesh)
		return;

	//Wireframe
	//----------------
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, resMesh->idVertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resMesh->idIndices);

	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (selected)
	{
		glLineWidth(1.5f);
		glColor4f(0.2f, 1.f, 0.2f, 1.f);
	}
	else
	{
		glLineWidth(1.f);
		glColor4f(0.f, 0.8f, 0.8f, 1.f);
		glDisable(GL_CULL_FACE);
	}

	glDrawElements(GL_TRIANGLES, resMesh->numIndices, GL_UNSIGNED_INT, NULL);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_LIGHTING);

	if (selected)
		glLineWidth(1.f);
	else
		glEnable(GL_CULL_FACE);

	//----------------
}

void ModuleRenderer3D::drawNormals(ResourceMesh* resMesh)
{
	if (!resMesh)
		return;

	glDisable(GL_LIGHTING);
	glLineWidth(0.7f);
	glBegin(GL_LINES);
	glColor4f(0.4f, 0.1f, 0.f, 1.f);

	for (uint i = 0; i < resMesh->numVertices; ++i)
	{
		glVertex3f(resMesh->vertices[i * 3], resMesh->vertices[i * 3 + 1], resMesh->vertices[i * 3 + 2]);
		glVertex3f(resMesh->vertices[i * 3] + resMesh->normals[i * 3], resMesh->vertices[i * 3 + 1] + resMesh->normals[i * 3 + 1], resMesh->vertices[i * 3 + 2] + resMesh->normals[i * 3 + 2]);
	}

	glEnd();
	glLineWidth(1.f);
	glEnable(GL_LIGHTING);
}