#ifndef __SceneLOSE_H__
#define __SceneLOSE_H__

#include "SceneManager.h"

#include "Scene.h"

struct SDL_Texture;

class SceneLose : public SceneManager
{
public:

	SceneLose();

	// Destructor
	virtual ~SceneLose();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate(Scene* scene);

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* SpriteLose;
};

#endif // __SceneLOSE_H__
#pragma once
