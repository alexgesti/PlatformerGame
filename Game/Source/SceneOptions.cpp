#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "ModuleController.h"
#include "SceneOptions.h"
#include "ScenePause.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

#include "SDL_mixer/include/SDL_mixer.h"

SceneOptions::SceneOptions() : Module() //Esto debe de heredar de scene, habria que sacar todos los void que no sean necesarios (no esten puesto en scene) y modificarlos. Ademas necesitamos un scene manager.
{
	name.Create("sceneintro");

	//Button Fullscreen
	fullscreen.PushBack({ 0, 0, 23, 23});
	fullscreen.PushBack({ 23, 0, 23, 23 });

	//Button Vsync
	vsync.PushBack({ 0, 0, 23, 23 });
	vsync.PushBack({ 23, 0, 23, 23 });

	//Button Exit
	back.PushBack({ 0, 0, 137, 27 });
	back.PushBack({ 0, 28, 137, 27 });
	back.PushBack({ 0, 56, 137, 27 });

	sldMusic = new GuiSlider(1, { 1280 / 2 - 300 / 2, 175, 300, 80 }, "MUSIC");
	sldMusic->SetObserver(this);

	sldFx = new GuiSlider(2, { 1280 / 2 - 300 / 2, 315, 300, 80 }, "FX");
	sldFx->SetObserver(this);

	btnFull = new GuiCheckBox(3, { 1000 / 2 - 300 / 2, 400, 80, 80 }, "FULLSCREEN");
	btnFull->SetObserver(this);

	btnSync = new GuiCheckBox(4, { 1000 / 2 - 300 / 2, 500, 80, 80 }, "VSYNC");
	btnSync->SetObserver(this);

	btnBack = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
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
	statesGuide = app->tex->Load("Assets/GUI/volbar.png");
	statesVolume = app->tex->Load("Assets/GUI/volume.png");
	statesMusic = app->tex->Load("Assets/GUI/music.png");
	statesFx = app->tex->Load("Assets/GUI/fx_volume.png");
	statesCheck = app->tex->Load("Assets/GUI/check.png");
	statesFullscreen = app->tex->Load("Assets/GUI/states_fullscreen.png");
	statesVsync = app->tex->Load("Assets/GUI/states_vsync.png");
	statesBack = app->tex->Load("Assets/GUI/states_back.png");

	buttonFx = app->audio->LoadFx("Assets/Audio/Fx/buttonfx.wav");

	btnFull->checked = false;
	btnFull->checked = false;

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

		if (btnFull->checked == true) fullscreen.GetSelectedFrame(1);
		else fullscreen.GetSelectedFrame(0);

		if (btnSync->checked == true) vsync.GetSelectedFrame(1);
		else vsync.GetSelectedFrame(0);

		switch (btnBack->state)
		{
		case GuiControlState::NORMAL: back.GetSelectedFrame(2);
			break;
		case GuiControlState::FOCUSED: back.GetSelectedFrame(1);
			break;
		case GuiControlState::PRESSED: back.GetSelectedFrame(0); 
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

	if (app->scenePause->active == false)
	{
		app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.h }, { 0, 0, 0, 200 });
	}

	app->render->DrawTexture(statesMusic, -app->render->camera.x + btnBack->bounds.x + ((btnBack->bounds.w - 174) / 2), -app->render->camera.y + 150);
	app->render->DrawTexture(statesVolume, -app->render->camera.x + sldMusic->bounds.x + ((sldMusic->bounds.w - 295) / 2), -app->render->camera.y + sldMusic->bounds.y + ((sldMusic->bounds.h - 24) / 2));
	app->render->DrawTexture(statesGuide, -app->render->camera.x + sldMusic->guideRender, -app->render->camera.y + sldMusic->bounds.y + ((sldMusic->bounds.h - 24) / 2));

	app->render->DrawTexture(statesFx, -app->render->camera.x + btnBack->bounds.x + ((btnBack->bounds.w - 323) / 2), -app->render->camera.y + 275);
	app->render->DrawTexture(statesVolume, -app->render->camera.x + sldFx->bounds.x + ((sldFx->bounds.w - 295) / 2), -app->render->camera.y + sldFx->bounds.y + ((sldFx->bounds.h - 24) / 2));
	app->render->DrawTexture(statesGuide, -app->render->camera.x + sldFx->guideRender, -app->render->camera.y + sldFx->bounds.y + ((sldFx->bounds.h - 24) / 2));

	SDL_Rect rect1 = fullscreen.GetCurrentFrame();
	app->render->DrawTexture(statesCheck, -app->render->camera.x + btnFull->bounds.x + ((btnFull->bounds.w - 23) / 2), -app->render->camera.y + btnFull->bounds.y + ((btnFull->bounds.h - 23) / 2), &rect1);
	app->render->DrawTexture(statesFullscreen, -app->render->camera.x + btnBack->bounds.x + ((btnBack->bounds.w - 360) / 2), -app->render->camera.y + btnFull->bounds.y + ((btnBack->bounds.h - 27) / 2));
	
	SDL_Rect rect2 = vsync.GetCurrentFrame();
	app->render->DrawTexture(statesCheck, -app->render->camera.x + btnSync->bounds.x + ((btnSync->bounds.w - 23) / 2), -app->render->camera.y + btnSync->bounds.y + ((btnSync->bounds.h - 23) / 2), &rect2);
	app->render->DrawTexture(statesVsync, -app->render->camera.x + btnBack->bounds.x + ((btnBack->bounds.w - 174) / 2), -app->render->camera.y + btnSync->bounds.y + ((btnBack->bounds.h - 27) / 2));
	
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
	case GuiControlType::SLIDER:
	{
		switch (control->id)
		{
		case 1:
			app->audio->PlayFx(buttonFx);
			break;
		case 2:
			app->audio->PlayFx(buttonFx);
			break;
		}
	}

	case GuiControlType::CHECKBOX:
	{
		switch (control->id)
		{
		case 3:
			app->audio->PlayFx(buttonFx);
			ToggleFullscreen(app->win->window);
			break;

		case 4:
			app->audio->PlayFx(buttonFx);
			app->modcontrol->capped = !app->modcontrol->capped;
			break;
		}
	}

	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 5:
			app->audio->PlayFx(buttonFx);
			active = false;			// Settings
			break;
		}
	}

	default: 
		break;
	}

	return true;
}
