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
	bool waslookingRight = true;
	bool IsDead = false;
	bool hitingPlayer = false;

	float speedx = 8;
	float speedy = 2;

	iPoint posCopy;
	iPoint position;

	Animation* currentAnim;

	Animation runRAnim;
	Animation deadRAnim;
	Animation runLAnim;
	Animation deadLAnim;

	Colliders* enemyCollider = nullptr;

private:
	SDL_Texture* spriteSheet = nullptr;

	static const int numnPoints = 4;
	int pointsCollision[numnPoints][2] = { {0, 0}, {32, 0}, {0, 28}, {32, 28} };
	int detectdistance = 192;
};

#endif // __SCENE_H__