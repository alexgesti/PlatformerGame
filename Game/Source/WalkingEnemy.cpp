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
	runLAnim.speed = 0.5f;

	//fall left animation
	fallLAnim.PushBack({ 64, 128, 64, 64 });
	fallLAnim.speed = 0.5f;

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
	runRAnim.speed = 0.5f;

	//fall rigth animation
	fallRAnim.PushBack({ 384, 0, 64, 64 });
	fallRAnim.speed = 0.5f;

	//dead rigth animation
	deadRAnim.PushBack({ 448, 64, 64, 64 });
	deadRAnim.PushBack({ 384, 64, 64, 64 });
	deadRAnim.PushBack({ 320, 64, 64, 64 });
	deadRAnim.PushBack({ 256, 64, 64, 64 });
	deadRAnim.PushBack({ 192, 64, 64, 64 });
	deadRAnim.PushBack({ 128, 64, 64, 64 });
	deadRAnim.loop = false;
	deadRAnim.speed = 0.5f;

	//dead left animation
	deadLAnim.PushBack({ 0, 192, 64, 64 });
	deadLAnim.PushBack({ 64, 192, 64, 64 });
	deadLAnim.PushBack({ 128, 192, 64, 64 });
	deadLAnim.PushBack({ 192, 192, 64, 64 });
	deadLAnim.PushBack({ 256, 192, 64, 64 });
	deadLAnim.PushBack({ 320, 192, 64, 64 });
	deadLAnim.loop = false;
	deadLAnim.speed = 0.5f;
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
	spriteSheet = app->tex->Load("Assets/textures/enemy_spritesheet.png");
	currentAnim = &runRAnim;

	position.x = -1470;
	position.y = -1920;

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

		if (waslookingRight) currentAnim = &fallRAnim;
		else currentAnim = &fallLAnim;
	}

	//Mov left
	if (app->input->GetKey(SDL_SCANCODE_5) == KEY_DOWN)
		/*&& fall == false
		&& dead == false)*/
	{


		currentAnim = &runLAnim;

		waslookingRight = false;
	}

	//Mov right
	if (app->input->GetKey(SDL_SCANCODE_6) == KEY_DOWN)
		/*&& fall == false
		&& dead == false)*/
	{


		currentAnim = &runRAnim;

		waslookingRight = true;
	}

	//Die tester
	if (app->input->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
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

	if (CollisionFloorEnemy())
	{
		gravity = false;
	}
	else gravity = true;

	/*if (CollisionEnemy() == 2 && waslookingRight == true) speedx = 0;
	else if (CollisionEnemy() == 3 && waslookingRight == false) speedx = 0;
	else speedx = 16; // Change when have pathfinding*/

	CollisionWithPlayer();

	currentAnim->Update();

	return true;
}

// Called each loop iteration
bool WalkingEnemy::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(spriteSheet, -position.x, -position.y, &rect);

	return ret;
}

// Called before quitting
bool WalkingEnemy::CleanUp()
{
	LOG("Freeing walkingenemy");

	return true;
}

int WalkingEnemy::CollisionWithPlayer()
{
	fPoint posMapPlayer[numnPoints];
	fPoint posMapEnemy[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posMapPlayer[i] = app->map->WorldToMap(-app->player->position.x + (int)app->player->pointsFloorCollision[i][0], -app->player->position.y + (int)app->player->pointsFloorCollision[i][1]);
		posMapEnemy[i] = app->map->WorldToMap(-position.x + (int)pointsCollision[i][0], -position.y + (int)pointsCollision[i][1]);
	}



	return false;
}

int WalkingEnemy::CollisionEnemy()
{
	fPoint posMapEnemy[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posMapEnemy[i] = app->map->WorldToMap(-position.x + (int)pointsCollision[i][0], -position.y + (int)pointsCollision[i][1]);
		//if (CheckCollision(posMapPlayer[i]) == 3) app->SaveGameRequest("GameFile.xml");
	}
	if (CheckCollision(posMapEnemy[numnPoints - 1]) == 1) return 2;
	if (CheckCollision(posMapEnemy[numnPoints - 2]) == 1) return 3;

	return false;
}

bool WalkingEnemy::CollisionFloorEnemy()
{
	fPoint posFloorEnemy[numnPoints];

	for (int i = 0; i < numnPoints; i++)
	{
		posFloorEnemy[i] = app->map->WorldToMap(-position.x + (int)pointsFloorCollision[i][0], -position.y + (int)pointsFloorCollision[i][1]);
		if (CheckCollision(posFloorEnemy[i]) == 1) return true;
	}

	return false;
}

int WalkingEnemy::CheckCollision(fPoint positionMapEnemy)
{
	if (app->map->data.layers.At(3)->data->Get(positionMapEnemy.x, positionMapEnemy.y) != 0) return 1;
	if (app->map->data.layers.At(4)->data->Get(positionMapEnemy.x, positionMapEnemy.y) != 0) return 1;

	return false;
}

int WalkingEnemy:: 