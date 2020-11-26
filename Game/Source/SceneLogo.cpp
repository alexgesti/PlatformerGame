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
	//HACER FADEOUT

	// Check if texture exist
	if (SpriteLogo)
	{
		// Set alpha of the texture
		SDL_SetTextureAlphaMod(SpriteLogo, alpha);
		LOG("Inside SET");
	}

	// Update the alpha value
	if (alpha < SDL_ALPHA_OPAQUE)
	{
		alphaCalc += 5 * 0.16f;
		alpha = alphaCalc;
		LOG("Inside ALPHA increchendo");
	}

	// If alpha is above 255, clamp it
	if (alpha >= SDL_ALPHA_OPAQUE)
	{
		alpha = SDL_ALPHA_OPAQUE;
		alphaCalc = (float)SDL_ALPHA_OPAQUE;
		LOG("Inside STAY 255");
	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(SpriteLogo, 0, 0, &rect);

	
	/*if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->scene->Reset();
		app->modcontrol->currentscene = 1;
	}*/
	return ret;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
