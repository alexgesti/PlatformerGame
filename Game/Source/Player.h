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

	int CollisionPlayer();
	bool CollisionFloorPlayer();

	int CheckCollision(iPoint positionMapPlayer);

public:
	bool gravity = false;
	bool jump = true;
	bool godmode = false;
	bool dead = false;
	bool lookingr = true;
	bool waslookingr = true;
	bool top = false;
	bool shoot;
	bool oncesound = false;

	float speedx = 8;
	float speedy = 8;
	float maxjump = 0;
	float ballspeed = 24;
	float cooldown = 0;

	//Player dimensions
	int playerWH = 64;
	int life = 3;

	iPoint position;
	iPoint bposition;

	Animation* currentanim;
	Animation* bcurrentanim;
	Animation* currentanimcoll;
	Animation* currentanimcollb;

	Animation idleranim;
	Animation jumpranim;
	Animation runranim;
	Animation deadranim;
	Animation idlelanim;
	Animation jumplanim;
	Animation runlanim;
	Animation deadlanim;

	Animation ball;

	Animation collision;
	Animation collisionb;

	Colliders* playercollider = nullptr;

private:
	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* balltext = nullptr;
	SDL_Texture* collisiontext = nullptr;

	static const int numnpoints = 4;
	int pointscollision[numnpoints][2] = { {-8, 0}, {64, 0}, {-8, 55}, {64, 55} };
	
public:
	int pointsfloorcollision[numnpoints][2] = { {8, 56}, {48, 56}, {8, 64}, {48, 64} };

	//FX
	uint jumpFx = 0;
	uint deathFx = 0;
	uint shootFx = 0;
	uint hitFx = 0;
};

#endif // __SCENE_H__