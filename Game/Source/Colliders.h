#ifndef __COLLIDERS_H__
#define __COLLIDERS_H__

#include "Module.h"

#include "SDL/include/SDL_Rect.h"

class Module;

struct Colliders
{
	enum Type
	{
		NONE = -1,
		FLOOR,
		WATER,
		DISAPPEAR,
		CAMERATP,
		DEAD,
		CAMERAFOLLOW,
		PLAYER,
		
		MAX
	};

	//Methods
	Colliders(SDL_Rect rectangle, Type type, Module* listener = nullptr);

	void SetPos(int x, int y);

	bool Intersects(const SDL_Rect& r) const;

	//Variables
	SDL_Rect rect;
	bool pendingToDelete = false;
	Type type;
	Module* listener = nullptr;
};


#endif // !__COLLIDERS_H__
