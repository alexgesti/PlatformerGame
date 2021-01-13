#ifndef __SCENELOGO_H__
#define __SCENELOGO_H__

#include "Module.h"

struct SDL_Texture;

class SceneLogo : public Module
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
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Reset
	bool Reset();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* spritelogo;

	bool onlystart;

	Uint8 alpha = 0;
	float alphacalc;

	bool alphafinished = false;
	float timer = 0;

public:
	bool musicon;
};

#endif // __SCENELOGO_H__
#pragma once
