#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "SceneLogo.h"
#include "Scene.h"
#include "ModuleController.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("sceneintro");
}

// Destructor
SceneIntro::~SceneIntro()
{}

// Called before render is available
bool SceneIntro::Awake()
{
	LOG("Loading Scene Intro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneIntro::Start()
{
	SpriteIntro = app->tex->Load("Assets/Screens/Title/intro.png");
	SpaceStart = app->tex->Load("Assets/Screens/Title/start.png");

	return true;
}

// Called each loop iteration
bool SceneIntro::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneIntro::Update(float dt)
{
	// Load music
	if (OneTimeOnly == false && app->sceneLogo->MusicOn == true)
	{
		app->audio->PlayMusic("Assets/Audio/Music/deities_get_takeout_too.ogg", 0);
		OneTimeOnly = true;
	}

	if (app->scene->NotSceneActived == false)
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h};
	app->render->DrawTexture(SpriteIntro, 0, 0, &rect);

	SDL_Rect start = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(SpaceStart, 0, 0, &rect);

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->scene->Reset();
		app->modcontrol->currentscene = 2;		
	}
	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
