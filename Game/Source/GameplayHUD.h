#ifndef _GAMEPLAYHUD_H_
#define _GAMEPLAYHUD_H_

#include "Module.h"
#include "Timer.h"
#include "Animation.h"

struct SDL_Texture;

class GameplayHUD : public Module
{
public:

	GameplayHUD();

	virtual ~GameplayHUD();

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

	//Called when reseting the level
	bool Reset();

	// Called before quitting
	bool CleanUp();

private:

	SDL_Texture* lifePlayer;
	SDL_Texture* pointsSprite;
	SDL_Texture* timeSprite;

	float Counter = 120;
	int digits[3];

	Timer timer;

	Animation nums;
	Animation clock;

	Animation* CurrentanimNum1;
	Animation* CurrentanimNum2;
	Animation* CurrentanimNum3;
};
#endif
