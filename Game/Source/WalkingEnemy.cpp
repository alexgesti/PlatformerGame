#include "App.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Render.h"
#include "Window.h"
#include "WalkingEnemy.h"
#include "Player.h"
#include "ModuleController.h"
#include "Pathfinding.h"
#include "Audio.h"
#include "GameplayHUD.h"
#include "ScenePause.h"

#include "Defs.h"
#include "Log.h"

WalkingEnemy::WalkingEnemy() : Module()
{
	name.Create("enemy");

	//run left animation
	runlanim.PushBack({ 0, 128, 64, 64 });
	runlanim.PushBack({ 64, 128, 64, 64 });
	runlanim.PushBack({ 128, 128, 64, 64 });
	runlanim.PushBack({ 192, 128, 64, 64 });
	runlanim.PushBack({ 256, 128, 64, 64 });
	runlanim.PushBack({ 320, 128, 64, 64 });
	runlanim.PushBack({ 384, 128, 64, 64 });
	runlanim.PushBack({ 448, 128, 64, 64 });
	runlanim.loop = true;
	runlanim.speed = 0.15f;

	//run rigth animation
	runranim.PushBack({ 448, 0, 64, 64 });
	runranim.PushBack({ 384, 0, 64, 64 });
	runranim.PushBack({ 320, 0, 64, 64 });
	runranim.PushBack({ 256, 0, 64, 64 });
	runranim.PushBack({ 192, 0, 64, 64 });
	runranim.PushBack({ 128, 0, 64, 64 });
	runranim.PushBack({ 64, 0, 64, 64 });
	runranim.PushBack({ 0, 0, 64, 64 });
	runranim.loop = true;
	runranim.speed = 0.15f;

	//dead rigth animation
	deadranim.PushBack({ 448, 64, 64, 64 });
	deadranim.PushBack({ 384, 64, 64, 64 });
	deadranim.PushBack({ 320, 64, 64, 64 });
	deadranim.PushBack({ 256, 64, 64, 64 });
	deadranim.PushBack({ 192, 64, 64, 64 });
	deadranim.PushBack({ 128, 64, 64, 64 });
	deadranim.loop = false;
	deadranim.speed = 0.12f;

	//dead left animation
	deadlanim.PushBack({ 0, 192, 64, 64 });
	deadlanim.PushBack({ 64, 192, 64, 64 });
	deadlanim.PushBack({ 128, 192, 64, 64 });
	deadlanim.PushBack({ 192, 192, 64, 64 });
	deadlanim.PushBack({ 256, 192, 64, 64 });
	deadlanim.PushBack({ 320, 192, 64, 64 });
	deadlanim.loop = false;
	deadlanim.speed = 0.12f;

	idleranim.PushBack({ 64, 0, 64, 64 });
	idleranim.PushBack({ 0, 0, 64, 64 });
	idlelanim.loop = true;
	idlelanim.speed = 0.05f;

	idlelanim.PushBack({ 384, 128, 64, 64 });
	idlelanim.PushBack({ 448, 128, 64, 64 });
	idleranim.loop = true;
	idleranim.speed = 0.05f;

	//collisions
	collision.PushBack({ 32, 0, 64, 64 });

	currentanimcoll = &collision;
}

// Destructor
WalkingEnemy::~WalkingEnemy()
{

}

// Called before render is available
bool WalkingEnemy::Awake()
{
	LOG("Loading WalkingEnemy");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool WalkingEnemy::Start()
{
	//Load texture
	spritesheet = app->tex->Load("Assets/Characters/Enemies/WalkingEnemies/enemy_spritesheet.png");
	deathenemyfx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Enemies/deathEnemy.wav");
	collisiontext = app->tex->Load("Assets/Screens/Gameplay/collision_entities.png");

	currentanim = &runranim;

	position.x = -3456;
	position.y = -1664;

	return true;
}

// Called each loop iteration
bool WalkingEnemy::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool WalkingEnemy::Update(float dt)
{
	if (app->scenepause->active == false)
	{
		//gravity
		if (gravity == true)
		{
			position.y -= speedy;
		}

		if (waslookingright == true)
		{
			currentanim = &idleranim;
		}
		if (waslookingright == false)
		{
			currentanim = &idlelanim;
		}

		//mov right
		if (app->player->position.y == position.y
			&& app->player->position.x >= position.x
			&& dead == false
			&& app->player->godmode == false)
		{
			currentanim = &runranim;
			position.x += speedx;
			waslookingright = true;
		}

		//mov left
		else if (app->player->position.y == position.y
			&& app->player->position.x <= position.x
			&& dead == false
			&& app->player->godmode == false)
		{
			currentanim = &runlanim;
			position.x -= speedx;
			waslookingright = false;
		}

		//die
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
				gravity = false;
				isdead = true;
			}
		}

		if (CollisionFloorEnemy())
		{
			gravity = false;
		}
		else gravity = true;

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
bool WalkingEnemy::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentanim->GetCurrentFrame();
	if (isdead == false) app->render->DrawTexture(spritesheet, -position.x, -position.y, &rect);

	if (app->modcontrol->showcollider)
	{
		SDL_Rect rectCol = currentanimcoll->GetCurrentFrame();
		app->render->DrawTexture(collisiontext, -position.x, -position.y, &rectCol);
	}

	return ret;
}

// Called before quitting
bool WalkingEnemy::CleanUp()
{
	LOG("Freeing walkingenemy");

	return true;
}

int WalkingEnemy::CollisionEnemy()
{
	iPoint posMapEnemy[numnpoints];

	for (int i = 0; i < numnpoints; i++)
	{
		posMapEnemy[i] = app->map->WorldToMap(-position.x + (int)pointscollision[i][0], -position.y + (int)pointscollision[i][1]);
	}
	if (CheckCollision(posMapEnemy[numnpoints - 1]) == 1) return 2;
	if (CheckCollision(posMapEnemy[numnpoints - 2]) == 1) return 3;

	return false;
}

bool WalkingEnemy::CollisionFloorEnemy()
{
	iPoint posFloorEnemy[numnpoints];

	for (int i = 0; i < numnpoints; i++)
	{
		posFloorEnemy[i] = app->map->WorldToMap(-position.x + (int)pointsfloorcollision[i][0], -position.y + (int)pointsfloorcollision[i][1]);
		if (CheckCollision(posFloorEnemy[i]) == 1) return true;
	}

	return false;
}

int WalkingEnemy::CheckCollision(iPoint positionMapEnemy)
{
	if (app->map->data.layers.At(1)->data->Get(positionMapEnemy.x, positionMapEnemy.y) != 0) return 1;

	return false;
}

bool WalkingEnemy::CheckCollisionRec(iPoint positionMapBullet, iPoint positionMapEnemy)
{
	if ((positionMapBullet.x < (positionMapEnemy.x + 52)) && ((positionMapBullet.x + 52) > positionMapEnemy.x) &&
		(positionMapBullet.y < (positionMapEnemy.y + 64)) && ((positionMapBullet.y + 64) > positionMapEnemy.y)) return true;


	return false;
}
