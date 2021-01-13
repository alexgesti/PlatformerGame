#ifndef __WALKINGENEMY_H__
#define __WALKINGENEMY_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class WalkingEnemy : public Module
{
public:

	WalkingEnemy();

	// Destructor
	virtual ~WalkingEnemy();

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

	// Collision
	int CollisionEnemy();
	bool CollisionFloorEnemy();

	int CheckCollision(iPoint positionMapEnemy);
	bool CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapEnemy);

public:
	bool gravity = false;
	bool dead = false;
	bool waslookingright = true;
	bool isdead = false;
	bool hitingplayer = false;
	bool oncesound = false;

	float speedx = 4;
	float speedy = 4;

	iPoint position;

	Animation* currentanim;
	Animation* currentanimcoll;

	Animation runranim;
	Animation deadranim;
	Animation idleranim;

	Animation runlanim;
	Animation deadlanim;
	Animation idlelanim;

	Animation collision;

	Colliders* enemycollider = nullptr;

private:
	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* collisiontext = nullptr;

	static const int numnpoints = 4;
	int pointscollision[numnpoints][2] = { {0, 0}, {64, 0}, {0, 62}, {64, 62} };
	int pointsfloorcollision[numnpoints][2] = { {12, 63}, {52, 63}, {12, 64}, {52, 64} };

	//FX
	uint deathenemyfx = 0;
};

#endif // __SCENE_H__