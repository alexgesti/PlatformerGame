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

	//bool CollisionEnemy(fPoint positionMapPlayer);

	//bool CheckCollision(fPoint positionMapPlayer);

public:
	bool dead = false;
	bool waslookingRight = true;

	float speedx = 15;
	float speedy = 15;

	//Enemy dimensions
	int enemyWH = 48;

	fPoint position;

	Animation* currentAnim;

	Animation runRAnim;
	Animation deadRAnim;
	Animation runLAnim;
	Animation deadLAnim;

	Colliders* enemyCollider = nullptr;

private:
	SDL_Texture* spriteSheet = nullptr;

	static const int numnPoints = 4;
	int pointsCollision[numnPoints][2] = { {1, 0}, {47, 0}, {0, 47}, {47, 47} }; // Change
};

#endif // __SCENE_H__