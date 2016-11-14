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

#include "Primitive.h"
#include "Camera.h"

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
	_LOG(LOG_REN, "Creating 3D Renderer context");
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
			_LOG(LOG_REN, "Using Glew %s", glewGetString(GLEW_VERSION));
	}

	if (ret == true)
	{
		// get version info
		_LOG(LOG_REN,"Vendor: %s", glGetString(GL_VENDOR));
		_LOG(LOG_REN,"Renderer: %s", glGetString(GL_RENDERER));
		_LOG(LOG_REN,"OpenGL version supported %s", glGetString(GL_VERSION));
		_LOG(LOG_REN, "GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

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

	onResize(app->window->getWidth(), app->window->getHeight());

	return ret;
}

bool ModuleRenderer3D::start()
{
	_LOG(LOG_STD, "Render3D: Start.");
	bool ret = true;

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::preUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//TODO
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(app->camera->getViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(app->camera->position.x, app->camera->position.y, app->camera->position.z);

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
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	projectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&projectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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

//---------------------------------

void ModuleRenderer3D::drawGameObject(GameObject* obj)
{
	if (!obj)
		return;

	Transform* trans = obj->getTransform();
	std::vector<Component*> meshes = obj->findComponent(MESH);
	std::vector<Component*> mats = obj->findComponent(MATERIAL);

	if (!meshes[0] || !trans)
		return;

	bool selected = (app->goManager->getSelected() == obj);

	//First push transform matrix, remember to pop it at end of draw
	//----------------------

	glPushMatrix();
	glMultMatrixf(*trans->getTransformMatrix().v);

	//----------------------

	//Iterate all meshes
	for (uint i = 0; i < meshes.size(); ++i)
	{
		Mesh* mesh = (Mesh*)meshes[i];
		Material* mat = (Material*)mats[i];

		glEnable(GL_LIGHTING);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Now pass vertices
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->idVertices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (mesh->renderWireframe)
		{
			drawWireframe(selected);
		}
		else
		{
			//Render normals
			if (mesh->renderNormals && mesh->numNormals > 0)
			{
				glDisable(GL_LIGHTING);
				glLineWidth(0.7f);
				glBegin(GL_LINES);
				glColor4f(0.4f, 0.1f, 0.f, 1.f);

				for (uint i = 0; i < mesh->numVertices; ++i)
				{
					glVertex3f(mesh->vertices[i * 3], mesh->vertices[i * 3 + 1], mesh->vertices[i * 3 + 2]);
					glVertex3f(mesh->vertices[i * 3] + mesh->normals[i * 3], mesh->vertices[i * 3 + 1] + mesh->normals[i * 3 + 1], mesh->vertices[i * 3 + 2] + mesh->normals[i * 3 + 2]);
				}

				glEnd();
				glLineWidth(1.f);
				glEnable(GL_LIGHTING);
			}

			//Set normals
			if (mesh->idNormals > 0)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, mesh->idNormals);
				glNormalPointer(GL_FLOAT, 0, NULL);
			}

			//If there is a material use its texture and colour if not use a default colour
			if (mat)
			{
				glEnable(GL_TEXTURE_2D);
				glColor4f(mat->color.r, mat->color.g, mat->color.b, mat->color.a);
				if (mesh->idTexture > -1)
				{
					uint tex = mat->getTexture(mesh->idTexture);
					if (tex > 0)
					{
						glBindTexture(GL_TEXTURE_2D, tex);
					}

					//Set UV's
					if (mesh->idTexCoords > 0)
					{
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glBindBuffer(GL_ARRAY_BUFFER, mesh->idTexCoords);
						glTexCoordPointer(2, GL_FLOAT, 0, NULL);
					}
				}
			}
			else
			{
				glColor4f(0.5f, 0.5f, 0.5f, 1.f);
			}
		}

		//Finally set indices and draw elements
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->idIndices);
		glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);

		if (selected && !mesh->renderWireframe)
		{
			drawWireframe(selected);
			glDrawElements(GL_TRIANGLES, mesh->numIndices, GL_UNSIGNED_INT, NULL);
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
		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor4f(1.f, 1.f, 1.f, 1.f);
	}

	//----------------------

	glPopMatrix();

	//----------------------
}

void ModuleRenderer3D::drawWireframe(bool selected)
{
	//Wireframe
	//----------------
	glDisable(GL_LIGHTING);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor4f(0.f, 0.8f, 0.8f, 1.f);

	if (selected)
	{
		glLineWidth(1.5f);
		glColor4f(0.2f, 1.f, 0.2f, 1.f);
	}
	else
	{
		glLineWidth(1.f);
		glDisable(GL_CULL_FACE);
	}

	//----------------
}