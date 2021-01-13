#ifndef __FLYENEMY_H__
#define __FLYENEMY_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class FlyEnemy : public Module
{
public:

	FlyEnemy();

	// Destructor
	virtual ~FlyEnemy();

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
	bool CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapEnemy);

public:
	bool dead = false;
	bool waslookingright = true;
	bool isdead = false;
	bool hitingplayer = false;
	bool oncesound = false;

	float speedx = 8;
	float speedy = 2;

	iPoint position;

	Animation* currentanim;
	Animation* currentanimcoll;

	Animation runranim;
	Animation deadranim;
	Animation runlanim;
	Animation deadlanim;

	Animation collision;

	Colliders* enemycollider = nullptr;

private:
	SDL_Texture* spritesheet = nullptr;
	SDL_Texture* collisiontex = nullptr;

	int detectdistance = 192;

	//FX
	uint deathenemyfx = 0;
};

#endif // __SCENE_H__