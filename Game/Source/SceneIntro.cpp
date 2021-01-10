#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "SceneLogo.h"
#include "SceneOptions.h"
#include "ModuleController.h"
#include "FadeController.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module()
{
	name.Create("sceneintro");

	//Button Play
	play.PushBack ({ 0, 0, 137, 27});
	play.PushBack({ 0, 28, 137, 27});
	play.PushBack({ 0, 54, 137, 27 });

	//Button Continue
	cont.PushBack({ 0, 0, 270, 27 });
	cont.PushBack({ 0, 28, 270, 27 });
	cont.PushBack({ 0, 56, 270, 27 });
	cont.PushBack({ 0, 85, 270, 27 });

	//Button Settings
	sett.PushBack({ 0, 0, 270, 27 });
	sett.PushBack({ 0, 28, 270, 27 });
	sett.PushBack({ 0, 56, 270, 27 });

	//Button credits
	credits.PushBack({ 0, 0, 233, 28 });
	credits.PushBack({ 0, 29, 233, 28 });
	credits.PushBack({ 0, 58, 233, 28 });

	//Button exit
	exit.PushBack({ 0, 0, 122, 28 });
	exit.PushBack({ 0, 29, 122, 28 });
	exit.PushBack({ 0, 56, 122, 28 });


	// GUI: Initialize required controls for the screen //PREPARACION DEL BOTON, CON TAG, POSICION Y COLOR
	btnStart = new GuiButton(1, { 1280 / 2 - 325, 400, 300, 80 }, "START");
	btnStart->SetObserver(this);

	btnContinue = new GuiButton(2, { 1280 / 2 + 25, 400, 300, 80 }, "CONTINUE");
	btnContinue->SetObserver(this);

	btnSettings = new GuiButton(3, { 1280 / 2 - 325, 500, 300, 80 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnCredits = new GuiButton(4, { 1280 / 2 + 25, 500, 300, 80 }, "CREDITS");
	btnCredits->SetObserver(this);

	btnExit = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnExit->SetObserver(this);
}

// Destructor
SceneIntro::~SceneIntro()
{

}

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
	spriteIntro = app->tex->Load("Assets/Screens/Title/intro.png");
	statesPlay = app->tex->Load("Assets/GUI/states_play.png");
	statesCont = app->tex->Load("Assets/GUI/states_continue.png");
	statesSett = app->tex->Load("Assets/GUI/states_sett.png");
	statesCredits = app->tex->Load("Assets/GUI/states_credits.png");
	statesExit = app->tex->Load("Assets/GUI/states_exit30.png");

	buttonFx = app->audio->LoadFx("Assets/Audio/Fx/buttonfx.wav");

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
		app->LoadGameRequest("save_game.xml");
	}

	if (app->SaveDataExist == false && app->fade->CanFade == true)
	{
		btnContinue->state = GuiControlState::DISABLED;
	}
	else if (app->fade->CanFade == true && app->SaveDataExist == true)
	{
		btnContinue->state = GuiControlState::NORMAL;
	}

	if (app->sceneOpts->active == false && app->fade->CanFade == false)
	{
		switch (btnStart->state)
		{
		case GuiControlState::NORMAL: play.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: play.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: play.GetSelectedFrame(0);
			break;
		default:
			break;
		}

		switch (btnContinue->state)
		{
		case GuiControlState::NORMAL: cont.GetSelectedFrame(4);
			break;
		case GuiControlState::FOCUSED: cont.GetSelectedFrame(1);
			break;					   
		case GuiControlState::PRESSED: cont.GetSelectedFrame(0);
			break;					   
		case GuiControlState::DISABLED:cont.GetSelectedFrame(2);
			break;
		default:
			break;
		}

		switch (btnSettings->state)
		{
		case GuiControlState::NORMAL: sett.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: sett.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: sett.GetSelectedFrame(0);
			break;
		default:
			break;
		}

		switch (btnCredits->state)
		{
		case GuiControlState::NORMAL: credits.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: credits.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: credits.GetSelectedFrame(0);
			break;
		default:
			break;
		}

		switch (btnExit->state)
		{
		case GuiControlState::NORMAL: exit.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: exit.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: exit.GetSelectedFrame(0);
			break;
		default:
			break;
		}

		btnStart->Update(app->input, dt);
		btnContinue->Update(app->input, dt);
		btnSettings->Update(app->input, dt);
		btnCredits->Update(app->input, dt);
		btnExit->Update(app->input, dt);
		
		play.Update();
		cont.Update();
		sett.Update();
		credits.Update();
		exit.Update();
	}

	return true;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(spriteIntro, 0, 0, &rect);

	//Render Buttons
	if (app->sceneOpts->active == false)
	{
		SDL_Rect rect1 = play.GetCurrentFrame();
		app->render->DrawTexture(statesPlay, btnStart->bounds.x + ((btnStart->bounds.w - 137)/2), btnStart->bounds.y + ((btnStart->bounds.h - 27) / 2), &rect1);

		SDL_Rect rect2 = cont.GetCurrentFrame();
		app->render->DrawTexture(statesCont, btnContinue->bounds.x + ((btnContinue->bounds.w - 270) / 2), btnContinue->bounds.y + ((btnContinue->bounds.h - 27) / 2), &rect2);

		SDL_Rect rect3 = sett.GetCurrentFrame();
		app->render->DrawTexture(statesSett, btnSettings->bounds.x + ((btnSettings->bounds.w - 270) / 2), btnSettings->bounds.y + ((btnSettings->bounds.h - 27) / 2), &rect3);

		SDL_Rect rect4 = credits.GetCurrentFrame();
		app->render->DrawTexture(statesCredits, btnCredits->bounds.x + ((btnCredits->bounds.w - 233) / 2), btnCredits->bounds.y + ((btnCredits->bounds.h - 28) / 2), &rect4);

		SDL_Rect rect5 = exit.GetCurrentFrame();
		app->render->DrawTexture(statesExit, btnExit->bounds.x + ((btnExit->bounds.w - 122) / 2), btnExit->bounds.y + ((btnExit->bounds.h - 28) / 2), &rect5);

		if (app->modcontrol->showButtons == true)
		{
			btnStart->Draw(app->render);
			btnContinue->Draw(app->render);
			btnSettings->Draw(app->render);
			btnCredits->Draw(app->render);
			btnExit->Draw(app->render);
		}
	}

	return ret;
}

// Called before quitting
bool SceneIntro::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneIntro::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 1:
			app->audio->PlayFx(buttonFx);
			app->fade->CanFade = true;
			app->fade->StartInBlack = false;
			app->fade->StartBoton = true;
			break;

		case 2:
			app->audio->PlayFx(buttonFx);
			app->fade->CanFade = true;
			app->fade->StartInBlack = false;
			app->fade->StartBoton = true;
			app->fade->ContinueBoton = true;
			break;

		case 3:
			app->audio->PlayFx(buttonFx);
			app->sceneOpts->active = true;			// Settings
			break;

		case 4:
			app->audio->PlayFx(buttonFx);
			break;

		case 5:
			app->audio->PlayFx(buttonFx);
			app->fade->CanFade = true;
			app->fade->StartInBlack = false;
			app->fade->StartBoton = true;
			app->fade->ExitBoton = true;
			break;

		}
	}

	default:
		break;
	}

	return true;
}
