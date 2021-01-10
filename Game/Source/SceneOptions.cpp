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
	fullscreen.PushBack({ 0, 0, 546, 27 });
	fullscreen.PushBack({ 0, 28, 546, 27 });
	fullscreen.PushBack({ 0, 56, 546, 27 });

	//Button Vsync
	vsync.PushBack({ 0, 0, 174, 27 });
	vsync.PushBack({ 0, 28, 174, 27 });
	vsync.PushBack({ 0, 56, 174, 27 });

	//Button Exit
	back.PushBack({ 0, 0, 137, 27 });
	back.PushBack({ 0, 28, 137, 27 });
	back.PushBack({ 0, 56, 137, 27 });

	sldMusic = new GuiSlider(1, { 1280 / 2 - 300 / 2, 200, 300, 80 }, "MUSIC");
	sldMusic->SetObserver(this);

	sldFx = new GuiSlider(2, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "FX");
	sldFx->SetObserver(this);

	btnFull = new GuiButton(1, { 1280 / 2 - 600 / 2, 400, 600, 80 }, "FULLSCREEN");
	btnFull->SetObserver(this);

	btnSync = new GuiButton(2, { 1280 / 2 - 300 / 2, 500, 300, 80 }, "VSYNC");
	btnSync->SetObserver(this);

	btnBack = new GuiButton(3, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnBack->SetObserver(this);
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
	statesBack = app->tex->Load("Assets/GUI/states_back.png");

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

	if (wait == true) {
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

		switch (btnBack->state)
		{
		case GuiControlState::NORMAL: back.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: back.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: back.GetSelectedFrame(3);
			break;
		default:
			break;
		}

		sldMusic->Update(app->input, dt);
		sldFx->Update(app->input, dt);
		btnFull->Update(app->input, dt);
		btnSync->Update(app->input, dt);
		btnBack->Update(app->input, dt);

		fullscreen.Update();
		vsync.Update();
		back.Update();
	}
	wait = true;

	return retU;
}

// Called each loop iteration
bool SceneOptions::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect1 = fullscreen.GetCurrentFrame();
	app->render->DrawTexture(statesFullscreen, -app->render->camera.x + btnFull->bounds.x + ((btnFull->bounds.w - 546) / 2), -app->render->camera.y + btnFull->bounds.y + ((btnFull->bounds.h - 27) / 2), &rect1);
	
	SDL_Rect rect2 = vsync.GetCurrentFrame();
	app->render->DrawTexture(statesVsync, -app->render->camera.x + btnSync->bounds.x + ((btnSync->bounds.w - 174) / 2), -app->render->camera.y + btnSync->bounds.y + ((btnSync->bounds.h - 27) / 2), &rect2);
	
	SDL_Rect rect3 = back.GetCurrentFrame();
	app->render->DrawTexture(statesBack, -app->render->camera.x + btnBack->bounds.x + ((btnBack->bounds.w - 137) / 2), -app->render->camera.y + btnBack->bounds.y + ((btnBack->bounds.h - 27) / 2), &rect3);
	
	if (app->modcontrol->showButtons == true)
	{
		sldMusic->Draw(app->render);
		sldFx->Draw(app->render);
		btnFull->Draw(app->render);
		btnSync->Draw(app->render);
		btnBack->Draw(app->render);
	}

	return ret;
}

// Called before quitting
bool SceneOptions::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void SceneOptions::ToggleFullscreen(SDL_Window* Window)
{
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
	SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
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
			ToggleFullscreen(app->win->window);
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
