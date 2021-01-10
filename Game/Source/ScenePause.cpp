#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "ModuleController.h"
#include "SceneOptions.h"
#include "ScenePause.h"

#include "Defs.h"
#include "Log.h"

ScenePause::ScenePause() : Module() //Esto debe de heredar de scene, habria que sacar todos los void que no sean necesarios (no esten puesto en scene) y modificarlos. Ademas necesitamos un scene manager.
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

	btnResume = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "RESUME");
	btnResume->SetObserver(this);

	btnSettings = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "SETTINGS");
	btnSettings->SetObserver(this);

	btnTitle = new GuiButton(3, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "TITLE");
	btnTitle->SetObserver(this);

	btnExit = new GuiButton(4, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnExit->SetObserver(this);
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
	statesResume = app->tex->Load("Assets/GUI/states_resume.png");
	statesSett = app->tex->Load("Assets/GUI/states_sett.png");
	statesTitle = app->tex->Load("Assets/GUI/states_title.png");
	statesExit = app->tex->Load("Assets/GUI/states_exit30.png");
	pause = app->tex->Load("Assets/GUI/pause.png");

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
	retU = true;

	if (app->sceneOpts->active == false)
	{
		switch (btnResume->state)
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

		switch (btnTitle->state)
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

		btnResume->Update(app->input, dt);
		btnSettings->Update(app->input, dt);
		btnTitle->Update(app->input, dt);
		btnExit->Update(app->input, dt);

		resume.Update();
		sett.Update();
		title.Update();
		exit.Update();
	}
	
	return retU;
}

// Called each loop iteration
bool ScenePause::PostUpdate()
{
	bool ret = true;

	app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.h}, { 0, 0, 0, 128});

	app->render->DrawTexture(pause, -app->render->camera.x + ((app->render->camera.w - 467) / 2), -app->render->camera.y + 48);

	if (app->sceneOpts->active == false)
	{
		SDL_Rect rect1 = resume.GetCurrentFrame();
		app->render->DrawTexture(statesResume, -app->render->camera.x + btnResume->bounds.x + ((btnResume->bounds.w - 226) / 2), -app->render->camera.y + btnResume->bounds.y + ((btnResume->bounds.h - 28) / 2), &rect1);

		SDL_Rect rect2 = sett.GetCurrentFrame();
		app->render->DrawTexture(statesSett, -app->render->camera.x + btnSettings->bounds.x + ((btnSettings->bounds.w - 270) / 2), -app->render->camera.y + btnSettings->bounds.y + ((btnSettings->bounds.h - 27) / 2), &rect2);

		SDL_Rect rect3 = title.GetCurrentFrame();
		app->render->DrawTexture(statesTitle, -app->render->camera.x + btnTitle->bounds.x + ((btnTitle->bounds.w - 159) / 2), -app->render->camera.y + btnTitle->bounds.y + ((btnTitle ->bounds.h - 28) / 2), &rect3);

		SDL_Rect rect4 = exit.GetCurrentFrame();
		app->render->DrawTexture(statesExit, -app->render->camera.x + btnExit->bounds.x + ((btnExit->bounds.w - 122) / 2), -app->render->camera.y + btnExit->bounds.y + ((btnExit->bounds.h - 28) / 2), &rect4);
	
		if (app->modcontrol->showButtons == true)
		{
			btnResume->Draw(app->render);
			btnSettings->Draw(app->render);
			btnTitle->Draw(app->render);
			btnExit->Draw(app->render);
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
			app->scenePause->active = false;
			break;

		case 2:
			app->sceneOpts->active = true;			// Settings
			break;

		case 3:
			app->modcontrol->currentscene = 1;

			app->scenePause->active = false;
			break;

		case 4:
			retU = false;
			break;
		}
	}

	default:
		break;
	}

	return true;
}
