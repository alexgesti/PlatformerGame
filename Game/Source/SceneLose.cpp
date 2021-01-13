#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLose.h"
#include "Scene.h"
#include "ModuleController.h"
#include "FadeController.h"

#include "Defs.h"
#include "Log.h"

SceneLose::SceneLose() : Module()
{
	name.Create("sceneLose");
}

// Destructor
SceneLose::~SceneLose()
{}

// Called before render is available
bool SceneLose::Awake()
{
	LOG("Loading Scene Lose");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLose::Start()
{
	spritelose = app->tex->Load("Assets/Screens/Title/Lose.png");

	return true;
}

// Called each loop iteration
bool SceneLose::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLose::Update(float dt)
{
	if (app->scene->notsceneactived == false)
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	return true;
}

// Called each loop iteration
bool SceneLose::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(spritelose, 0, 0, &rect);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->fade->canfade = true;
		app->fade->startinblack = false;
		app->fade->winlosecontinue = true;
	}

	return ret;
}

// Called before quitting
bool SceneLose::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
