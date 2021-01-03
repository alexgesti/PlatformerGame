#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Scene.h"
#include "Audio.h"
#include "ModuleController.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	//idle rigth animation
	idleRAnim.PushBack({ 0, 0, 64, 64 });
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

	//Shoot
	Ball.PushBack({ 0, 0, 0, 0 });
	Ball.PushBack({ 0, 0, 16, 16 });
}

// Destructor
Player::~Player()
{

}

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
	spriteSheet = app->tex->Load("Assets/Characters/Hero/herochar_spriteSheet.png");
	ball = app->tex->Load("Assets/Screens/Gameplay/shoot.png");

	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/jump.wav");
	deathFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/deathPlayer.wav");
	hitFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/hit.wav");
	shootFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/shoot.wav");

	currentAnim = &idleRAnim;

	position.x = -1544;
	position.y = -1920;

	BcurrentAnim = &Ball;
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
bool Player::Update(float dt, Map* map)
{
	dt *= 100;

	if (Godmode == false)
	{
		//Gravity
		if (gravity == true && dead == false)
		{
			position.y -= speedy * dt;

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
			position.x += speedx * dt;

			if (gravity == false) currentAnim = &runLAnim;

			LookingR = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
			&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE 
			&& dead == false)
		{
			position.x -= speedx * dt;

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
			app->audio->PlayFx(jumpFx);
		}
		if (top == true && position.y < maxJump)
		{
			position.y += speedy;
			if (position.y >= maxJump) top = false;
		}

		if (jump == true)
		{
			if (LookingR) currentAnim = &jumpRAnim;
			else currentAnim = &jumpLAnim;
		}

		//Die
		if (life <= 0)
		{
			dead = true;

			if (oncesound == false)
			{
				oncesound = true;
				app->audio->PlayFx(deathFx);
			}

			if (LookingR) currentAnim = &deadRAnim;
			else currentAnim = &deadLAnim;
		}

		if (CollisionFloorPlayer(map))
		{
			gravity = false;
			jump = false;
			int auxpos = position.y / 16;
			position.y = auxpos * 16;
		}
		else if (top == false)
		{
			gravity = true;
		}
		if(!CollisionFloorPlayer(map)) jump = true;

		if (CollisionPlayer(map) == 2 && LookingR == true)
		{
			speedx = 0;
			int auxpos = position.x / 8;
			position.x = auxpos * 8;
		}
		else if (CollisionPlayer(map) == 3 && LookingR == false)
		{
			speedx = 0;
			int auxpos = position.x / 8;
			position.x = auxpos * 8;
		}
		else speedx = 8;

		//F9
		/*
		if (CollisionPlayer() == 1) 
		{
			app->SaveGameRequest("save_game.xml");

			app->scene->CheckPointActive = true;

			if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) position = { -2057, -1921 };
			if (app->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) position = { -4552, -1024 };
			if (app->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN) position = { -6472, -640 };

		}
		else
		{
			app->scene->CheckPointActive = false;
			if (app->scene->counterCheckPointSound >= 1) app->scene->counterCheckPointSound = 0;
			app->scene->SoundOneTime = false;
			app->scene->pillar.Reset();
		}
		*/
	}

	//Godmode
	if (Godmode == true)
	{
		gravity = false;
		jump = false;
		top = false;
		speedx = 8;

		//Mov left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
		{
			position.x += speedx * dt;

			LookingR = false;
		}

		//Mov right
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
		{
			position.x -= speedx * dt;

			LookingR = true;
		}

		//Mov up
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
			position.y += speedx * dt;

		//Mov down
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
			position.y -= speedx * dt;

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
			app->audio->PlayFx(shootFx);
		}
	}

	if (shoot == true && cooldown >= 20) shoot = false;

	if (shoot == true)
	{
		Ball.GetSelectedFrame(1);

		if (WasLookingR) Bposition.x -= ballspeed;
		else Bposition.x += ballspeed;

		cooldown++;
	}
	else
	{
		Ball.GetSelectedFrame(2);
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

int Player::CollisionPlayer(Map* map)
{
	iPoint posMapPlayer[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posMapPlayer[i] = map->WorldToMap(-position.x + (int)pointsCollision[i][0], -position.y + (int)pointsCollision[i][1]);
		if (CheckCollision(posMapPlayer[i], map) == 2) life = 0;
		if (CheckCollision(posMapPlayer[i], map) == 3) return 1;
		if (CheckCollision(posMapPlayer[i], map) == 4) app->modcontrol->currentscene=4;
	}

	if (CheckCollision(posMapPlayer[numnPoints - 1], map) == 1) return 2;
	if (CheckCollision(posMapPlayer[numnPoints - 2], map) == 1) return 3;

	return false;
}

bool Player::CollisionFloorPlayer(Map* map)
{
	iPoint posFloorPlayer[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posFloorPlayer[i] = map->WorldToMap(-position.x + (int)pointsFloorCollision[i][0], -position.y + (int)pointsFloorCollision[i][1]);
		if (CheckCollision(posFloorPlayer[i], map) == 1) return true;
	}

	return false;
}

int Player::CheckCollision(iPoint positionMapPlayer, Map* map)
{
	if (map->data.layers.At(1)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 1;
	if (map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 4;
	if (map->data.layers.At(3)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 2;
	if (map->data.layers.At(4)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 3;

	return false;
}