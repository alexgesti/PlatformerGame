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
	btnstart = new GuiButton(1, { 1280 / 2 - 325, 400, 300, 80 }, "START");
	btnstart->SetObserver(this);

	btncontinue = new GuiButton(2, { 1280 / 2 + 25, 400, 300, 80 }, "CONTINUE");
	btncontinue->SetObserver(this);

	btnsettings = new GuiButton(3, { 1280 / 2 - 325, 500, 300, 80 }, "SETTINGS");
	btnsettings->SetObserver(this);

	btncredits = new GuiButton(4, { 1280 / 2 + 25, 500, 300, 80 }, "CREDITS");
	btncredits->SetObserver(this);

	btnexit = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnexit->SetObserver(this);
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
	spriteintro = app->tex->Load("Assets/Screens/Title/intro.png");
	creditstext = app->tex->Load("Assets/Screens/Title/credits.png");
	statesplay = app->tex->Load("Assets/GUI/states_play.png");
	statescont = app->tex->Load("Assets/GUI/states_continue.png");
	statessett = app->tex->Load("Assets/GUI/states_sett.png");
	statescredits = app->tex->Load("Assets/GUI/states_credits.png");
	statesexit = app->tex->Load("Assets/GUI/states_exit30.png");

	buttonfx = app->audio->LoadFx("Assets/Audio/Fx/buttonfx.wav");

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
	if (onetimeonly == false && app->scenelogo->musicon == true)
	{
		app->audio->PlayMusic("Assets/Audio/Music/deities_get_takeout_too.ogg", 0);
		onetimeonly = true;
	}

	if (app->scene->notsceneactived == false)
	{
		app->render->camera.x = 0;
		app->render->camera.y = 0;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KeyState::KEY_DOWN ||
		app->input->GetKey(SDL_SCANCODE_SPACE) == KeyState::KEY_DOWN) creditsbut = false;

	if (app->savedataexist == false && app->fade->canfade == true)
	{
		btncontinue->state = GuiControlState::DISABLED;
	}
	else if (app->fade->canfade == true && app->savedataexist == true)
	{
		btncontinue->state = GuiControlState::NORMAL;
	}

	if (app->sceneopts->active == false && app->fade->canfade == false && creditsbut == false)
	{
		switch (btnstart->state)
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

		switch (btncontinue->state)
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

		switch (btnsettings->state)
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

		switch (btncredits->state)
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

		switch (btnexit->state)
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

		btnstart->Update(app->input, dt);
		btncontinue->Update(app->input, dt);
		btnsettings->Update(app->input, dt);
		btncredits->Update(app->input, dt);
		btnexit->Update(app->input, dt);
		
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
	app->render->DrawTexture(spriteintro, 0, 0, &rect);

	//Render Buttons
	if (app->sceneopts->active == false)
	{
		SDL_Rect rect1 = play.GetCurrentFrame();
		app->render->DrawTexture(statesplay, btnstart->bounds.x + ((btnstart->bounds.w - 137)/2), btnstart->bounds.y + ((btnstart->bounds.h - 27) / 2), &rect1);

		SDL_Rect rect2 = cont.GetCurrentFrame();
		app->render->DrawTexture(statescont, btncontinue->bounds.x + ((btncontinue->bounds.w - 270) / 2), btncontinue->bounds.y + ((btncontinue->bounds.h - 27) / 2), &rect2);

		SDL_Rect rect3 = sett.GetCurrentFrame();
		app->render->DrawTexture(statessett, btnsettings->bounds.x + ((btnsettings->bounds.w - 270) / 2), btnsettings->bounds.y + ((btnsettings->bounds.h - 27) / 2), &rect3);

		SDL_Rect rect4 = credits.GetCurrentFrame();
		app->render->DrawTexture(statescredits, btncredits->bounds.x + ((btncredits->bounds.w - 233) / 2), btncredits->bounds.y + ((btncredits->bounds.h - 28) / 2), &rect4);

		SDL_Rect rect5 = exit.GetCurrentFrame();
		app->render->DrawTexture(statesexit, btnexit->bounds.x + ((btnexit->bounds.w - 122) / 2), btnexit->bounds.y + ((btnexit->bounds.h - 28) / 2), &rect5);

		if (app->modcontrol->showbuttons == true)
		{
			btnstart->Draw(app->render);
			btncontinue->Draw(app->render);
			btnsettings->Draw(app->render);
			btncredits->Draw(app->render);
			btnexit->Draw(app->render);
		}
	}

	if (creditsbut == true)
	{
		SDL_Rect rectCred = { 0, 0, app->render->camera.w, app->render->camera.h };
		app->render->DrawTexture(creditstext, -app->render->camera.x, -app->render->camera.y, &rectCred);
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
			app->audio->PlayFx(buttonfx);
			app->fade->canfade = true;
			app->fade->startinblack = false;
			app->fade->startboton = true;
			break;

		case 2:
			app->audio->PlayFx(buttonfx);
			app->fade->canfade = true;
			app->fade->startinblack = false;
			app->fade->startboton = true;
			app->fade->continueboton = true;
			break;

		case 3:
			app->audio->PlayFx(buttonfx);
			app->sceneopts->active = true;			// Settings
			break;

		case 4:
			app->audio->PlayFx(buttonfx);
			creditsbut = !creditsbut;
			break;

		case 5:
			app->audio->PlayFx(buttonfx);
			app->fade->canfade = true;
			app->fade->startinblack = false;
			app->fade->startboton = true;
			app->fade->exitboton = true;
			break;

		}
	}

	default:
		break;
	}

	return true;
}
