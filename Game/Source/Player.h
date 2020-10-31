#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Point.h"

class Player : public Module
{
public:

	Player();

	// Destructor
	virtual ~Player();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before all Updates
	//bool PostUpdate();

	// Called before quitting
	bool CleanUp();

public:
	
	iPoint position;

	SDL_Texture* hero = nullptr;

	bool gravity = false;
	bool jump = false;
	bool Godmode = false;
	float speedx = 1;
	float speedy = 1;
	float maxJump = 0;

};

#endif //!__PLAYER_H__
