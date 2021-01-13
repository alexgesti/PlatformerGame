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

	SDL_Texture* lifeplayer;
	SDL_Texture* timesprite;

	Animation nums;
	Animation nums2;
	Animation nums3;
	Animation clock;

	Animation points1;
	Animation points2;
	Animation points3;
	Animation points4;
	Animation points5;
	Animation pointsname;

	Animation coinsimbol;
	Animation coins1;
	Animation coins2;

	Animation* currentclock;
	Animation* currentanimnum1;
	Animation* currentanimnum2;
	Animation* currentanimnum3;

	Animation* currentpname;
	Animation* currentanimp1;
	Animation* currentanimp2;
	Animation* currentanimp3;
	Animation* currentanimp4;
	Animation* currentanimp5;

	Animation* currentcsimbol;
	Animation* currentanimc1;
	Animation* currentanimc2;

public:

	int digits[3];
	float counter = 9;

	int points[5];

	int coins[2];

};
#endif
