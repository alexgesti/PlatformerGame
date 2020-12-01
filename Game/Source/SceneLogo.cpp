#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "SceneLogo.h"
#include "Scene.h"
#include "ModuleController.h"

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
	SpriteLogo = app->tex->Load("Assets/textures/Logo.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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

	// Update the alpha value positive
	if (alpha < SDL_ALPHA_OPAQUE && alphaFinished == false)
	{
		alphaCalc += 2.5f * 0.16f;
		alpha = alphaCalc;
	}

	// If alpha is above 255, clamp it
	if (alpha >= SDL_ALPHA_OPAQUE && alphaFinished == false)
	{
		alpha = SDL_ALPHA_OPAQUE;
		alphaCalc = (float)SDL_ALPHA_OPAQUE;
		Timer += 1 * 0.16f;
	}

	if (alphaFinished == false && Timer >= 5)
	{
		alphaFinished = true;
		Timer = 0;
	}

	// Update the alpha value negative
	if (alphaFinished == true && alpha > 0)
	{
		alphaCalc -= 2.5f * 0.16f;
		alpha = alphaCalc;
	}

	// If alpha is above 0, clamp it
	if (alphaFinished == true && alpha <= 0)
	{
		alpha = 0;
		alphaCalc = 0;
		Timer += 1 * 0.16f;
	}

	if (alphaFinished == true && Timer >= 5)
	{
		app->scene->Reset();
		app->modcontrol->currentscene = 1;
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

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
