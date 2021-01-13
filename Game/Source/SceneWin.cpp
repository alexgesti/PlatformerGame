#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneWin.h"
#include "Scene.h"
#include "ModuleController.h"
#include "FadeController.h"

#include "Defs.h"
#include "Log.h"

SceneWin::SceneWin() : Module()
{
	name.Create("sceneWin");
}

// Destructor
SceneWin::~SceneWin()
{}

// Called before render is available
bool SceneWin::Awake()
{
	LOG("Loading Scene Win");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneWin::Start()
{
	spritewin = app->tex->Load("Assets/Screens/Title/Win.png");

	return true;
}

// Called each loop iteration
bool SceneWin::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneWin::Update(float dt)
{
	if (app->scene->notsceneactived == false)
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	return true;
}

// Called each loop iteration
bool SceneWin::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(spritewin, 0, 0, &rect);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->canfade = true;
		app->fade->startinblack = false;
		app->fade->winlosecontinue = true;
	}
	return ret;
}

// Called before quitting
bool SceneWin::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
