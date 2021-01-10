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

	bool CanFade = false;
	bool StartInBlack = false;
	bool StartBoton = false;
	bool ExitBoton = false;
	bool ContinueBoton = false;
	bool BackTitle = false;
	bool WinLoseContinue = false;
	bool HeWin = false;
	bool HeLose = false;

	SDL_Color blackFade = { 0, 0, 0, 0 };

};

#endif // !_FADECONTROLLER_H_
