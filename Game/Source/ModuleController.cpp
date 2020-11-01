#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "Map.h"
#include "Player.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

ModuleController::ModuleController() : Module()
{

}

// Destructor
ModuleController::~ModuleController()
{
	// Release modules
	ListItem<Module*>* item = modules.end;

	while (item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void ModuleController::AddModule(Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool ModuleController::Awake(pugi::xml_node& config)
{
	LOG("Loading Module Controller");

	app->win->active = true;		// Windows
	app->input->active = true;		// Input
	app->tex->active = true;		// Texture
	app->audio->active = true;		// Audio
	app->map->active = true;		// Map
	app->scene->active = true;		// Scene
	app->sceneIntro->active = true;	// SceneIntro
	app->player->active = true;		// Player
	app->modcontrol->active = true;	// ModControl
	app->render->active = true;		// Render

	currentscene = 1;				// Current Scene
	
	return true;
}

// Called before the first frame
bool ModuleController::Start()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.start;

	while (item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	// Load music
	app->audio->PlayMusic("Assets/audio/music/deities_get_takeout_too.ogg", 0);

	return ret;
}

bool ModuleController::Update(float dt)
{
	// DEBUG KEYS
	if (app->scene->NotSceneActived)
	{
		if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		{
			currentscene = 2;
			app->scene->Reset();
		}

		if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && currentscene == 2)
		{
			app->scene->Reset();
		}

		if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
			app->SaveGameRequest("savegame.xml");

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadGameRequest("savegame.xml");

		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		{
			if (app->map->showCollider == false)
				app->map->showCollider = true;
			else if (app->map->showCollider == true)
				app->map->showCollider = false;
		}

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
			if (app->player->Godmode == false)
				app->player->Godmode = true;
			else if (app->player->Godmode == true)
				app->player->Godmode = false;
	}


	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN) currentscene = 0;
	if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) currentscene = 1;
	if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		currentscene = 2;

		app->scene->Reset();
	}
	if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) currentscene = 3;

	switch (currentscene)
	{
	case 0:	//Logo

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->sceneIntro->active = false;		// SceneIntro

		break;

	case 1:	//Intro

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->sceneIntro->active = true;			// SceneIntro

		break;

	case 2:	//Gameplay

		app->map->active = true;				// Map
		app->scene->active = true;				// Scene
		app->scene->NotSceneActived = true;		// SceneCamera
		app->player->active = true;				// Player
		app->sceneIntro->active = false;		// SceneIntro
		

		break;

	case 3:	//Ending

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->NotSceneActived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->sceneIntro->active = false;		// SceneIntro

		break;

	default:
		break;
	}

	return true;
}

// Load Scene State (Underconstruction)
//bool ModuleController::LoadState(pugi::xml_node& data)
//{
//	currentscene = data.child("currentscene").attribute("Scene").as_int();
//
//	return true;
//}

// Save Scene State (Underconstruction)
//bool ModuleController::SaveState(pugi::xml_node& data) const
//{
//	pugi::xml_node scene = data.append_child("currentscene");
//
//	scene.append_attribute("Scene") = currentscene;
//
//	return true;
//}

bool ModuleController::CleanUp()
{
	bool ret = true;
	ListItem<Module*>* item;
	item = modules.end;

	while (item != NULL && ret == true && item->data->active == false)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}