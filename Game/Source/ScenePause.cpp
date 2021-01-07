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
		btnResume->Update(app->input, dt);
		btnSettings->Update(app->input, dt);
		btnTitle->Update(app->input, dt);
		btnExit->Update(app->input, dt);
	}
	
	return retU;
}

// Called each loop iteration
bool ScenePause::PostUpdate()
{
	bool ret = true;

	if (app->sceneOpts->active == false)
	{
		btnResume->Draw(app->render);
		btnSettings->Draw(app->render);
		btnTitle->Draw(app->render);
		btnExit->Draw(app->render);
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
