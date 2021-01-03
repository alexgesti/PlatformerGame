#ifndef  _GAMEPLAYSCREENHUD_H_
#define _GAMEPLAYSCREENHUD_H_

#include "Module.h"

#include "Animation.h"
struct SDL_Texture;

class ScreenHUD : public Module
{
public:

	ScreenHUD();

	// Destructor
	virtual ~ScreenHUD();

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

	// CollisionOrb
	bool CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapOrb);

	//Called when reseting the level
	bool Reset();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* lifePlayer;
	SDL_Texture* pointsSprite;
	SDL_Texture* time;
	SDL_Texture* collectables;

	int digit1;
	int digit2;
	int digit3;

	Timer timer;

	Animation nums;
	
	Animation* animNum1;
	Animation* animNum2;
	Animation* animNum3;
	Animation* clock;
};


#endif // ! _GAMEPLAYSCREENHUD_H_
#pragma once
