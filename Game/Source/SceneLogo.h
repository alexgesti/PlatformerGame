#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "SceneManager.h"

#include "Scene.h"
#include "SceneIntro.h"

struct SDL_Texture;

class SceneLogo : public SceneManager
{
public:

	SceneLogo();

	// Destructor
	virtual ~SceneLogo();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt, Scene* scene);

	// Called before all Updates
	bool PostUpdate();

	// Reset
	bool Reset();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* SpriteLogo;

	bool OnlyStart;

	Uint8 alpha = 0;
	float alphaCalc;

	bool alphaFinished = false;
	float Timer = 0;
};

#endif // __SCENELOGO_H__
#pragma once
