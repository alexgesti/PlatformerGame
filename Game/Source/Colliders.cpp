#include "Colliders.h"

#include "SDL/include/SDL_Rect.h"

Colliders::Colliders(SDL_Rect rectangle, Type type, Module* listener) : rect(rectangle), type(type), listener(listener)
{

}

void Colliders::SetPos(int x, int y)
{
	rect.x = x;
	rect.y = y;
}

bool Colliders::Intersects(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}