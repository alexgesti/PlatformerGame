#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "SceneIntro.h"
#include "Scene.h"
#include "ModuleController.h"
#include "Audio.h"
#include "FadeController.h"

#include "Defs.h"
#include "Log.h"

SceneLogo::SceneLogo() : Module()
{
	name.Create("sceneLogo");
}

// Destructor
SceneLogo::~SceneLogo()
{}

// Called before render is available
bool SceneLogo::Awake()
{
	LOG("Loading Scene Logo");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool SceneLogo::Start()
{
	SpriteLogo = app->tex->Load("Assets/Screens/Logo/logo.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	Reset();

	return true;
}

// Called each loop iteration
bool SceneLogo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneLogo::Update(float dt)
{
	// Check if texture exist
	if (SpriteLogo)
	{
		// Set alpha of the texture
		SDL_SetTextureAlphaMod(SpriteLogo, alpha);
	}

	if (OnlyStart == false)
	{
		Timer++;

		if (Timer >= 5)
		{
			OnlyStart = true;
			Timer = 0;
		}

		app->audio->PlayMusic("NULL", 0);
	}

	// Update the alpha value positive
	if (alpha < 254 && alphaFinished == false && OnlyStart == true)
	{
		alphaCalc += 15 * 0.16f;
		alpha = alphaCalc;
	}

	// If alpha is above 255, clamp it
	if (alpha >= 254 && alphaFinished == false)
	{
		alpha = SDL_ALPHA_OPAQUE;
		alphaCalc = (float)SDL_ALPHA_OPAQUE;
		Timer++;
	}

	if (alphaFinished == false && Timer >= 100 && OnlyStart == true)
	{
		alphaFinished = true;
		Timer = 0;
	}

	// Update the alpha value negative
	if (alphaFinished == true && alpha > 0)
	{
		alphaCalc -= 15 * 0.16f;
		alpha = alphaCalc;
	}

	// If alpha is above 0, clamp it
	if (alphaFinished == true && alpha <= 0)
	{
		alpha = 0;
		alphaCalc = 0;
		Timer++;
	}

	if (alphaFinished == true && Timer >= 5 && OnlyStart == true)
	{	
		app->modcontrol->currentscene = 1;
		MusicOn = true;
		app->fade->blackFade.a = 255;
		app->fade->StartInBlack = true;
		app->fade->CanFade = true;
		app->scene->Reset();
	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(SpriteLogo, 0, 0, &rect);

	return ret;
}

// Reset
bool SceneLogo::Reset()
{
	MusicOn = false;
	app->sceneIntro->OneTimeOnly = false;
	OnlyStart = false;
	alphaFinished = false;
	Timer = 0;
	alpha = 0;

	return true;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
