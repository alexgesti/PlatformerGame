#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

struct SDL_Texture;

class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update();

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	bool gravity = false;
	bool jump = false;
	bool Godmode = false;

	float speedx = 1;
	float speedy = 1;
	float maxJump = 0;

private:
	SDL_Texture* spriteSheet = nullptr;
};

#endif // __SCENE_H__