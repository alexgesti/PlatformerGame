/*#include "GameplayScreenHUD.h"
#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "Timer.h"
#include "Player.h"
#include "Scene.h"


#include "Defs.h"
#include "Log.h"



ScreenHUD::ScreenHUD()
{
	//Numbers Timer

	nums.PushBack({6,14,12,22});
	nums.PushBack({20,14,22,22});
	nums.PushBack({44,14,22,22}); 
	nums.PushBack({68,14,22,22});
	nums.PushBack({92,14,22,22});
	nums.PushBack({116,14,22,22});
	nums.PushBack({140,14,22,22});
	nums.PushBack({164,14,22,22});
	nums.PushBack({188,14,22,22});
	nums.PushBack({212,14,22,22});
	nums.PushBack({13,52,23,23});
}

ScreenHUD::~ScreenHUD()
{
}

bool ScreenHUD::Awake()
{
	LOG("Loading ScreenHUD");
	bool ret = true;
	//clock = &nums.GetSelectedFrame(11);
	return ret;
}

bool ScreenHUD::Start()
{

	pointsSprite = app->tex->Load("Assets/Screens/Gameplay/points.png");
	lifePlayer = app->tex->Load("Assets/Screens/Gameplay/lifLife_X64.png");
	time = app->tex->Load("Assets/Screens/Gameplay/time.png");
	

	return true;
}

//Called each loop iteration
bool ScreenHUD::PreUpdate()
{
	return false;
}

bool ScreenHUD::Update(float dt)
{
	if(digit1 == 0 && digit2 == 0 && digit3 ==0)
	{
		//?
	}
	else
	{
		if (timer.ReadSec() >= 1) digit3 --;
		if (digit3 < 0)
		{
			digit2--;
			digit3 = 9;
		}
		if (digit2 < 0)
		{
			digit1--;
			digit2 = 9;
		}
	}
	switch (digit1)
	{
	case 0:
		nums.GetSelectedFrame(10);
		break;
	case 1:
		nums.GetSelectedFrame(1);
		break;
	case 2:
		nums.GetSelectedFrame(2);
		break;
	case 3:
		nums.GetSelectedFrame(3);
		break;
	case 4:
		nums.GetSelectedFrame(4);
		break;
	case 5:
		nums.GetSelectedFrame(5);
		break;
	case 6:
		nums.GetSelectedFrame(6);
		break;
	case 7:
		nums.GetSelectedFrame(7);
		break;
	case 8:
		nums.GetSelectedFrame(8);
		break;
	case 9:
		nums.GetSelectedFrame(9);
		break;
	}
	animNum1 = &nums;

	switch (digit2)
	{
	case 0:
		nums.GetSelectedFrame(10);
		break;
	case 1:
		nums.GetSelectedFrame(1);
		break;
	case 2:
		nums.GetSelectedFrame(2);
		break;
	case 3:
		nums.GetSelectedFrame(3);
		break;
	case 4:
		nums.GetSelectedFrame(4);
		break;
	case 5:
		nums.GetSelectedFrame(5);
		break;
	case 6:
		nums.GetSelectedFrame(6);
		break;
	case 7:
		nums.GetSelectedFrame(7);
		break;
	case 8:
		nums.GetSelectedFrame(8);
		break;
	case 9:
		nums.GetSelectedFrame(9);
		break;
	}
	animNum2 = &nums;

	switch (digit3)
	{
	case 0:
		nums.GetSelectedFrame(10);
		break;
	case 1:
		nums.GetSelectedFrame(1);
		break;
	case 2:
		nums.GetSelectedFrame(2);
		break;
	case 3:
		nums.GetSelectedFrame(3);
		break;
	case 4:
		nums.GetSelectedFrame(4);
		break;
	case 5:
		nums.GetSelectedFrame(5);
		break;
	case 6:
		nums.GetSelectedFrame(6);
		break;
	case 7:
		nums.GetSelectedFrame(7);
		break;
	case 8:
		nums.GetSelectedFrame(8);
		break;
	case 9:
		nums.GetSelectedFrame(9);
		break;
	}
	animNum3 = &nums;

	return true;
}

bool ScreenHUD::PostUpdate()
{
	bool ret = true;

	for (int i = 0; i < app->player->life; i++)
	{
		app->render->DrawTexture(lifePlayer, -app->render->camera.x + (64 * i), -app->render->camera.y);
	}

	SDL_Rect pointRect = app->scene->pointsAnim->GetCurrentFrame();
	app->render->DrawTexture(pointsSprite, -app->render->camera.x + 850, -app->render->camera.y, &pointRect);

	SDL_Rect n1 = animNum1->GetCurrentFrame();
	app->render->DrawTexture(time, -app->render->camera.x + 850, -app->render->camera.y, &n1);
	SDL_Rect n2 = animNum2->GetCurrentFrame();
	app->render->DrawTexture(time, -app->render->camera.x + 850, -app->render->camera.y, &n2);
	SDL_Rect n3 = animNum3->GetCurrentFrame();
	app->render->DrawTexture(time, -app->render->camera.x + 850, -app->render->camera.y, &n3);



	
	return ret;
}

bool ScreenHUD::Reset()
{
	app->LoadGameRequest("StartValues.xml");

	return true;
}

// Called before quitting
bool ScreenHUD::CleanUp()
{
	LOG("Freeing scene");

	return true;
}*/
