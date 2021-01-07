#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneIntro.h"
#include "SceneLogo.h"
#include "Scene.h"
#include "ModuleController.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

SceneIntro::SceneIntro() : Module() //Esto debe de heredar de scene, habria que sacar todos los void que no sean necesarios (no esten puesto en scene) y modificarlos. Ademas necesitamos un scene manager.
{
	name.Create("sceneintro");

	// GUI: Initialize required controls for the screen //PREPARACION DEL BOTON, CON TAG, POSICION Y COLOR
	btnStart = new GuiButton(1, { 1280 / 2 - 300 / 2, 300, 300, 80 }, "START");
	btnStart->SetObserver(this);

	btnExit = new GuiButton(2, { 1280 / 2 - 300 / 2, 400, 300, 80 }, "EXIT");
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
	SpriteIntro = app->tex->Load("Assets/Screens/Title/intro.png");
	SpaceStart = app->tex->Load("Assets/Screens/Title/start.png");

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
	retU = true;

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
	}

	btnStart->Update(app->input, dt);
	btnExit->Update(app->input, dt);

	return retU;
}

// Called each loop iteration
bool SceneIntro::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h};
	app->render->DrawTexture(SpriteIntro, 0, 0, &rect);

	SDL_Rect start = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(SpaceStart, 0, 0, &rect);

	btnStart->Draw(app->render);
	btnExit->Draw(app->render);

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
		if (control->id == 1)
		{
			app->scene->Reset();
			app->modcontrol->currentscene = 2;
		}
		else if (control->id == 2) retU = false; // TODO: Exit request
	}
	default: break;
	}

	return true;
}
