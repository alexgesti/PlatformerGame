#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Render.h"
#include "Window.h"
#include "Player.h"
#include "Scene.h"
#include "Audio.h"
#include "ModuleController.h"
#include "ScenePause.h"
#include "FadeController.h"

#include "Defs.h"
#include "Log.h"

Player::Player() : Module()
{
	name.Create("player");

	//idle rigth animation
	idleranim.PushBack({ 0, 0, 64, 64 });
	idleranim.PushBack({ 64, 0, 64, 64 });
	idleranim.PushBack({ 128, 0, 64, 64 });
	idleranim.PushBack({ 192, 0, 64, 64 });
	idleranim.loop = true;
	idleranim.speed = 0.15f;

	//jump rigth animation
	jumpranim.PushBack({ 0, 64, 64, 64 });
	jumpranim.PushBack({ 64, 64, 64, 64 });
	jumpranim.PushBack({ 128, 64, 64, 64 });
	jumpranim.loop = true;
	jumpranim.speed = 0.15f;

	//run rigth animation
	runranim.PushBack({ 0, 128, 64, 64 });
	runranim.PushBack({ 64, 128, 64, 64 });
	runranim.PushBack({ 128, 128, 64, 64 });
	runranim.PushBack({ 192, 128, 64, 64 });
	runranim.PushBack({ 256, 128, 64, 64 });
	runranim.PushBack({ 320, 128, 64, 64 });
	runranim.loop = true;
	runranim.speed = 0.15f;

	//dead rigth animation
	deadranim.PushBack({ 0, 192, 64, 64 });
	deadranim.PushBack({ 64, 192, 64, 64 });
	deadranim.PushBack({ 128, 192, 64, 64 });
	deadranim.PushBack({ 192, 192, 64, 64 });
	deadranim.PushBack({ 256, 192, 64, 64 });
	deadranim.PushBack({ 320, 192, 64, 64 });
	deadranim.PushBack({ 384, 192, 64, 64 });
	deadranim.PushBack({ 448, 192, 64, 64 });
	deadranim.PushBack({ 512, 192, 64, 64 });
	deadranim.loop = false;
	deadranim.speed = 0.15f;

	//idle left animation
	idlelanim.PushBack({ 0, 256, 64, 64 });
	idlelanim.PushBack({ 64, 256, 64, 64 });
	idlelanim.PushBack({ 128, 256, 64, 64 });
	idlelanim.PushBack({ 192, 256, 64, 64 });
	idlelanim.loop = true;
	idlelanim.speed = 0.15f;

	//jump left animation
	jumplanim.PushBack({ 0, 320, 64, 64 });
	jumplanim.PushBack({ 64, 320, 64, 64 });
	jumplanim.PushBack({ 128, 320, 64, 64 });
	jumplanim.loop = true;
	jumplanim.speed = 0.15f;

	//run left animation
	runlanim.PushBack({ 0, 384, 64, 64 });
	runlanim.PushBack({ 64, 384, 64, 64 });
	runlanim.PushBack({ 128, 384, 64, 64 });
	runlanim.PushBack({ 192, 384, 64, 64 });
	runlanim.PushBack({ 256, 384, 64, 64 });
	runlanim.PushBack({ 320, 384, 64, 64 });
	runlanim.loop = true;
	runlanim.speed = 0.15f;

	//dead left animation
	deadlanim.PushBack({ 0, 448, 64, 64 });
	deadlanim.PushBack({ 64, 448, 64, 64 });
	deadlanim.PushBack({ 128, 448, 64, 64 });
	deadlanim.PushBack({ 192, 448, 64, 64 });
	deadlanim.PushBack({ 256, 448, 64, 64 });
	deadlanim.PushBack({ 320, 448, 64, 64 });
	deadlanim.PushBack({ 384, 448, 64, 64 });
	deadlanim.PushBack({ 448, 448, 64, 64 });
	deadlanim.PushBack({ 512, 448, 64, 64 });
	deadlanim.loop = false;
	deadlanim.speed = 0.15f;

	//Shoot
	ball.PushBack({ 0, 0, 0, 0 });
	ball.PushBack({ 0, 0, 16, 16 });

	//Collisions
	collision.PushBack({96, 0, 64, 64});
	collisionb.PushBack({210, 0, 16, 16});

	currentanimcoll = &collision;
	currentanimcollb = &collisionb;
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
	spritesheet = app->tex->Load("Assets/Characters/Hero/herochar_spriteSheet.png");
	balltext = app->tex->Load("Assets/Screens/Gameplay/shoot.png");
	collisiontext = app->tex->Load("Assets/Screens/Gameplay/collision_entities.png");

	jumpFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/jump.wav");
	deathFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/deathPlayer.wav");
	hitFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/hit.wav");
	shootFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Hero/shoot.wav");

	currentanim = &idleranim;

	position.x = -1544;
	position.y = -1920;

	bcurrentanim = &ball;
	bposition.x = position.x;
	bposition.y = position.y - 15;

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
	if (app->scenepause->active == false && app->fade->canfade == false)
	{
		dt *= 100;

		if (godmode == false)
		{
			//gravity
			if (gravity == true && dead == false)
			{
				position.y -= speedy * dt;

				if (lookingr) currentanim = &jumpranim;
				else currentanim = &jumplanim;
			}

			if (jump == true && top == false) gravity = true;

			//Idle	
			if ((app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
				&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
				|| (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
					&& app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				&& jump == false
				&& dead == false)
			{
				if (lookingr) currentanim = &idleranim;
				else currentanim = &idlelanim;
			}

			//Mov left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT
				&& app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE
				&& dead == false)
			{
				position.x += speedx * dt;

				if (gravity == false) currentanim = &runlanim;

				lookingr = false;
			}

			//Mov right
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT
				&& app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE
				&& dead == false)
			{
				position.x -= speedx * dt;

				if (gravity == false) currentanim = &runranim;

				lookingr = true;
			}

			//Jump
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN
				&& jump == false
				&& dead == false)
			{
				jump = true;
				top = true;
				gravity = false;
				maxjump = position.y + 144;
				app->audio->PlayFx(jumpFx);
			}
			if (top == true && position.y < maxjump)
			{
				position.y += speedy;
				if (position.y >= maxjump) top = false;
			}
			if (!CollisionFloorPlayer()) jump = true;

			if (jump == true)
			{
				if (lookingr) currentanim = &jumpranim;
				else currentanim = &jumplanim;
			}

			//die
			if (life <= 0)
			{
				dead = true;

				if (oncesound == false)
				{
					oncesound = true;
					app->audio->PlayFx(deathFx);
				}

				if (lookingr) currentanim = &deadranim;
				else currentanim = &deadlanim;
			}

			if (CollisionFloorPlayer())
			{
				gravity = false;
				jump = false;
				int auxposy = position.y / 16;
				position.y = auxposy * 16;
			}

			if (CollisionPlayer() == 2 && lookingr == true)
			{
				speedx = 0;
				int auxposx = position.x / 8;
				position.x = auxposx * 8;
			}
			else if (CollisionPlayer() == 3 && lookingr == false)
			{
				speedx = 0;
				int auxposx = position.x / 8;
				position.x = auxposx * 8;
			}
			else speedx = 8;

			if (CollisionPlayer() == 1 ||
				CollisionPlayer() == 4 ||
				CollisionPlayer() == 5)
			{
				app->SaveGameRequest("save_game.xml");

				app->scene->checkpointactive = true;
			}
			else
			{
				app->scene->checkpointactive = false;
				if (app->scene->countercheckpointsound >= 1) app->scene->countercheckpointsound = 0;
				app->scene->soundonetime = false;
				app->scene->pillar.Reset();
			}
		}

		//Godmode
		if (godmode == true)
		{
			gravity = false;
			jump = false;
			top = false;
			speedx = 8;

			//Mov left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
			{
				position.x += speedx * dt;

				lookingr = false;
			}

			//Mov right
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
			{
				position.x -= speedx * dt;

				lookingr = true;
			}

			//Mov up
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
				position.y += speedx * dt;

			//Mov down
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
				position.y -= speedx * dt;

			if (lookingr) currentanim = &jumpranim;
			else currentanim = &jumplanim;
		}

		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		{
			if (shoot == false && cooldown == 0)
			{
				shoot = true;
				if (lookingr) waslookingr = true;
				else waslookingr = false;
				app->audio->PlayFx(shootFx);
			}
		}

		if (shoot == true && cooldown >= 20) shoot = false;

		if (shoot == true)
		{
			ball.GetSelectedFrame(1);

			if (waslookingr) bposition.x -= ballspeed;
			else bposition.x += ballspeed;

			cooldown++;
		}
		else
		{
			ball.GetSelectedFrame(2);
			bposition.x = position.x;
			bposition.y = position.y - 15;

			cooldown = 0;
		}


		currentanim->Update();
	}

	return true;
}

