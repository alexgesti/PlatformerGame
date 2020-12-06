#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLose.h"
#include "Scene.h"
#include "ModuleController.h"

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
	SpriteLose = app->tex->Load("Assets/Screens/Title/Lose.png");

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
	if (app->scene->NotSceneActived == false)
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
	app->render->DrawTexture(SpriteLose, 0, 0, &rect);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->scene->Reset();
		app->modcontrol->currentscene = 1;

	}

	return ret;
}

// Called before quitting
bool SceneLose::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
