#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Map.h"
#include "Render.h"
#include "Window.h"
#include "FlyEnemy.h"
#include "ModuleController.h"
#include "Pathfinding.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

FlyEnemy::FlyEnemy() : Entity(EntityType::FENEMY)
{
	//run left animation
	runLAnim.PushBack({ 0, 96, 32, 32 });
	runLAnim.PushBack({ 32, 96, 32, 32 });
	runLAnim.PushBack({ 64, 96, 32, 32 });
	runLAnim.loop = true;
	runLAnim.speed = 0.15f;

	//run rigth animation
	runRAnim.PushBack({ 128, 32, 32, 32 });
	runRAnim.PushBack({ 96, 32, 32, 32 });
	runRAnim.PushBack({ 64, 32, 32, 32 });
	runRAnim.loop = true;
	runRAnim.speed = 0.15f;

	//dead rigth animation
	deadRAnim.PushBack({ 128, 0, 32, 32 });
	deadRAnim.PushBack({ 96, 0, 32, 32 });
	deadRAnim.PushBack({ 64, 0, 32, 32 });
	deadRAnim.PushBack({ 32, 0, 32, 32 });
	deadRAnim.PushBack({ 0, 0, 32, 32 });
	deadRAnim.loop = false;
	deadRAnim.speed = 0.12f;

	//dead left animation
	deadLAnim.PushBack({ 0, 64, 32, 32 });
	deadLAnim.PushBack({ 32, 64, 32, 32 });
	deadLAnim.PushBack({ 64, 64, 32, 32 });
	deadLAnim.PushBack({ 96, 64, 32, 32 });
	deadLAnim.PushBack({ 128, 64, 32, 32 });
	deadLAnim.loop = false;
	deadLAnim.speed = 0.12f;
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
	spriteSheet = app->tex->Load("Assets/Characters/Enemies/FlyingEnemies/flyenemy_spritesheet.png");
	deathEnemyFx = app->audio->LoadFx("Assets/Audio/Fx/Characters/Enemies/deathEnemy.wav");

	currentAnim = &runRAnim;

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
bool FlyEnemy::Update(float dt, Player* player)
{
	//Mov left
	if (((player->position.y <= position.y + detectdistance
		&& player->position.y > position.y)
		|| (player->position.y >= position.y - detectdistance
		&& player->position.y < position.y)
		|| player->position.y == position.y)
		&& player->position.x >= position.x
		&& dead == false
		&& player->Godmode == false)
	{
		currentAnim = &runLAnim;
		position.x += speedx;
		waslookingRight = false;

		if (player->position.y == position.y);
		if (player->position.y <= position.y + detectdistance && player->position.y > position.y) position.y += speedy;
		if (player->position.y >= position.y - detectdistance && player->position.y < position.y) position.y -= speedy;
	}

	//Mov right
	if (((player->position.y <= position.y + detectdistance
		&& player->position.y > position.y)
		|| (player->position.y >= position.y - detectdistance
			&& player->position.y < position.y)
		|| player->position.y == position.y)
		&& player->position.x <= position.x
		&& dead == false
		&& player->Godmode == false)
	{
		currentAnim = &runRAnim;
		position.x -= speedx;
		waslookingRight = true;

		if (player->position.y == position.y);
		if (player->position.y <= position.y + detectdistance && player->position.y > position.y) position.y += speedy;
		if (player->position.y >= position.y - detectdistance && player->position.y < position.y) position.y -= speedy;
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
			IsDead = true;
		}
	}

	if (CheckCollisionRec(player->Bposition, position) && player->shoot == true && dead == false)
	{
		dead = true;
		player->shoot = false;
	}

	if (CheckCollisionRec(player->position, position) && hitingPlayer == false && dead == false && player->Godmode == false)
	{
		hitingPlayer = true;
		player->life--;
		app->audio->PlayFx(player->hitFx);
	}

	if (!CheckCollisionRec(player->position, position) && dead == false)
	{
		hitingPlayer = false;
	}

	//app->pathfinding->CreatePath(position, app->player->position);

	if (IsDead == false) currentAnim->Update();

	return true;
}

// Called each loop iteration
bool FlyEnemy::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	if (IsDead == false) app->render->DrawTexture(spriteSheet, -position.x, -position.y, &rect);

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