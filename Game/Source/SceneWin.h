#ifndef __SceneWIN_H__
#define __SceneWIN_H__

#include "SceneBase.h"

struct SDL_Texture;

class SceneWin : public SceneBase
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
	SDL_Texture* SpriteWin;
};

#endif // __SceneWin_H__
#pragma once
