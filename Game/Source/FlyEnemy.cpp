#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Render.h"
#include "Window.h"
#include "FlyEnemy.h"
#include "ModuleController.h"
#include "Pathfinding.h"
#include "Player.h"
#include "Audio.h"
#include "ScenePause.h"
#include "GameplayHUD.h"

#include "Defs.h"
#include "Log.h"

FlyEnemy::FlyEnemy() : Module()
{
	name.Create("Flyenemy");

	//run left animation
	runlanim.PushBack({ 0, 96, 32, 32 });
	runlanim.PushBack({ 32, 96, 32, 32 });
	runlanim.PushBack({ 64, 96, 32, 32 });
	runlanim.loop = true;
	runlanim.speed = 0.15f;

	//run rigth animation
	runranim.PushBack({ 128, 32, 32, 32 });
	runranim.PushBack({ 96, 32, 32, 32 });
	runranim.PushBack({ 64, 32, 32, 32 });
	runranim.loop = true;
	runranim.speed = 0.15f;

	//dead rigth animation
	deadranim.PushBack({ 128, 0, 32, 32 });
	deadranim.PushBack({ 96, 0, 32, 32 });
	deadranim.PushBack({ 64, 0, 32, 32 });
	deadranim.PushBack({ 32, 0, 32, 32 });
	deadranim.PushBack({ 0, 0, 32, 32 });
	deadranim.loop = false;
	deadranim.speed = 0.12f;

	//dead left animation
	deadlanim.PushBack({ 0, 64, 32, 32 });
	deadlanim.PushBack({ 32, 64, 32, 32 });
	deadlanim.PushBack({ 64, 64, 32, 32 });
	deadlanim.PushBack({ 96, 64, 32, 32 });
	deadlanim.PushBack({ 128, 64, 32, 32 });
	deadlanim.loop = false;
	deadlanim.speed = 0.12f;

	//collisions
	collision.PushBack({ 0, 0, 32, 32 });

	currentanimcoll = &collision;
}

// Destructor
FlyEnemy::~FlyEnemy()
{

}

// Called before render is available
bool FlyEnemy::Awake()
{
	LOG("Loading FlyingEnemy");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool FlyEnemy::Start()
{
	//Load texture
	spritesheet = app->tex->Load("Assets/Characters/Enemies/FlyingEnemies/flyenemy_spritesheet.png");
	collisiontex = app->tex->Load("Assets/Screens/Gameplay/collision_entities.png");
	deathenemyfx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Enemies/deathEnemy.wav");

	currentanim = &runranim;

	position.x = -4928;
	position.y = -1024;

	return true;
}

// Called each loop iteration
bool FlyEnemy::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool FlyEnemy::Update(float dt)
{
	if (app->scenepause->active == false)
	{
		//Mov left
		if (((app->player->position.y <= position.y + detectdistance
			&& app->player->position.y > position.y)
			|| (app->player->position.y >= position.y - detectdistance
				&& app->player->position.y < position.y)
			|| app->player->position.y == position.y)
			&& app->player->position.x >= position.x
			&& dead == false
			&& app->player->godmode == false)
		{
			currentanim = &runlanim;
			position.x += speedx;
			waslookingright = false;

			if (app->player->position.y == position.y);
			if (app->player->position.y <= position.y + detectdistance && app->player->position.y > position.y) position.y += speedy;
			if (app->player->position.y >= position.y - detectdistance && app->player->position.y < position.y) position.y -= speedy;
		}

		//Mov right
		if (((app->player->position.y <= position.y + detectdistance
			&& app->player->position.y > position.y)
			|| (app->player->position.y >= position.y - detectdistance
				&& app->player->position.y < position.y)
			|| app->player->position.y == position.y)
			&& app->player->position.x <= position.x
			&& dead == false
			&& app->player->godmode == false)
		{
			currentanim = &runranim;
			position.x -= speedx;
			waslookingright = true;

			if (app->player->position.y == position.y);
			if (app->player->position.y <= position.y + detectdistance && app->player->position.y > position.y) position.y += speedy;
			if (app->player->position.y >= position.y - detectdistance && app->player->position.y < position.y) position.y -= speedy;
		}

		//Die
		if (dead)
		{
			if (waslookingright) currentanim = &deadranim;
			else currentanim = &deadlanim;

		if (oncesound == false)
		{
			oncesound = true;
			app->audio->PlayFx(deathenemyfx);
			app->gamehud->points[2]++;
		}

			if (deadranim.FinishedAlready || deadlanim.FinishedAlready)
			{
				position.x = 0;
				position.y = 0;
				deadranim.Reset();
				deadlanim.Reset();
				isdead = true;
			}
		}

		if (CheckCollisionRec(app->player->bposition, position) && app->player->shoot == true && dead == false)
		{
			dead = true;
			app->player->shoot = false;
		}

		if (CheckCollisionRec(app->player->position, position) && hitingplayer == false && dead == false && app->player->godmode == false)
		{
			hitingplayer = true;
			app->player->life--;
			app->audio->PlayFx(app->player->hitFx);
		}

		if (!CheckCollisionRec(app->player->position, position) && dead == false)
		{
			hitingplayer = false;
		}

		//app->pathfinding->CreatePath(position, app->player->position);

		if (isdead == false) currentanim->Update();
	}

	return true;
}

// Called each loop iteration
bool FlyEnemy::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentanim->GetCurrentFrame();
	if (isdead == false) app->render->DrawTexture(spritesheet, -position.x, -position.y, &rect);

	if (app->modcontrol->showcollider)
	{
		SDL_Rect rectCol = currentanimcoll->GetCurrentFrame();
		app->render->DrawTexture(collisiontex, -position.x, -position.y, &rectCol);
	}

	return ret;
}

// Called before quitting
bool FlyEnemy::CleanUp()
{
	LOG("Freeing flyingenemy");

	return true;
}

bool FlyEnemy::CheckCollisionRec(iPoint positionMapBullet, iPoint positionMapEnemy)
{
	if ((positionMapBullet.x < (positionMapEnemy.x + 52)) && ((positionMapBullet.x + 52) > positionMapEnemy.x) &&
		(positionMapBullet.y < (positionMapEnemy.y + 47)) && ((positionMapBullet.y + 47) > positionMapEnemy.y)) return 1;


	return false;
}