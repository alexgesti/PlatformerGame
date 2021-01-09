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
	nums.PushBack({ 6,14,12,22 });		// 1
	nums.PushBack({ 20,14,22,22 });		// 2
	nums.PushBack({ 44,14,22,22 });		// 3
	nums.PushBack({ 68,14,22,22 });		// 4
	nums.PushBack({ 92,14,22,22 });		// 5
	nums.PushBack({ 116,14,22,22 });	// 6
	nums.PushBack({ 140,14,22,22 });	// 7
	nums.PushBack({ 164,14,22,22 });	// 8
	nums.PushBack({ 188,14,22,22 });	// 9
	nums.PushBack({ 212,14,22,22 });	// 0

	// Numbers Timer 2
	nums2.PushBack({ 6,14,12,22 });		// 1
	nums2.PushBack({ 20,14,22,22 });	// 2
	nums2.PushBack({ 44,14,22,22 });	// 3
	nums2.PushBack({ 68,14,22,22 });	// 4
	nums2.PushBack({ 92,14,22,22 });	// 5
	nums2.PushBack({ 116,14,22,22 });	// 6
	nums2.PushBack({ 140,14,22,22 });	// 7
	nums2.PushBack({ 164,14,22,22 });	// 8
	nums2.PushBack({ 188,14,22,22 });	// 9
	nums2.PushBack({ 212,14,22,22 });	// 0

	// Numbers Timer 3
	nums3.PushBack({ 6,14,12,22 });		// 1
	nums3.PushBack({ 20,14,22,22 });	// 2
	nums3.PushBack({ 44,14,22,22 });	// 3
	nums3.PushBack({ 68,14,22,22 });	// 4
	nums3.PushBack({ 92,14,22,22 });	// 5
	nums3.PushBack({ 116,14,22,22 });	// 6
	nums3.PushBack({ 140,14,22,22 });	// 7
	nums3.PushBack({ 164,14,22,22 });	// 8
	nums3.PushBack({ 188,14,22,22 });	// 9
	nums3.PushBack({ 212,14,22,22 });	// 0

	// Points 1
	points1.PushBack({ 212,14,22,22 });	// 0

	// Points 2
	points2.PushBack({ 212,14,22,22 });	// 0

	// Points 3
	points3.PushBack({ 6,14,12,22 });	// 1
	points3.PushBack({ 20,14,22,22 });	// 2
	points3.PushBack({ 44,14,22,22 });	// 3
	points3.PushBack({ 68,14,22,22 });	// 4
	points3.PushBack({ 92,14,22,22 });	// 5
	points3.PushBack({ 116,14,22,22 });	// 6
	points3.PushBack({ 140,14,22,22 });	// 7
	points3.PushBack({ 164,14,22,22 });	// 8
	points3.PushBack({ 188,14,22,22 });	// 9
	points3.PushBack({ 212,14,22,22 });	// 0

	// Points 4
	points4.PushBack({ 6,14,12,22 });	// 1
	points4.PushBack({ 20,14,22,22 });	// 2
	points4.PushBack({ 44,14,22,22 });	// 3
	points4.PushBack({ 68,14,22,22 });	// 4
	points4.PushBack({ 92,14,22,22 });	// 5
	points4.PushBack({ 116,14,22,22 });	// 6
	points4.PushBack({ 140,14,22,22 });	// 7
	points4.PushBack({ 164,14,22,22 });	// 8
	points4.PushBack({ 188,14,22,22 });	// 9
	points4.PushBack({ 212,14,22,22 });	// 0

	// Points 5
	points5.PushBack({ 6,14,12,22 });	// 1
	points5.PushBack({ 20,14,22,22 });	// 2
	points5.PushBack({ 44,14,22,22 });	// 3
	points5.PushBack({ 68,14,22,22 });	// 4
	points5.PushBack({ 92,14,22,22 });	// 5
	points5.PushBack({ 116,14,22,22 });	// 6
	points5.PushBack({ 140,14,22,22 });	// 7
	points5.PushBack({ 164,14,22,22 });	// 8
	points5.PushBack({ 188,14,22,22 });	// 9
	points5.PushBack({ 212,14,22,22 });	// 0

	// Coins 1
	coins1.PushBack({ 6,14,12,22 });	// 1
	coins1.PushBack({ 20,14,22,22 });	// 2
	coins1.PushBack({ 44,14,22,22 });	// 3
	coins1.PushBack({ 68,14,22,22 });	// 4
	coins1.PushBack({ 92,14,22,22 });	// 5
	coins1.PushBack({ 116,14,22,22 });	// 6
	coins1.PushBack({ 140,14,22,22 });	// 7
	coins1.PushBack({ 164,14,22,22 });	// 8
	coins1.PushBack({ 188,14,22,22 });	// 9
	coins1.PushBack({ 212,14,22,22 });	// 0

	// Coins 2
	coins2.PushBack({ 6,14,12,22 });	// 1
	coins2.PushBack({ 20,14,22,22 });	// 2
	coins2.PushBack({ 44,14,22,22 });	// 3
	coins2.PushBack({ 68,14,22,22 });	// 4
	coins2.PushBack({ 92,14,22,22 });	// 5
	coins2.PushBack({ 116,14,22,22 });	// 6
	coins2.PushBack({ 140,14,22,22 });	// 7
	coins2.PushBack({ 164,14,22,22 });	// 8
	coins2.PushBack({ 188,14,22,22 });	// 9
	coins2.PushBack({ 212,14,22,22 });	// 0

	// Clock
	clock.PushBack({ 13,52,23,23 });

	// Points Name
	pointsName.PushBack({48, 48, 103, 22});

	// Coin Simbol
	CoinSimbol.PushBack({176, 48, 43, 23});

	CurrentClock = &clock;

	CurrentpName = &pointsName;

	CurrentCSimbol = &CoinSimbol;

	CurrentanimP5 = &points1;
	CurrentanimP4 = &points2;
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
	lifePlayer = app->tex->Load("Assets/Screens/Gameplay/lifLife_X64.png");
	timeSprite = app->tex->Load("Assets/Screens/Gameplay/time.png");

	digits[0] = 1;
	digits[1] = 2;
	digits[2] = 0;

	points[0] = 0;
	points[1] = 0;
	points[2] = 0;
	points[3] = 0;
	points[4] = 0;

	Counter = 9;

	coins[0] = 0;
	coins[1] = 0;

	return true;
}

