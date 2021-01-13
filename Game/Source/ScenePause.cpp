#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "ModuleController.h"
#include "SceneOptions.h"
#include "ScenePause.h"
#include "FadeController.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

ScenePause::ScenePause() : Module()
{
	name.Create("sceneintro");

	//Button Resume
	resume.PushBack({ 0, 0, 226, 28 });
	resume.PushBack({ 0, 29, 226, 28 });
	resume.PushBack({ 0, 56, 226, 28 });

	//Button Settings
	sett.PushBack({ 0, 0, 270, 27 });
	sett.PushBack({ 0, 28, 270, 27 });
	sett.PushBack({ 0, 56, 270, 27 });

	//Button Back to Tilte
	title.PushBack({ 0, 0, 159, 28 });
	title.PushBack({ 0, 29, 159, 28 });
	title.PushBack({ 0, 58, 159	, 28 });

	//Button Exit
	exit.PushBack({ 0, 0, 122, 28 });
	exit.PushBack({ 0, 29, 122, 28 });
	exit.PushBack({ 0, 56, 122, 28 });

	btnresume = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "RESUME");
	btnresume->SetObserver(this);

	btnsettings = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "SETTINGS");
	btnsettings->SetObserver(this);

	btntitle = new GuiButton(3, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "TITLE");
	btntitle->SetObserver(this);

	btnexit = new GuiButton(4, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnexit->SetObserver(this);
}

// Destructor
ScenePause::~ScenePause()
{

}

// Called before render is available
bool ScenePause::Awake()
{
	LOG("Loading Scene Intro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ScenePause::Start()
{
	statesresume = app->tex->Load("Assets/GUI/states_resume.png");
	statessett = app->tex->Load("Assets/GUI/states_sett.png");
	statestitle = app->tex->Load("Assets/GUI/states_title.png");
	statesexit = app->tex->Load("Assets/GUI/states_exit30.png");
	pause = app->tex->Load("Assets/GUI/pause.png");

	buttonFx = app->audio->LoadFx("Assets/Audio/Fx/buttonfx.wav");

	return true;
}

// Called each loop iteration
bool ScenePause::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool ScenePause::Update(float dt)
{
	if (app->sceneopts->active == false && app->fade->canfade == false)
	{
		switch (btnresume->state)
		{
		case GuiControlState::NORMAL: resume.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: resume.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: resume.GetSelectedFrame(0);
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

		switch (btntitle->state)
		{
		case GuiControlState::NORMAL: title.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: title.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: title.GetSelectedFrame(0);
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

		btnresume->Update(app->input, dt);
		btnsettings->Update(app->input, dt);
		btntitle->Update(app->input, dt);
		btnexit->Update(app->input, dt);

		resume.Update();
		sett.Update();
		title.Update();
		exit.Update();
	}
	
	return true;
}

// Called each loop iteration
bool ScenePause::PostUpdate()
{
	bool ret = true;

	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.h}, { 0, 0, 0, 128});

	app->render->DrawTexture(pause, -app->render->camera.x + ((app->render->camera.w - 467) / 2), -app->render->camera.y + 48);

	if (app->sceneopts->active == false)
	{
		SDL_Rect rect1 = resume.GetCurrentFrame();
		app->render->DrawTexture(statesresume, -app->render->camera.x + btnresume->bounds.x + ((btnresume->bounds.w - 226) / 2), -app->render->camera.y + btnresume->bounds.y + ((btnresume->bounds.h - 28) / 2), &rect1);

		SDL_Rect rect2 = sett.GetCurrentFrame();
		app->render->DrawTexture(statessett, -app->render->camera.x + btnsettings->bounds.x + ((btnsettings->bounds.w - 270) / 2), -app->render->camera.y + btnsettings->bounds.y + ((btnsettings->bounds.h - 27) / 2), &rect2);

		SDL_Rect rect3 = title.GetCurrentFrame();
		app->render->DrawTexture(statestitle, -app->render->camera.x + btntitle->bounds.x + ((btntitle->bounds.w - 159) / 2), -app->render->camera.y + btntitle->bounds.y + ((btntitle ->bounds.h - 28) / 2), &rect3);

		SDL_Rect rect4 = exit.GetCurrentFrame();
		app->render->DrawTexture(statesexit, -app->render->camera.x + btnexit->bounds.x + ((btnexit->bounds.w - 122) / 2), -app->render->camera.y + btnexit->bounds.y + ((btnexit->bounds.h - 28) / 2), &rect4);
	
		if (app->modcontrol->showbuttons == true)
		{
			btnresume->Draw(app->render);
			btnsettings->Draw(app->render);
			btntitle->Draw(app->render);
			btnexit->Draw(app->render);
		}
	}

	return ret;
}

// Called before quitting
bool ScenePause::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool ScenePause::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 1:
			app->audio->PlayFx(buttonFx);
			app->scenepause->active = false;
			break;

		case 2:
			app->audio->PlayFx(buttonFx);
			app->sceneopts->active = true;			// Settings
			break;

		case 3:
			app->audio->PlayFx(buttonFx);
			app->fade->canfade = true;
			app->fade->startinblack = false;
			app->fade->backtitle = true;
			break;

		case 4:
			app->audio->PlayFx(buttonFx);
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
