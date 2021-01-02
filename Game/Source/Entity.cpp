#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Render.h"
#include "Window.h"
#include "ModuleController.h"
#include "Pathfinding.h"
#include "Audio.h"
#include "Entity.h"

#include "Defs.h"
#include "Log.h"

Entity::Entity() : Module()
{
	name.Create("entity");
}

// Destructor
Entity::~Entity()
{

}

// Called before render is available
bool Entity::Awake()
{
	LOG("Loading Entity");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Entity::Start()
{
	return true;
}

// Called each loop iteration
bool Entity::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Entity::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool Entity::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Entity::CleanUp()
{
	LOG("Freeing entity");

	return true;
}

bool Entity::CheckCollisionRec(iPoint positionMapBullet, iPoint positionMapEnemy)
{
	return false;
}