bool GameplayHUD::PreUpdate()
{
	return true;
}

bool GameplayHUD::Update(float dt)
{
	// Clock
	Counter -= 0.16f;

	if (Counter < 0)
	{
		Counter = 9;
		digits[2]--;
	}

	if (digits[2] < 0)
	{
		digits[2] = 9;
		digits[1]--;

		if (digits[1] < 0)
		{
			digits[1] = 9;
			digits[0]--;

			if (digits[0] < 0)
			{
				digits[0] = 0;
				digits[1] = 0;
				digits[2] = 0;
				app->player->life = 0;
			}
		}
	}

	LOG("%d %d %d", digits[0], digits[1], digits[2]);

	switch (digits[0])
	{
	case 0:
		nums.GetSelectedFrame(9);
		break;

	case 1:
		nums.GetSelectedFrame(0);
		break;

	case 2:
		nums.GetSelectedFrame(1);
		break;
	}

	CurrentanimNum1 = &nums;

	switch (digits[1])
	{
	case 0:
		nums2.GetSelectedFrame(9);
		break;

	case 1:
		nums2.GetSelectedFrame(0);
		break;

	case 2:
		nums2.GetSelectedFrame(1);
		break;

	case 3:
		nums2.GetSelectedFrame(2);
		break;
	
	case 4:
		nums2.GetSelectedFrame(3);
		break;

	case 5:
		nums2.GetSelectedFrame(4);
		break;

	case 6:
		nums2.GetSelectedFrame(5);
		break;

	case 7:
		nums2.GetSelectedFrame(6);
		break;

	case 8:
		nums2.GetSelectedFrame(7);
		break;

	case 9:
		nums2.GetSelectedFrame(8);
		break;
	}

	CurrentanimNum2 = &nums2;

	switch(digits[2])
	{
	case 0:
		nums3.GetSelectedFrame(9);
		break;

	case 1:
		nums3.GetSelectedFrame(0);
		break;

	case 2:
		nums3.GetSelectedFrame(1);
		break;

	case 3:
		nums3.GetSelectedFrame(2);
		break;

	case 4:
		nums3.GetSelectedFrame(3);
		break;

	case 5:
		nums3.GetSelectedFrame(4);
		break;

	case 6:
		nums3.GetSelectedFrame(5);
		break;

	case 7:
		nums3.GetSelectedFrame(6);
		break;

	case 8:
		nums3.GetSelectedFrame(7);
		break;

	case 9:
		nums3.GetSelectedFrame(8);
		break;
	}

	CurrentanimNum3 = &nums3;

	// Points

	if (points[2] > 9)
	{
		points[2] = 0;
		points[1]++;

		if (points[1] > 9)
		{
			points[1] = 0;
			points[0]++;

			if (points[0] > 9)
			{
				points[0] = 9;
			}
		}
	}

	// Points 1
	switch (points[0])
	{
	case 0:
		points5.GetSelectedFrame(9);
		break;

	case 1:
		points5.GetSelectedFrame(0);
		break;

	case 2:
		points5.GetSelectedFrame(1);
		break;

	case 3:
		points5.GetSelectedFrame(2);
		break;

	case 4:
		points5.GetSelectedFrame(3);
		break;

	case 5:
		points5.GetSelectedFrame(4);
		break;

	case 6:
		points5.GetSelectedFrame(5);
		break;

	case 7:
		points5.GetSelectedFrame(6);
		break;

	case 8:
		points5.GetSelectedFrame(7);
		break;

	case 9:
		points5.GetSelectedFrame(8);
		break;
	}

	CurrentanimP1 = &points5;

	// Points 2
	switch (points[1])
	{
	case 0:
		points4.GetSelectedFrame(9);
		break;

	case 1:
		points4.GetSelectedFrame(0);
		break;

	case 2:
		points4.GetSelectedFrame(1);
		break;

	case 3:
		points4.GetSelectedFrame(2);
		break;

	case 4:
		points4.GetSelectedFrame(3);
		break;

	case 5:
		points4.GetSelectedFrame(4);
		break;

	case 6:
		points4.GetSelectedFrame(5);
		break;

	case 7:
		points4.GetSelectedFrame(6);
		break;

	case 8:
		points4.GetSelectedFrame(7);
		break;

	case 9:
		points4.GetSelectedFrame(8);
		break;
	}

	CurrentanimP2 = &points4;

	// Points 3
	switch (points[2])
	{
	case 0:
		points3.GetSelectedFrame(9);
		break;

	case 1:
		points3.GetSelectedFrame(0);
		break;

	case 2:
		points3.GetSelectedFrame(1);
		break;

	case 3:
		points3.GetSelectedFrame(2);
		break;

	case 4:
		points3.GetSelectedFrame(3);
		break;

	case 5:
		points3.GetSelectedFrame(4);
		break;

	case 6:
		points3.GetSelectedFrame(5);
		break;

	case 7:
		points3.GetSelectedFrame(6);
		break;

	case 8:
		points3.GetSelectedFrame(7);
		break;

	case 9:
		points3.GetSelectedFrame(8);
		break;
	}

	CurrentanimP3 = &points3;

	// "Coins"
	if (coins[1] > 9)
	{
		coins[1] = 0;
		coins[0]++;

		if (coins[0] > 9)
		{
			coins[0] = 9;
			coins[1] = 9;
		}
	}

	// Coins 1
	switch (coins[1])
	{
	case 0:
		coins2.GetSelectedFrame(9);
		break;

	case 1:
		coins2.GetSelectedFrame(0);
		break;

	case 2:
		coins2.GetSelectedFrame(1);
		break;

	case 3:
		coins2.GetSelectedFrame(2);
		break;

	case 4:
		coins2.GetSelectedFrame(3);
		break;

	case 5:
		coins2.GetSelectedFrame(4);
		break;

	case 6:
		coins2.GetSelectedFrame(5);
		break;

	case 7:
		coins2.GetSelectedFrame(6);
		break;

	case 8:
		coins2.GetSelectedFrame(7);
		break;

	case 9:
		coins2.GetSelectedFrame(8);
		break;
	}

	CurrentanimC2 = &coins2;

	// Coins 2
	switch (coins[0])
	{
	case 0:
		coins1.GetSelectedFrame(9);
		break;

	case 1:
		coins1.GetSelectedFrame(0);
		break;

	case 2:
		coins1.GetSelectedFrame(1);
		break;

	case 3:
		coins1.GetSelectedFrame(2);
		break;

	case 4:
		coins1.GetSelectedFrame(3);
		break;

	case 5:
		coins1.GetSelectedFrame(4);
		break;

	case 6:
		coins1.GetSelectedFrame(5);
		break;

	case 7:
		coins1.GetSelectedFrame(6);
		break;

	case 8:
		coins1.GetSelectedFrame(7);
		break;

	case 9:
		coins1.GetSelectedFrame(8);
		break;
	}

	CurrentanimC1 = &coins1;

	return true;
}