// Called each loop iteration
bool Player::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentanim->GetCurrentFrame();
	app->render->DrawTexture(spritesheet, -position.x, -position.y, &rect);

	SDL_Rect rectB = bcurrentanim->GetCurrentFrame();
	app->render->DrawTexture(balltext, -bposition.x, -bposition.y, &rectB); //Probar solo 1 shoot
	
	if (app->modcontrol->showcollider)
	{
		SDL_Rect rectCol = currentanimcoll->GetCurrentFrame();
		app->render->DrawTexture(collisiontext, -position.x, -position.y, &rectCol);

		SDL_Rect rectColB = currentanimcollb->GetCurrentFrame();
		app->render->DrawTexture(collisiontext, -bposition.x, -bposition.y, &rectColB);
	}

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
	iPoint posMapPlayer[numnpoints];

	for (int i = 0; i < numnpoints; i++)
	{
		posMapPlayer[i] = app->map->WorldToMap(-position.x + (int)pointscollision[i][0], -position.y + (int)pointscollision[i][1]);
		if (CheckCollision(posMapPlayer[i]) == 2) life = 0;
		if (CheckCollision(posMapPlayer[i]) == 3) return 1; // Checkpoint 1
		if (CheckCollision(posMapPlayer[i]) == 5) return 4; // Checkpoint 2
		if (CheckCollision(posMapPlayer[i]) == 6) return 5; // Checkpoint 3
		if (CheckCollision(posMapPlayer[i]) == 4)
		{
			app->fade->canfade = true;
			app->fade->startinblack = false;
			app->fade->hewin = true;
		}
	}

	if (CheckCollision(posMapPlayer[numnpoints - 1]) == 1) return 2;
	if (CheckCollision(posMapPlayer[numnpoints - 2]) == 1) return 3;

	return false;
}

bool Player::CollisionFloorPlayer()
{
	iPoint posFloorPlayer[numnpoints];

	for (int i = 0; i < numnpoints; i++)
	{
		posFloorPlayer[i] = app->map->WorldToMap(-position.x + (int)pointsfloorcollision[i][0], -position.y + (int)pointsfloorcollision[i][1]);
		if (CheckCollision(posFloorPlayer[i]) == 1) return true;
	}

	return false;
}

int Player::CheckCollision(iPoint positionMapPlayer)
{
	if (app->map->data.layers.At(1)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 1;
	if (app->map->data.layers.At(2)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 4;
	if (app->map->data.layers.At(3)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 2;
	if (app->map->data.layers.At(4)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 3; // CheckPoint 1
	if (app->map->data.layers.At(5)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 5; // CheckPoint 2
	if (app->map->data.layers.At(6)->data->Get(positionMapPlayer.x, positionMapPlayer.y) != 0) return 6; // CheckPoint 3

	return false;
}