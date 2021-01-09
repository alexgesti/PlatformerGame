#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "ModuleController.h"
#include "SceneOptions.h"

#include "Defs.h"
#include "Log.h"

#include "SDL_mixer/include/SDL_mixer.h"

SceneOptions::SceneOptions() : Module() //Esto debe de heredar de scene, habria que sacar todos los void que no sean necesarios (no esten puesto en scene) y modificarlos. Ademas necesitamos un scene manager.
{
	name.Create("sceneintro");


	//Button Fullscreen
	fullscreen.PushBack({ 0, 0, 137, 27 });
	fullscreen.PushBack({ 0, 28, 137, 27 });
	fullscreen.PushBack({ 0, 58, 137, 27 });

	//Button Vsync
	vsync.PushBack({ 0, 0, 470, 53 });
	vsync.PushBack({ 0, 54, 470, 53 });
	vsync.PushBack({ 0, 107, 470, 53 });

	//Button Exit
	exit.PushBack({ 0, 0, 122, 28 });
	exit.PushBack({ 0, 29, 122, 28 });
	exit.PushBack({ 0, 56, 122, 28 });

	sldMusic = new GuiSlider(1, { 1280 / 2 - 300 / 2, 200, 300, 80 }, "MUSIC");
	sldMusic->SetObserver(this);

	sldFx = new GuiSlider(2, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "FX");
	sldFx->SetObserver(this);

	btnFull = new GuiButton(1, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "FULLSCREEN");
	btnFull->SetObserver(this);

	btnSync = new GuiButton(2, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "VSYNC");
	btnSync->SetObserver(this);

	btnExit = new GuiButton(3, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
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
	statesFullscreen = app->tex->Load("Assets/GUI/states_fullscreen.png");
	statesVsync = app->tex->Load("Assets/GUI/states_vsync.png");
	statesExit = app->tex->Load("Assets/GUI/states_exit30.png");

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

	Mix_VolumeMusic(sldMusic->percentage);
	Mix_Volume(-1, sldFx->percentage);

	switch (btnFull->state)
	{
	case GuiControlState::NORMAL: fullscreen.GetSelectedFrame(2);
		break;
	case GuiControlState::FOCUSED: fullscreen.GetSelectedFrame(1);
		break;
	case GuiControlState::PRESSED: fullscreen.GetSelectedFrame(3);
		break;
	default:
		break;
	}

	switch (btnSync->state)
	{
	case GuiControlState::NORMAL: vsync.GetSelectedFrame(2);
		break;
	case GuiControlState::FOCUSED: vsync.GetSelectedFrame(1);
		break;
	case GuiControlState::PRESSED: vsync.GetSelectedFrame(3);
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
	case GuiControlState::PRESSED: exit.GetSelectedFrame(3);
		break;
	default:
		break;
	}

	sldMusic->Update(app->input, dt);
	sldFx->Update(app->input, dt);
	btnFull->Update(app->input, dt);
	btnSync->Update(app->input, dt);
	btnExit->Update(app->input, dt);
	
	fullscreen.Update();
	vsync.Update();
	exit.Update();

	return retU;
}

// Called each loop iteration
bool SceneOptions::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect1 = fullscreen.GetCurrentFrame();
	app->render->DrawTexture(statesFullscreen, btnFull->bounds.x, btnFull->bounds.y, &rect1);
	
	SDL_Rect rect2 = vsync.GetCurrentFrame();
	app->render->DrawTexture(statesVsync, btnSync->bounds.x, btnSync->bounds.y, &rect2);
	
	SDL_Rect rect3 = exit.GetCurrentFrame();
	app->render->DrawTexture(statesExit, btnExit->bounds.x, btnExit->bounds.y, &rect3);
	
	if (app->modcontrol->showCollider == true)
	{
		sldMusic->Draw(app->render);
		sldFx->Draw(app->render);
		btnFull->Draw(app->render);
		btnSync->Draw(app->render);
		btnExit->Draw(app->render);
	}

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
