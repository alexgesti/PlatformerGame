#ifndef _FADECONTROLLER_H_
#define _FADECONTROLLER_H_

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"

class FadeController : public Module
{
public:

	FadeController();

	virtual ~FadeController();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

public:

	bool canfade = false;
	bool startinblack = false;
	bool startboton = false;
	bool exitboton = false;
	bool continueboton = false;
	bool backtitle = false;
	bool winlosecontinue = false;
	bool hewin = false;
	bool helose = false;

	SDL_Color blackfade = { 0, 0, 0, 0 };

};

#endif // !_FADECONTROLLER_H_
