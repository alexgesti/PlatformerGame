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

	int CheckCollision(fPoint positionMapPlayer);

public:
	bool gravity = false;
	bool jump = true;
	bool Godmode = false;
	bool dead = false;
	bool LookingR = true;
	bool top = false;
	

	float speedx = 16;
	float speedy = 16;
	float maxJump = 0;

	//Player dimensions
	int playerWH = 64;

	fPoint position;

	Animation* currentAnim;

	Animation idleRAnim;
	Animation jumpRAnim;
	Animation runRAnim;
	Animation deadRAnim;
	Animation idleLAnim;
	Animation jumpLAnim;
	Animation runLAnim;
	Animation deadLAnim;

	Colliders* playerCollider = nullptr;

private:
	SDL_Texture* spriteSheet = nullptr;

	static const int numnPoints = 4;
	int pointsCollision[numnPoints][2] = { {0, 0}, {64, 0}, {0, 62}, {64, 62} };
	
public:
	int pointsFloorCollision[numnPoints][2] = { {12, 63}, {52, 63}, {12, 64}, {52, 64} };
};

#endif // __SCENE_H__