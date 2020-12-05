#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	//idle rigth animation
	idleRAnim.PushBack({ 0, 0, 64, 64 });
	idleRAnim.PushBack({ 64, 0, 64, 64 });
	idleRAnim.PushBack({ 128, 0, 64, 64 });
	idleRAnim.PushBack({ 192, 0, 64, 64 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.5f;

	//jump rigth animation
	jumpRAnim.PushBack({ 0, 64, 64, 64 });
	jumpRAnim.PushBack({ 64, 64, 64, 64 });
	jumpRAnim.PushBack({ 128, 64, 64, 64 });
	jumpRAnim.loop = true;
	jumpRAnim.speed = 0.5f;

	//run rigth animation
	runRAnim.PushBack({ 0, 128, 64, 64 });
	runRAnim.PushBack({ 64, 128, 64, 64 });
	runRAnim.PushBack({ 128, 128, 64, 64 });
	runRAnim.PushBack({ 192, 128, 64, 64 });
	runRAnim.PushBack({ 256, 128, 64, 64 });
	runRAnim.PushBack({ 320, 128, 64, 64 });
	runRAnim.loop = true;
	runRAnim.speed = 0.5f;

	//dead rigth animation
	deadRAnim.PushBack({ 0, 192, 64, 64 });
	deadRAnim.PushBack({ 64, 192, 64, 64 });
	deadRAnim.PushBack({ 128, 192, 64, 64 });
	deadRAnim.PushBack({ 192, 192, 64, 64 });
	deadRAnim.PushBack({ 256, 192, 64, 64 });
	deadRAnim.PushBack({ 320, 192, 64, 64 });
	deadRAnim.PushBack({ 384, 192, 64, 64 });
	deadRAnim.PushBack({ 448, 192, 64, 64 });
	deadRAnim.PushBack({ 512, 192, 64, 64 });
	deadRAnim.loop = false;
	deadRAnim.speed = 0.5f;

	//idle left animation
	idleLAnim.PushBack({ 0, 256, 64, 64 });
	idleLAnim.PushBack({ 64, 256, 64, 64 });
	idleLAnim.PushBack({ 128, 256, 64, 64 });
	idleLAnim.PushBack({ 192, 256, 64, 64 });
	idleLAnim.loop = true;
	idleLAnim.speed = 0.5f;

	//jump left animation
	jumpLAnim.PushBack({ 0, 320, 64, 64 });
	jumpLAnim.PushBack({ 64, 320, 64, 64 });
	jumpLAnim.PushBack({ 128, 320, 64, 64 });
	jumpLAnim.loop = true;
	jumpLAnim.speed = 0.5f;

	//run left animation
	runLAnim.PushBack({ 0, 384, 64, 64 });
	runLAnim.PushBack({ 64, 384, 64, 64 });
	runLAnim.PushBack({ 128, 384, 64, 64 });
	runLAnim.PushBack({ 192, 384, 64, 64 });
	runLAnim.PushBack({ 256, 384, 64, 64 });
	runLAnim.PushBack({ 320, 384, 64, 64 });
	runLAnim.loop = true;
	runLAnim.speed = 0.5f;

	//dead left animation
	deadLAnim.PushBack({ 0, 448, 64, 64 });
	deadLAnim.PushBack({ 64, 448, 64, 64 });
	deadLAnim.PushBack({ 128, 448, 64, 64 });
	deadLAnim.PushBack({ 192, 448, 64, 64 });
	deadLAnim.PushBack({ 256, 448, 64, 64 });
	deadLAnim.PushBack({ 320, 448, 64, 64 });
	deadLAnim.PushBack({ 384, 448, 64, 64 });
	deadLAnim.PushBack({ 448, 448, 64, 64 });
	deadLAnim.PushBack({ 512, 448, 64, 64 });
	deadLAnim.loop = false;
	deadLAnim.speed = 0.5f;

	//Shoot NULL
	BallN.PushBack({ 0, 0, 0, 0 });

	//Shoot Real
	BallS.PushBack({ 0, 0, 16, 16 });
}

// Destructor
Player::~Player()
{}

// Called before render is available
bool Player::Awake()
{
	LOG("Loading Player");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Player::Start()
{
	//Load texture
	spriteSheet = app->tex->Load("Assets/textures/herochar_spriteSheet.png");
	ball = app->tex->Load("Assets/textures/shoot.png");
	currentAnim = &idleRAnim;

	position.x = -1544;
	position.y = -1920;

	BcurrentAnim = &BallN;
	Bposition.x = position.x;
	Bposition.y = position.y - 15;

	return true;
}

// Called each loop iteration
bool Player::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Player::Update(float dt)
{
	if (Godmode == false)
	{
		//Gravity
		if (gravity == true)
		{
			position.y -= speedy;

			if (LookingR) currentAnim = &jumpRAnim;
			else currentAnim = &jumpLAnim;
		}

		//Idle	
		if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			|| (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			&& jump == false
			&& dead == false)
		{
			if (LookingR) currentAnim = &idleRAnim;
			else currentAnim = &idleLAnim;
		}

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
			&& dead == false)
		{
			position.x += speedx;

			if (gravity == false) currentAnim = &runLAnim;

			LookingR = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE 
			&& dead == false)
		{
			position.x -= speedx;

			if(gravity == false) currentAnim = &runRAnim;

			LookingR = true;
		}

		//Jump
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN
			&& jump == false
			&& dead == false)
		{
			jump = true;
			top = true;
			gravity = false;
			maxJump = position.y + 144;
		}
		if (top == true && position.y <= maxJump)
		{
			position.y += speedy;
			if (position.y == maxJump) top = false;
		}

		if (jump == true)
		{
			if (LookingR) currentAnim = &jumpRAnim;
			else currentAnim = &jumpLAnim;
		}

		//Die tester
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			dead = !dead;

			if (LookingR) currentAnim = &deadRAnim;
			else currentAnim = &deadLAnim;

			if (deadRAnim.FinishedAlready && dead == false 
				|| deadLAnim.FinishedAlready && dead == false)
			{
				deadRAnim.Reset();
				deadLAnim.Reset();
			}
		}

		if (CollisionFloorPlayer())
		{
			gravity = false;
			jump = false;
		}
		else if (top == false)
		{
			gravity = true;
		}

		if (CollisionPlayer() == 2 && LookingR == true) speedx = 0;
		else if (CollisionPlayer() == 3 && LookingR == false) speedx = 0;
		else speedx = 16;

		if (CollisionPlayer() == 1) {
			app->SaveGameRequest("GameFile.xml");

			if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) position = { -2057, -1921 };
			if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) position = { -4552, -1024 };
			if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) position = { -6472, -640 };

		}
	}

	//Godmode
	if (Godmode == true)
	{
		gravity = false;
		jump = false;

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		{
			position.x += speedx;

			LookingR = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		{
			position.x -= speedx;

			LookingR = true;
		}

		//Mov up
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
			position.y += speedx;

		//Mov down
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
			position.y -= speedx;

		if (LookingR) currentAnim = &jumpRAnim;
		else currentAnim = &jumpLAnim;
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		if (shoot == false && cooldown == 0)
		{
			shoot = true;
			if (LookingR) WasLookingR = true;
			else WasLookingR = false;
		}
	}

	if (shoot == true && cooldown >= 20) shoot = false;

	if (shoot == true)
	{
		BcurrentAnim = &BallS;

		if (WasLookingR) Bposition.x -= ballspeed;
		else Bposition.x += ballspeed;

		cooldown++;
	}
	else
	{
		BcurrentAnim = &BallN;
		Bposition.x = position.x;
		Bposition.y = position.y - 15;

		cooldown = 0;
	}


	currentAnim->Update();

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(spriteSheet, -position.x, -position.y, &rect);

	SDL_Rect rectB = BcurrentAnim->GetCurrentFrame();
	app->render->DrawTexture(ball, -Bposition.x, -Bposition.y, &rectB); //Probar solo 1 shoot
	

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}

int Player::CollisionPlayer()
{
	iPoint posMapPlayer[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posMapPlayer[i] = app->map->WorldToMap(-position.x + (int)pointsCollision[i][0], -position.y + (int)pointsCollision[i][1]);
		if (CheckCollision(posMapPlayer[i]) == 2) app->modcontrol->currentscene = 3;
		if (CheckCollision(posMapPlayer[i]) == 3) return 1;
	}
	if (CheckCollision(posMapPlayer[numnPoints - 1]) == 1) return 2;
	if (CheckCollision(posMapPlayer[numnPoints - 2]) == 1) return 3;

	return false;
}

bool Player::CollisionFloorPlayer()
{
	iPoint posFloorPlayer[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posFloorPlayer[i] = app->map->WorldToMap(-position.x + (int)pointsFloorCollision[i][0], -position.y + (int)pointsFloorCollision[i][1]);
		if (CheckCollision(posFloorPlayer[i]) == 1) return true;
	}

	return false;
}

int Player::CheckCollision(iPoint positionMapPlayer)
{
	if (app->map->data.layers.At(1)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 1;
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 2;
	if (app->map->data.layers.At(3)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 2;
	if (app->map->data.layers.At(4)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 3;

	return false;
}