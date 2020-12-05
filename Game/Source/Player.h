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
	bool Godmode = false;
	bool dead = false;
	bool LookingR = true;
	bool WasLookingR = true;
	bool top = false;
	bool shoot;

	float speedx = 8;
	float speedy = 8;
	float maxJump = 0;
	float ballspeed = 24;
	float cooldown = 0;

	//Player dimensions
	int playerWH = 64;
	int life = 3;

	iPoint position;
	iPoint Bposition;

	Animation* currentAnim;
	Animation* BcurrentAnim;

	Animation idleRAnim;
	Animation jumpRAnim;
	Animation runRAnim;
	Animation deadRAnim;
	Animation idleLAnim;
	Animation jumpLAnim;
	Animation runLAnim;
	Animation deadLAnim;

	Animation Ball;

	Colliders* playerCollider = nullptr;

private:
	SDL_Texture* spriteSheet = nullptr;
	SDL_Texture* ball = nullptr;

	static const int numnPoints = 4;
	int pointsCollision[numnPoints][2] = { {0, 0}, {64, 0}, {0, 63}, {64, 63} };
	
public:
	int pointsFloorCollision[numnPoints][2] = { {12, 64}, {52, 64}, {12, 64}, {52, 64} };
};

#endif // __SCENE_H__