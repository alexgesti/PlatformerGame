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
	idleRAnim.PushBack({0, 0, 64, 64});
	idleRAnim.PushBack({ 64, 0, 64, 64 });
	idleRAnim.PushBack({ 128, 0, 64, 64 });
	idleRAnim.PushBack({ 192, 0, 64, 64 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.15f;

	//jump rigth animation
	jumpRAnim.PushBack({ 0, 64, 64, 64 });
	jumpRAnim.PushBack({ 64, 64, 64, 64 });
	jumpRAnim.PushBack({ 128, 64, 64, 64 });
	jumpRAnim.loop = true;
	jumpRAnim.speed = 0.15f;

	//run rigth animation
	runRAnim.PushBack({ 0, 128, 64, 64 });
	runRAnim.PushBack({ 64, 128, 64, 64 });
	runRAnim.PushBack({ 128, 128, 64, 64 });
	runRAnim.PushBack({ 192, 128, 64, 64 });
	runRAnim.PushBack({ 256, 128, 64, 64 });
	runRAnim.PushBack({ 320, 128, 64, 64 });
	runRAnim.loop = true;
	runRAnim.speed = 0.15f;

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
	deadRAnim.speed = 0.15f;

	//idle left animation
	idleLAnim.PushBack({ 0, 256, 64, 64 });
	idleLAnim.PushBack({ 64, 256, 64, 64 });
	idleLAnim.PushBack({ 128, 256, 64, 64 });
	idleLAnim.PushBack({ 192, 256, 64, 64 });
	idleLAnim.loop = true;
	idleLAnim.speed = 0.15f;

	//jump left animation
	jumpLAnim.PushBack({ 0, 320, 64, 64 });
	jumpLAnim.PushBack({ 64, 320, 64, 64 });
	jumpLAnim.PushBack({ 128, 320, 64, 64 });
	jumpLAnim.loop = true;
	jumpLAnim.speed = 0.15f;

	//run left animation
	runLAnim.PushBack({ 0, 384, 64, 64 });
	runLAnim.PushBack({ 64, 384, 64, 64 });
	runLAnim.PushBack({ 128, 384, 64, 64 });
	runLAnim.PushBack({ 192, 384, 64, 64 });
	runLAnim.PushBack({ 256, 384, 64, 64 });
	runLAnim.PushBack({ 320, 384, 64, 64 });
	runLAnim.loop = true;
	runLAnim.speed = 0.15f;

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
	deadLAnim.speed = 0.15f;
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
	currentAnim = &idleRAnim;

	position.x = 0;
	position.y = -1260 - (app->render->camera.h/2);

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
		gravity = true;

		iPoint BeforePos = position;

		//Gravity
		if (gravity == true)
			position.y -= speedy;

		//Idle	
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
			&& jump == false
			&& dead == false)
		{
			if (waslookingRight) currentAnim = &idleRAnim;
			else currentAnim = &idleLAnim;
		}

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
			&& jump == false
			&& dead == false)
		{
			position.x += speedx;

			currentAnim = &runLAnim;

			waslookingRight = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE 
			&& jump == false
			&& dead == false)
		{
			position.x -= speedx;

			currentAnim = &runRAnim;

			waslookingRight = true;
		}

		//Jump
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN 
			&& gravity == false 
			&& dead == false)
		{
			jump = true;
			maxJump = position.y;
		}
		if (jump == true && app->render->camera.y < maxJump + 40)
		{
			position.y -= 2 * speedx;
		}

		if (jump)
		{
			if (waslookingRight) currentAnim = &jumpRAnim;
			else currentAnim = &jumpLAnim;
		}

		//Die tester
		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			dead = !dead;

			if (waslookingRight) currentAnim = &deadRAnim;
			else currentAnim = &deadLAnim;

			if (deadRAnim.FinishedAlready && dead == false 
				|| deadLAnim.FinishedAlready && dead == false)
			{
				deadRAnim.Reset();
				deadLAnim.Reset();
			}
		}

		//if (CollisionPlayer({ position.x, position.y - speedy})) position = BeforePos;
	}

	//Godmode
	if (Godmode == true)
	{
		gravity = false;

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		{
			position.x += speedx;

			waslookingRight = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		{
			position.x -= speedx;

			waslookingRight = true;
		}

		//Mov up
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
			position.y += speedx;

		//Mov down
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
			position.y -= speedx;

		if (waslookingRight) currentAnim = &jumpRAnim;
		else currentAnim = &jumpLAnim;
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

	return ret;
}

// Called before quitting
bool Player::CleanUp()
{
	LOG("Freeing player");

	return true;
}

/*
bool Player::CollisionPlayer(iPoint positionMapPlayer)
{
	iPoint posMapPlayer;
	int y = (int)positionMapPlayer.y * -1;
	int x = (int)positionMapPlayer.x * -1;

	for (int i = 0; i < numnPoints; i++)
	{
		posMapPlayer = app->map->WorldToMap(x + (int)pointsCollision[i][0], y + (int)pointsCollision[i][1]);
		if (CheckCollision(posMapPlayer)) return true;
	}

	return false;
}

bool Player::CheckCollision(iPoint positionMapPlayer)
{
	if (app->map->data.layers.At(3)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return true;
	if (app->map->data.layers.At(4)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) app->modcontrol->currentscene = 3;

	return false;
}
*/