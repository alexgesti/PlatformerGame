#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class Entity : public Module
{
public:

	Entity();

	// Destructor
	virtual ~Entity();

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
};

#endif // __SCENE_H__
