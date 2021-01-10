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
	SDL_Texture* timeSprite;

	Animation nums;
	Animation nums2;
	Animation nums3;
	Animation clock;

	Animation points1;
	Animation points2;
	Animation points3;
	Animation points4;
	Animation points5;
	Animation pointsName;

	Animation CoinSimbol;
	Animation coins1;
	Animation coins2;

	Animation* CurrentClock;
	Animation* CurrentanimNum1;
	Animation* CurrentanimNum2;
	Animation* CurrentanimNum3;

	Animation* CurrentpName;
	Animation* CurrentanimP1;
	Animation* CurrentanimP2;
	Animation* CurrentanimP3;
	Animation* CurrentanimP4;
	Animation* CurrentanimP5;

	Animation* CurrentCSimbol;
	Animation* CurrentanimC1;
	Animation* CurrentanimC2;

public:

	int digits[3];
	float Counter = 9;

	int points[5];

	int coins[2];

};
#endif
