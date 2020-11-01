#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"

#include "Animation.h"

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
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	bool gravity = false;
	bool jump = false;
	bool Godmode = false;
	bool dead = false;
	bool waslookingRight = true;

	float speedx = 15;
	float speedy = 15;
	float maxJump = 0;

	//Player dimensions
	int playerWH = 48;

	iPoint position;

	Animation* currentAnim;

	Animation idleRAnim;
	Animation jumpRAnim;
	Animation runRAnim;
	Animation deadRAnim;
	Animation idleLAnim;
	Animation jumpLAnim;
	Animation runLAnim;
	Animation deadLAnim;

private:
	SDL_Texture* spriteSheet = nullptr;
};

#endif // __SCENE_H__