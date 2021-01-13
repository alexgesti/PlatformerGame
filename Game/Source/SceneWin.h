#ifndef __SceneWIN_H__
#define __SceneWIN_H__

#include "Module.h"

struct SDL_Texture;

class SceneWin : public Module
{
public:

	SceneWin();

	// Destructor
	virtual ~SceneWin();

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

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* spritewin;
};

#endif // __SceneWin_H__
#pragma once
