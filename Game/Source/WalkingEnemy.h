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
	bool wasLookingRight = true;
	bool IsDead = false;
	bool hitingPlayer = false;
	bool oncesound = false;

	float speedx = 4;
	float speedy = 4;

	iPoint position;

	Animation* currentAnim;

	Animation runRAnim;
	Animation deadRAnim;
	Animation idleRAnim;

	Animation runLAnim;
	Animation deadLAnim;
	Animation idleLAnim;

	Colliders* enemyCollider = nullptr;

private:
	SDL_Texture* spriteSheet = nullptr;

	static const int numnPoints = 4;
	int pointsCollision[numnPoints][2] = { {0, 0}, {64, 0}, {0, 62}, {64, 62} };
	int pointsFloorCollision[numnPoints][2] = { {12, 63}, {52, 63}, {12, 64}, {52, 64} };

	//FX
	uint deathEnemyFx = 0;
};

#endif // __SCENE_H__