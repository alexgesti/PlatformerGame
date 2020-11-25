#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#define MAX_COLLIDERS 100

#include "Module.h"

#include "Colliders.h"

class Collisions : public Module
{
public:
	// Constructor
	// Fills all collision matrix data
	Collisions();

	// Destructor
	~Collisions();

	// Called at the beginning of the application loop
	// Removes all colliders pending to delete
	// Checks for new collisions and calls its listeners
	bool PreUpdate();

	// Called at the middle of the application loop
	// Switches the debug mode on/off
	bool Update(float dt);

	// Called at the end of the application loop
	// Draw all colliders (if debug mode is enabled)
	bool PostUpdate();

	// Removes all existing colliders
	bool CleanUp();

	// Adds a new collider to the list
	Colliders* AddCollider(SDL_Rect rect, Colliders::Type type, Module* listener = nullptr);

	// Removes the collider memory and removes it from the colliders array
	void RemoveCollider(Colliders* collider);

	// Draws all existing colliders with some transparency
	void DebugDraw();

	inline uint GetColliderCount() const { return colliderCount; };

private:
	// All existing colliders in the scene
	Colliders* colliders[MAX_COLLIDERS] = { nullptr };

	// The collision matrix. Defines the interaction for two collider types
	// If set two false, collider 1 will ignore collider 2
	bool matrix[Colliders::Type::MAX][Colliders::Type::MAX];

	// The amount of colliders loaded into the array
	uint colliderCount = 0;

	// Simple debugging flag to draw all colliders
	bool debug = false;
};

#endif // __COLLISIONS_H__
