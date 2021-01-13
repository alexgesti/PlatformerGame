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
	spritelogo = app->tex->Load("Assets/Screens/Logo/logo.png");

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
	if (spritelogo)
	{
		// Set alpha of the texture
		SDL_SetTextureAlphaMod(spritelogo, alpha);
	}

	if (onlystart == false)
	{
		timer++;

		if (timer >= 5)
		{
			onlystart = true;
			timer = 0;
		}

		app->audio->PlayMusic("NULL", 0);
	}

	// Update the alpha value positive
	if (alpha < 254 && alphafinished == false && onlystart == true)
	{
		alphacalc += 15 * 0.16f;
		alpha = alphacalc;
	}

	// If alpha is above 255, clamp it
	if (alpha >= 254 && alphafinished == false)
	{
		alpha = SDL_ALPHA_OPAQUE;
		alphacalc = (float)SDL_ALPHA_OPAQUE;
		timer++;
	}

	if (alphafinished == false && timer >= 100 && onlystart == true)
	{
		alphafinished = true;
		timer = 0;
	}

	// Update the alpha value negative
	if (alphafinished == true && alpha > 0)
	{
		alphacalc -= 15 * 0.16f;
		alpha = alphacalc;
	}

	// If alpha is above 0, clamp it
	if (alphafinished == true && alpha <= 0)
	{
		alpha = 0;
		alphacalc = 0;
		timer++;
	}

	if (alphafinished == true && timer >= 5 && onlystart == true)
	{	
		app->modcontrol->currentscene = 1;
		musicon = true;
		app->fade->blackfade.a = 255;
		app->fade->startinblack = true;
		app->fade->canfade = true;
		app->scene->Reset();
		app->LoadGameRequest("save_game.xml");
	}

	return true;
}

// Called each loop iteration
bool SceneLogo::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = { 0, 0, app->render->camera.w, app->render->camera.h };
	app->render->DrawTexture(spritelogo, 0, 0, &rect);

	return ret;
}

// Reset
bool SceneLogo::Reset()
{
	musicon = false;
	app->sceneintro->onetimeonly = false;
	onlystart = false;
	alphafinished = false;
	timer = 0;
	alpha = 0;

	return true;
}

// Called before quitting
bool SceneLogo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
