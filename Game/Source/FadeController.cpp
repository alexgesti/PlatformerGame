#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "FadeController.h"
#include "Scene.h"
#include "ModuleController.h"
#include "Player.h"
#include "ScenePause.h"

FadeController::FadeController() : Module()
{

}

FadeController::~FadeController()
{

}

bool FadeController::Awake()
{
	return true;
}

bool FadeController::Start()
{
	return true;
}

bool FadeController::PreUpdate()
{
	return true;
}

bool FadeController::Update(float dt)
{
	// Fade General
	if (canfade)
	{
		if (startinblack)
		{
			blackfade.a -= 15 * 0.16f;
			if (blackfade.a <= 5)
			{
				blackfade.a = 0;
				canfade = false;
			}
		}
		else
		{
			blackfade.a += 15 * 0.16f;
			if (blackfade.a >= 245)
			{
				blackfade.a = 255;
				canfade = false;
			}
		}
	}

	// Intro
	if (startboton
		&& blackfade.a >= 245
		&& continueboton == false
		&& exitboton == false)
	{
		canfade = true;
		startinblack = true;
		startboton = false;
		exitboton = false;
		continueboton = false;

		app->player->currentanim = &app->player->idleranim;

		app->scene->Reset();
		app->modcontrol->currentscene = 2;
	}
	else if (startboton
		&& blackfade.a >= 245
		&& continueboton == true
		&& exitboton == false)
	{
		canfade = true;
		startinblack = true;
		startboton = false;
		exitboton = false;
		continueboton = false;

		app->player->currentanim = &app->player->idleranim;

		app->LoadGameRequest("save_game.xml");
		app->modcontrol->currentscene = 2;
	}
	else if (startboton
		&& blackfade.a >= 245
		&& continueboton == false
		&& exitboton == true)
	{
		return false;
	}

	//Scene
	if (hewin && blackfade.a >= 245)
	{
		hewin = false;
		app->modcontrol->currentscene = 4;
		
		canfade = true;
		startinblack = true;
	}

	// Pause
	if (backtitle && blackfade.a >= 245)
	{
		app->scenepause->active = false;
		app->LoadGameRequest("save_game.xml");

		backtitle = false;
		app->modcontrol->currentscene = 1;

		canfade = true;
		startinblack = true;
	}
	
	// win / lose
	if (helose && blackfade.a >= 245)
	{
		app->player->deadranim.Reset();
		app->player->deadlanim.Reset();

		helose = false;
		app->modcontrol->currentscene = 3;
		
		canfade = true;
		startinblack = true;
	}

	if (winlosecontinue && blackfade.a >= 245)
	{
		winlosecontinue = false;
		app->scene->Reset();
		app->modcontrol->currentscene = 1;
	
		canfade = true;
		startinblack = true;
	}

	return true;
}

bool FadeController::PostUpdate()
{
	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.h }, blackfade);

	return true;
}

bool FadeController::CleanUp()
{
	return true;
}