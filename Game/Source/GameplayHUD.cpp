#include "GameplayHUD.h"
#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Textures.h"
#include "Window.h"
#include "Player.h"
#include "Map.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

GameplayHUD::GameplayHUD()
{
	// Numbers Timer
	nums.PushBack({ 6,14,12,22 });
	nums.PushBack({ 20,14,22,22 });
	nums.PushBack({ 44,14,22,22 });
	nums.PushBack({ 68,14,22,22 });
	nums.PushBack({ 92,14,22,22 });
	nums.PushBack({ 116,14,22,22 });
	nums.PushBack({ 140,14,22,22 });
	nums.PushBack({ 164,14,22,22 });
	nums.PushBack({ 188,14,22,22 });
	nums.PushBack({ 212,14,22,22 });

	// Clock
	clock.PushBack({ 13,52,23,23 });
}

GameplayHUD::~GameplayHUD()
{

}

bool GameplayHUD::Awake()
{
	bool ret = true;

	LOG("Loading ScreenHUD");

	return ret;
}

bool GameplayHUD::Start()
{
	//pointsSprite = app->tex->Load("Assets/Screens/Gameplay/points.png");
	//lifePlayer = app->tex->Load("Assets/Screens/Gameplay/lifLife_X64.png");
	timeSprite = app->tex->Load("Assets/Screens/Gameplay/time.png");

	Counter = 120;

	digits[0] = 1;
	digits[1] = 2;
	digits[2] = 0;

	return true;
}

bool GameplayHUD::PreUpdate()
{
	return true;
}

bool GameplayHUD::Update(float dt)
{
	

	//LOG("%d", FrameTimer);

	return true;
}

bool GameplayHUD::PostUpdate()
{
	return true;
}

bool GameplayHUD::Reset()
{
	LOG("Freeing scene");

	return true;
}

bool GameplayHUD::CleanUp()
{
	return true;
}