#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"
#include "ModuleEditor.h"
#include "DrawDebug.h"

#include "Primitive.h"

#include "OpenGL.h"

#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */


ModuleRenderer3D::ModuleRenderer3D(bool startEnabled) : Module(startEnabled)
{
	_LOG(LOG_STD, "Renderer3D: Creation.");

	name.assign("module_renderer3d");

	vsync = VSYNC;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{
	_LOG(LOG_STD, "Renderer3D: Destroying.");
}

// Called before render is available
bool ModuleRenderer3D::init()
{
	_LOG(LOG_STD, "Renderer3D: Init.");
	_LOG(LOG_REN, "Creating 3D Renderer context");
	bool ret = true;

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

	if (app->debug)
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