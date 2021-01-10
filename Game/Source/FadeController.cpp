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
	if (CanFade)
	{
		if (StartInBlack)
		{
			blackFade.a -= 15 * 0.16f;
			if (blackFade.a <= 5)
			{
				blackFade.a = 0;
				CanFade = false;
			}
		}
		else
		{
			blackFade.a += 15 * 0.16f;
			if (blackFade.a >= 245)
			{
				blackFade.a = 255;
				CanFade = false;
			}
		}
	}

	// Intro
	if (StartBoton
		&& blackFade.a >= 245
		&& ContinueBoton == false
		&& ExitBoton == false)
	{
		CanFade = true;
		StartInBlack = true;
		StartBoton = false;
		ExitBoton = false;
		ContinueBoton = false;

		app->player->currentAnim = &app->player->idleRAnim;

		app->scene->Reset();
		app->modcontrol->currentscene = 2;
	}
	else if (StartBoton
		&& blackFade.a >= 245
		&& ContinueBoton == true
		&& ExitBoton == false)
	{
		CanFade = true;
		StartInBlack = true;
		StartBoton = false;
		ExitBoton = false;
		ContinueBoton = false;

		app->player->currentAnim = &app->player->idleRAnim;

		app->LoadGameRequest("save_game.xml");
		app->modcontrol->currentscene = 2;
	}
	else if (StartBoton
		&& blackFade.a >= 245
		&& ContinueBoton == false
		&& ExitBoton == true)
	{
		return false;
	}

	//Scene
	if (HeWin && blackFade.a >= 245)
	{
		HeWin = false;
		app->modcontrol->currentscene = 4;
		
		CanFade = true;
		StartInBlack = true;
	}

	// Pause
	if (BackTitle && blackFade.a >= 245)
	{
		app->scenePause->active = false;
		app->LoadGameRequest("save_game.xml");

		BackTitle = false;
		app->modcontrol->currentscene = 1;

		CanFade = true;
		StartInBlack = true;
	}
	
	// Win / Lose
	if (HeLose && blackFade.a >= 245)
	{
		app->player->deadRAnim.Reset();
		app->player->deadLAnim.Reset();

		HeLose = false;
		app->modcontrol->currentscene = 3;
		
		CanFade = true;
		StartInBlack = true;
	}

	if (WinLoseContinue && blackFade.a >= 245)
	{
		WinLoseContinue = false;
		app->scene->Reset();
		app->modcontrol->currentscene = 1;
	
		CanFade = true;
		StartInBlack = true;
	}

	return true;
}

bool FadeController::PostUpdate()
{
	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.h }, blackFade);

	return true;
}

bool FadeController::CleanUp()
{
	return true;
}