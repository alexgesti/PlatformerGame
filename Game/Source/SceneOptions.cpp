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

SceneOptions::SceneOptions() : Module()
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

	sldmusic = new GuiSlider(1, { 1280 / 2 - 300 / 2, 175, 300, 80 }, "MUSIC");
	sldmusic->SetObserver(this);

	sldfx = new GuiSlider(2, { 1280 / 2 - 300 / 2, 315, 300, 80 }, "FX");
	sldfx->SetObserver(this);

	btnfull = new GuiCheckBox(3, { 1000 / 2 - 300 / 2, 400, 80, 80 }, "FULLSCREEN");
	btnfull->SetObserver(this);

	btnsync = new GuiCheckBox(4, { 1000 / 2 - 300 / 2, 500, 80, 80 }, "VSYNC");
	btnsync->SetObserver(this);

	btnback = new GuiButton(5, { 1280 / 2 - 300 / 2, 600, 300, 80 }, "EXIT");
	btnback->SetObserver(this);
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
	statesguide = app->tex->Load("Assets/GUI/volbar.png");
	statesvolume = app->tex->Load("Assets/GUI/volume.png");
	statesmusic = app->tex->Load("Assets/GUI/music.png");
	statesfx = app->tex->Load("Assets/GUI/fx_volume.png");
	statescheck = app->tex->Load("Assets/GUI/check.png");
	statesfullscreen = app->tex->Load("Assets/GUI/states_fullscreen.png");
	statesvsync = app->tex->Load("Assets/GUI/states_vsync.png");
	statesback = app->tex->Load("Assets/GUI/states_back.png");

	buttonfx = app->audio->LoadFx("Assets/Audio/Fx/buttonfx.wav");

	btnfull->checked = false;
	btnfull->checked = false;

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
		Mix_VolumeMusic(sldmusic->percentage);
		Mix_Volume(-1, sldfx->percentage);

		if (btnfull->checked == true) fullscreen.GetSelectedFrame(1);
		else fullscreen.GetSelectedFrame(0);

		if (btnsync->checked == true) vsync.GetSelectedFrame(1);
		else vsync.GetSelectedFrame(0);

		switch (btnback->state)
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

		sldmusic->Update(app->input, dt);
		sldfx->Update(app->input, dt);
		btnfull->Update(app->input, dt);
		btnsync->Update(app->input, dt);
		btnback->Update(app->input, dt);

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

	if (app->scenepause->active == false)
	{
		app->render->DrawRectangle({ 0, 0, app->render->camera.w, app->render->camera.h }, { 0, 0, 0, 200 });
	}

	app->render->DrawTexture(statesmusic, -app->render->camera.x + btnback->bounds.x + ((btnback->bounds.w - 174) / 2), -app->render->camera.y + 150);
	app->render->DrawTexture(statesvolume, -app->render->camera.x + sldmusic->bounds.x + ((sldmusic->bounds.w - 295) / 2), -app->render->camera.y + sldmusic->bounds.y + ((sldmusic->bounds.h - 24) / 2));
	app->render->DrawTexture(statesguide, -app->render->camera.x + sldmusic->guiderender, -app->render->camera.y + sldmusic->bounds.y + ((sldmusic->bounds.h - 24) / 2));

	app->render->DrawTexture(statesfx, -app->render->camera.x + btnback->bounds.x + ((btnback->bounds.w - 323) / 2), -app->render->camera.y + 275);
	app->render->DrawTexture(statesvolume, -app->render->camera.x + sldfx->bounds.x + ((sldfx->bounds.w - 295) / 2), -app->render->camera.y + sldfx->bounds.y + ((sldfx->bounds.h - 24) / 2));
	app->render->DrawTexture(statesguide, -app->render->camera.x + sldfx->guiderender, -app->render->camera.y + sldfx->bounds.y + ((sldfx->bounds.h - 24) / 2));

	SDL_Rect rect1 = fullscreen.GetCurrentFrame();
	app->render->DrawTexture(statescheck, -app->render->camera.x + btnfull->bounds.x + ((btnfull->bounds.w - 23) / 2), -app->render->camera.y + btnfull->bounds.y + ((btnfull->bounds.h - 23) / 2), &rect1);
	app->render->DrawTexture(statesfullscreen, -app->render->camera.x + btnback->bounds.x + ((btnback->bounds.w - 360) / 2), -app->render->camera.y + btnfull->bounds.y + ((btnback->bounds.h - 27) / 2));
	
	SDL_Rect rect2 = vsync.GetCurrentFrame();
	app->render->DrawTexture(statescheck, -app->render->camera.x + btnsync->bounds.x + ((btnsync->bounds.w - 23) / 2), -app->render->camera.y + btnsync->bounds.y + ((btnsync->bounds.h - 23) / 2), &rect2);
	app->render->DrawTexture(statesvsync, -app->render->camera.x + btnback->bounds.x + ((btnback->bounds.w - 174) / 2), -app->render->camera.y + btnsync->bounds.y + ((btnback->bounds.h - 27) / 2));
	
	SDL_Rect rect3 = back.GetCurrentFrame();
	app->render->DrawTexture(statesback, -app->render->camera.x + btnback->bounds.x + ((btnback->bounds.w - 137) / 2), -app->render->camera.y + btnback->bounds.y + ((btnback->bounds.h - 27) / 2), &rect3);
	
	if (app->modcontrol->showbuttons == true)
	{
		sldmusic->Draw(app->render);
		sldfx->Draw(app->render);
		btnfull->Draw(app->render);
		btnsync->Draw(app->render);
		btnback->Draw(app->render);
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
			app->audio->PlayFx(buttonfx);
			break;
		case 2:
			app->audio->PlayFx(buttonfx);
			break;
		}
	}

	case GuiControlType::CHECKBOX:
	{
		switch (control->id)
		{
		case 3:
			app->audio->PlayFx(buttonfx);
			ToggleFullscreen(app->win->window);
			break;

		case 4:
			app->audio->PlayFx(buttonfx);
			app->modcontrol->capped = !app->modcontrol->capped;
			break;
		}
	}

	case GuiControlType::BUTTON:
	{
		switch (control->id)
		{
		case 5:
			app->audio->PlayFx(buttonfx);
			active = false;			// Settings
			break;
		}
	}

	default: 
		break;
	}

	return true;
}
