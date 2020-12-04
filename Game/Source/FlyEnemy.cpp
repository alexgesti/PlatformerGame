#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Input.h"
#include "Map.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "FlyEnemy.h"
#include "ModuleController.h"
#include "Pathfinding.h"
#include "Player.h"

#include "Defs.h"
#include "Log.h"

FlyEnemy::FlyEnemy() : Module()
{
	name.Create("Flyenemy");

	//run left animation
	runLAnim.PushBack({ 0, 96, 32, 32 });
	runLAnim.PushBack({ 32, 96, 32, 32 });
	runLAnim.PushBack({ 64, 96, 32, 32 });
	runLAnim.loop = true;
	runLAnim.speed = 0.5f;

	//run rigth animation
	runRAnim.PushBack({ 128, 32, 32, 32 });
	runRAnim.PushBack({ 96, 32, 32, 32 });
	runRAnim.PushBack({ 64, 32, 32, 32 });
	runRAnim.loop = true;
	runRAnim.speed = 0.5f;

	//dead rigth animation
	deadRAnim.PushBack({ 128, 0, 32, 32 });
	deadRAnim.PushBack({ 96, 0, 32, 32 });
	deadRAnim.PushBack({ 64, 0, 32, 32 });
	deadRAnim.PushBack({ 32, 0, 32, 32 });
	deadRAnim.PushBack({ 0, 0, 32, 32 });
	deadRAnim.loop = false;
	deadRAnim.speed = 0.5f;

	//dead left animation
	deadLAnim.PushBack({ 0, 64, 32, 32 });
	deadLAnim.PushBack({ 32, 64, 32, 32 });
	deadLAnim.PushBack({ 64, 64, 32, 32 });
	deadLAnim.PushBack({ 96, 64, 32, 32 });
	deadLAnim.PushBack({ 128, 64, 32, 32 });
	deadLAnim.loop = false;
	deadLAnim.speed = 0.5f;
}

// Destructor
FlyEnemy::~FlyEnemy()
{}

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
	spriteSheet = app->tex->Load("Assets/textures/flyenemy_spritesheet.png");
	currentAnim = &runRAnim;

	position.x = -1430;
	position.y = -1920;

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
	//gravity = true;

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

	//if (CollisionEnemy({ position.x, position.y - speedy })) position = BeforePos;

	//app->pathfinding->CreatePath(position, app->player->position);

	currentAnim->Update();

	return true;
}

// Called each loop iteration
bool FlyEnemy::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(spriteSheet, -position.x, -position.y, &rect);

	return ret;
}

// Called before quitting
bool FlyEnemy::CleanUp()
{
	LOG("Freeing flyingenemy");

	return true;
}