bool GameplayHUD::PostUpdate()
{
	// Clock
	SDL_Rect cl = CurrentClock->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 105), -app->render->camera.y + 15, &cl);

	SDL_Rect n1 = CurrentanimNum1->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 77), -app->render->camera.y + 15, &n1);

	SDL_Rect n2 = CurrentanimNum2->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 53), -app->render->camera.y + 15, &n2);

	SDL_Rect n3 = CurrentanimNum3->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 29), -app->render->camera.y + 15, &n3);

	// Points
	SDL_Rect PN = CurrentpName->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 366), -app->render->camera.y + 15, &PN);

	SDL_Rect p1 = CurrentanimP1->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 249), -app->render->camera.y + 15, &p1);
																   
	SDL_Rect p2 = CurrentanimP2->GetCurrentFrame();				   
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 225), -app->render->camera.y + 15, &p2);
																   
	SDL_Rect p3 = CurrentanimP3->GetCurrentFrame();				   
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 201), -app->render->camera.y + 15, &p3);

	SDL_Rect p4 = CurrentanimP4->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 177), -app->render->camera.y + 15, &p4);

	SDL_Rect p5 = CurrentanimP5->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + (app->win->width - 153), -app->render->camera.y + 15, &p5);

	// Life
	for (int i = 0; i < app->player->life; i++)
	{
		app->render->DrawTexture(lifePlayer, -app->render->camera.x + (64 * i), -app->render->camera.y);
	}

	// Coins
	SDL_Rect CoinS = CurrentCSimbol->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + 12, -app->render->camera.y + 64, &CoinS);

	SDL_Rect coin1 = CurrentanimC1->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + 63, -app->render->camera.y + 64, &coin1);

	SDL_Rect coin2 = CurrentanimC2->GetCurrentFrame();
	app->render->DrawTexture(timeSprite, -app->render->camera.x + 87, -app->render->camera.y + 64, &coin2);

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