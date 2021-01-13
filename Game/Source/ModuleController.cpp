#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "SceneLogo.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "SceneOptions.h"
#include "ScenePause.h"
#include "Map.h"
#include "Player.h"
#include "WalkingEnemy.h"
#include "FlyEnemy.h"
#include "GameplayHUD.h"
#include "ModuleController.h"
#include "FadeController.h"

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

	app->win->active = true;			// Windows
	app->input->active = true;			// Input
	app->tex->active = true;			// Texture
	app->audio->active = true;			// Audio
	app->map->active = true;			// Map
	app->scene->active = true;			// Scene
	app->sceneintro->active = true;		// SceneIntro
	app->scenelogo->active = true;		// SceneLogo
	app->scenelose->active = true;		// SceneLose
	app->scenewin->active = true;		// SceneWin
	app->scenepause->active = false;	// Pause
	app->sceneopts->active = false;		// Settings
	app->player->active = true;			// Player
	app->wenemy->active = true;			// Walking Enemy
	app->fenemy->active = true;			// Flying Enemy
	app->modcontrol->active = true;		// ModControl
	app->gamehud->active = true;		// GameplayHUD
	app->render->active = true;			// Render
	app->fade->active = true;			// Fade

	currentscene = 0;					// Current Scene
	
	app->SaveGameRequest("StartValues.xml");

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

	return ret;
}

bool ModuleController::Update(float dt)
{
	// DEBUG KEYS
	if (app->scene->notsceneactived)
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
			app->SaveGameRequest("save_game.xml");

		if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
			app->LoadGameRequest("save_game.xml");

		if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && app->player->dead == false)
		{
			if (app->player->CollisionPlayer() == 1)
			{
				app->player->position.x = -(app->scene->poscheck2.x);
				app->player->position.y = -(app->scene->poscheck2.y + 15);
			}
			else if (app->player->CollisionPlayer() == 4)
			{
				app->player->position.x = -(app->scene->poscheck3.x);
				app->player->position.y = -(app->scene->poscheck3.y + 15);
			}
			else if (app->player->CollisionPlayer() == 5)
			{
				app->player->position.x = -(app->scene->poscheck1.x);
				app->player->position.y = -(app->scene->poscheck1.y + 15);
			}
		}

		if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		{
			showbuttons = !showbuttons;
		}
		
		if (app->sceneopts->active == false) app->sceneopts->wait = false;

		if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		{
			showcollider = !showcollider;
		}

		if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		{
			app->player->godmode = !app->player->godmode;
		}

		if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
		{
			capped = !capped;
		}
	}

	if (capped)
		app->cappedms = 30;
	else if (capped == false)
		app->cappedms = 60;

	if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
	{
		currentscene = 0;

		app->scenelogo->Reset();

		app->fade->canfade = true;
		app->fade->startinblack = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_7) == KEY_DOWN)
	{
		currentscene = 1;

		app->scenelogo->musicon = true;
		app->sceneintro->onetimeonly = false;

		app->fade->canfade = true;
		app->fade->startinblack = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		currentscene = 2;

		app->scene->Reset();

		app->fade->canfade = true;
		app->fade->startinblack = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_9) == KEY_DOWN)
	{
		currentscene = 3;

		app->fade->canfade = true;
		app->fade->startinblack = true;
	}
	if (app->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		currentscene = 4;

		app->fade->canfade = true;
		app->fade->startinblack = true;
	}

	if (app->player->life <= 0 && (app->player->deadlanim.FinishedAlready || app->player->deadranim.FinishedAlready))
	{
		app->fade->helose = true;
		app->fade->startinblack = false;
		app->fade->canfade = true;
	}
	
	switch (currentscene)
	{
	case 0:	//Logo

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->notsceneactived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->wenemy->active = false;			// Walking Enemy
		app->fenemy->active = false;			// Flying Enemy
		app->sceneintro->active = false;		// SceneIntro
		app->scenelogo->active = true;			// SceneLogo
		app->scenelose->active = false;			// SceneLose
		app->scenewin->active = false;			// SceneWin
		app->gamehud->active = false;			// GameplayHUD

		app->render->SetBackgroundColor(black);	

		break;

	case 1:	//Intro

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->notsceneactived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->wenemy->active = false;			// Walking Enemy
		app->fenemy->active = false;			// Flying Enemy
		app->sceneintro->active = true;			// SceneIntro
		app->scenelogo->active = false;			// SceneLogo
		app->scenelose->active = false;			// SceneLose
		app->scenewin->active = false;			// SceneWin
		app->gamehud->active = false;			// GameplayHUD

		break;

	case 2:	//Gameplay
		
		app->map->active = true;				// Map
		app->scene->active = true;				// Scene
		app->scene->notsceneactived = true;		// SceneCamera
		app->player->active = true;				// Player
		app->wenemy->active = true;				// Walking Enemy
		app->fenemy->active = true;				// Flying Enemy
		app->sceneintro->active = false;		// SceneIntro
		app->scenelogo->active = false;			// SceneLogo
		app->scenelose->active = false;			// SceneLose
		app->scenewin->active = false;			// SceneWin
		app->gamehud->active = true;			// GameplayHUD

		app->render->SetBackgroundColor(blue);

		break;

	case 3:	//Ending

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->notsceneactived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->wenemy->active = false;			// Walking Enemy
		app->fenemy->active = false;			// Flying Enemy
		app->sceneintro->active = false;		// SceneIntro
		app->scenelogo->active = false;			// SceneLogo
		app->scenelose->active = true;			// SceneLose
		app->scenewin->active = false;			// SceneWin
		app->gamehud->active = false;			// GameplayHUD

		break;

	case 4: //Win

		app->map->active = false;				// Map
		app->scene->active = false;				// Scene
		app->scene->notsceneactived = false;	// SceneCamera
		app->player->active = false;			// Player
		app->wenemy->active = false;			// Walking Enemy
		app->fenemy->active = false;			// Flying Enemy
		app->sceneintro->active = false;		// SceneIntro
		app->scenelogo->active = false;			// SceneLogo
		app->scenelose->active = false;			// SceneLose
		app->scenewin->active = true;			// SceneWin
		app->gamehud->active = false;			// GameplayHUD

		break;

	default:
		break;
	}

	return true;
}


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