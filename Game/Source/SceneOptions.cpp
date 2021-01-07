#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "ModuleController.h"
#include "SceneOptions.h"

#include "Defs.h"
#include "Log.h"

SceneOptions::SceneOptions() : Module() //Esto debe de heredar de scene, habria que sacar todos los void que no sean necesarios (no esten puesto en scene) y modificarlos. Ademas necesitamos un scene manager.
{
	name.Create("sceneintro");

	btnFull = new GuiButton(1, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "FULLSCREEN");
	btnFull->SetObserver(this);

	btnSync = new GuiButton(2, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "VSYNC");
	btnSync->SetObserver(this);

	btnExit = new GuiButton(3, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "Exit");
	btnExit->SetObserver(this);
}

// Destructor
SceneOptions::~SceneOptions()
{

}

// Called before render is available
bool SceneOptions::Awake()
{
	LOG("Loading Scene Intro");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneOptions::Start()
{
	return true;
}

// Called each loop iteration
bool SceneOptions::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneOptions::Update(float dt)
{
	retU = true;

	btnFull->Update(app->input, dt);
	btnSync->Update(app->input, dt);
	btnExit->Update(app->input, dt);
	
	return retU;
}

// Called each loop iteration
bool SceneOptions::PostUpdate()
{
	bool ret = true;

	btnFull->Draw(app->render);
	btnSync->Draw(app->render);
	btnExit->Draw(app->render);

	return ret;
}

// Called before quitting
bool SceneOptions::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

//----------------------------------------------------------
// Manage GUI events for this screen
//----------------------------------------------------------
bool SceneOptions::OnGuiMouseClickEvent(GuiControl* control)
{
	switch (control->type)
	{
	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 1:
			//app->win->fullscreen = !app->win->fullscreen;
			break;

		case 2:
			app->modcontrol->capped = !app->modcontrol->capped;
			break;

		case 3:
			active = false;			// Settings
			break;
		}
	}

	default: 
		break;
	}

	return true;
}
