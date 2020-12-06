#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "WalkingEnemy.h"
#include "Player.h"
#include "ModuleController.h"
#include "Pathfinding.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

WalkingEnemy::WalkingEnemy() : Module()
{
	name.Create("enemy");

	//run left animation
	runLAnim.PushBack({ 0, 128, 64, 64 });
	runLAnim.PushBack({ 64, 128, 64, 64 });
	runLAnim.PushBack({ 128, 128, 64, 64 });
	runLAnim.PushBack({ 192, 128, 64, 64 });
	runLAnim.PushBack({ 256, 128, 64, 64 });
	runLAnim.PushBack({ 320, 128, 64, 64 });
	runLAnim.PushBack({ 384, 128, 64, 64 });
	runLAnim.PushBack({ 448, 128, 64, 64 });
	runLAnim.loop = true;
	runLAnim.speed = 0.15f;

	//run rigth animation
	runRAnim.PushBack({ 448, 0, 64, 64 });
	runRAnim.PushBack({ 384, 0, 64, 64 });
	runRAnim.PushBack({ 320, 0, 64, 64 });
	runRAnim.PushBack({ 256, 0, 64, 64 });
	runRAnim.PushBack({ 192, 0, 64, 64 });
	runRAnim.PushBack({ 128, 0, 64, 64 });
	runRAnim.PushBack({ 64, 0, 64, 64 });
	runRAnim.PushBack({ 0, 0, 64, 64 });
	runRAnim.loop = true;
	runRAnim.speed = 0.15f;

	//dead rigth animation
	deadRAnim.PushBack({ 448, 64, 64, 64 });
	deadRAnim.PushBack({ 384, 64, 64, 64 });
	deadRAnim.PushBack({ 320, 64, 64, 64 });
	deadRAnim.PushBack({ 256, 64, 64, 64 });
	deadRAnim.PushBack({ 192, 64, 64, 64 });
	deadRAnim.PushBack({ 128, 64, 64, 64 });
	deadRAnim.loop = false;
	deadRAnim.speed = 0.12f;

	//dead left animation
	deadLAnim.PushBack({ 0, 192, 64, 64 });
	deadLAnim.PushBack({ 64, 192, 64, 64 });
	deadLAnim.PushBack({ 128, 192, 64, 64 });
	deadLAnim.PushBack({ 192, 192, 64, 64 });
	deadLAnim.PushBack({ 256, 192, 64, 64 });
	deadLAnim.PushBack({ 320, 192, 64, 64 });
	deadLAnim.loop = false;
	deadLAnim.speed = 0.12f;

	idleRAnim.PushBack({ 64, 0, 64, 64 });
	idleRAnim.PushBack({ 0, 0, 64, 64 });
	idleLAnim.loop = true;
	idleLAnim.speed = 0.05f;

	idleLAnim.PushBack({ 384, 128, 64, 64 });
	idleLAnim.PushBack({ 448, 128, 64, 64 });
	idleRAnim.loop = true;
	idleRAnim.speed = 0.05f;
}

// Destructor
WalkingEnemy::~WalkingEnemy()
{}

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
	spriteSheet = app->tex->Load("Assets/Characters/Enemies/WalkingEnemies/enemy_spritesheet.png");
	deathEnemyFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Enemies/deathEnemy.wav");

	currentAnim = &runRAnim;

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
	//Gravity
	if (gravity == true)
	{
		position.y -= speedy;
	}

	if (waslookingRight == true)
	{
		currentAnim = &idleRAnim;
	}
	if (waslookingRight == false)
	{
		currentAnim = &idleLAnim;
	}

	//Mov right
	if (app->player->position.y == position.y 
		&& app->player->position.x >= position.x 
		&& dead == false
		&& app->player->Godmode == false)
	{
		currentAnim = &runRAnim;
		position.x += speedx;
		waslookingRight = true;
	}

	//Mov left
	else if (app->player->position.y == position.y
		&& app->player->position.x <= position.x
		&& dead == false
		&& app->player->Godmode == false)
	{
		currentAnim = &runLAnim;
		position.x -= speedx;
		waslookingRight = false;
	}

	//Die
	if (dead)
	{
		if (waslookingRight) currentAnim = &deadRAnim;
		else currentAnim = &deadLAnim;

		if (oncesound == false) 
		{
			oncesound = true;
			app->audio->PlayFx(deathEnemyFx);
		}		

		if (deadRAnim.FinishedAlready || deadLAnim.FinishedAlready)
		{
			position.x = 0;
			position.y = 0;
			deadRAnim.Reset();
			deadLAnim.Reset();
			gravity = false;
			IsDead = true;
		}
	}

	if (CollisionFloorEnemy())
	{
		gravity = false;
	}
	else gravity = true;

	if (CheckCollisionRec(app->player->Bposition, position) && app->player->shoot == true && dead == false)
	{
		dead = true;
		app->player->shoot = false;
	}

	if (CheckCollisionRec(app->player->position, position) && hitingPlayer == false && dead == false && app->player->Godmode == false)
	{
		hitingPlayer = true;
		app->player->life--;
		app->audio->PlayFx(app->player->hitFx);
	}

	if (!CheckCollisionRec(app->player->position, position) && dead == false)
	{
		hitingPlayer = false;
	}

	//app->pathfinding->CreatePath(position, app->player->position);

	if (IsDead == false) currentAnim->Update();

	return true;
}

// Called each loop iteration
bool WalkingEnemy::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	if (IsDead == false) app->render->DrawTexture(spriteSheet, -position.x, -position.y, &rect);

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
	iPoint posMapEnemy[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posMapEnemy[i] = app->map->WorldToMap(-position.x + (int)pointsCollision[i][0], -position.y + (int)pointsCollision[i][1]);
	}
	if (CheckCollision(posMapEnemy[numnPoints - 1]) == 1) return 2;
	if (CheckCollision(posMapEnemy[numnPoints - 2]) == 1) return 3;

	return false;
}

bool WalkingEnemy::CollisionFloorEnemy()
{
	iPoint posFloorEnemy[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posFloorEnemy[i] = app->map->WorldToMap(-position.x + (int)pointsFloorCollision[i][0], -position.y + (int)pointsFloorCollision[i][1]);
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
