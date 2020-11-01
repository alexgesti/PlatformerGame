#include <math.h>
/*
#include "App.h"
#include "Module.h"
#include "FateToBlack.h"
#include "Window.h"
#include "Log.h"
#include "Render.h"
#include "ModuleController.h"
#include "Scene.h"
#include "SceneIntro.h"
#include "SceneLogo.h"
#include "SceneLose.h"
#include "External/SDL/include/SDL_render.h"
#include "External/SDL/include/SDL_timer.h"

FadeToBlack::FadeToBlack()
{
	name.Create("fade");

}

FadeToBlack::~FadeToBlack()
{}

bool FadeToBlack::Awake(pugi::xml_node& conf) {


	uint width, height;
	
	screen.w = width + 200;
	screen.h = height;
	screen.x = screen.y = 0;

	return true;
}

// Load assets
bool FadeToBlack::Start()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);

	fadingPlayer = false;
	sceneSwitch = false;
	wantToSwitchScene = "Scene";

	return true;
}

// Update: draw background
bool FadeToBlack::Update(float dt)
{

	if (currentStep == FadeStep::NONE)
		return true;

	Uint32 now = SDL_GetTicks() - startTime;
	float normalized = MIN(1.0f, (float)now / (float)totalTime);

	switch (currentStep)
	{
	case FadeStep::FADE_TO:
	{

	}
	case FadeStep::FADE_FROM:
	{
		normalized = 1.0f - normalized;

		if (now >= totalTime)
			currentStep = FadeStep::NONE;
		
		
		sceneSwitch = false;
	} break;
	}
	SDL_SetRenderDrawColor(app->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(app->render->renderer, &screen);

	return true;
}

bool FadeToBlack::DoFadeToBlack(int level, float time)
{
	bool ret = false;

	nextLevel = level;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}

bool FadeToBlack::FadeToBlackScene(char* scene, float time)
{
	bool ret = false;

	sceneSwitch = true;
	wantToSwitchScene = scene;

	if (currentStep == FadeStep::NONE)
	{
		currentStep = FadeStep::FADE_TO;
		startTime = SDL_GetTicks();
		totalTime = (Uint32)(time * 0.5f * 1000.0f);
		ret = true;
	}

	return ret;
}
*/
