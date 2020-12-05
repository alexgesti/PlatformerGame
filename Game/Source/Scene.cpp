#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"
#include "WalkingEnemy.h"
#include "FlyEnemy.h"
#include "Audio.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");

	//pillar animation
	pillar.PushBack({ 0, 0, 48, 80 });
	pillar.PushBack({ 48, 0, 48, 80 });
	pillar.PushBack({ 96, 0, 48, 80 });
	pillar.loop = true;
	pillar.speed = 0.15f;

	//Orb
	obrN.PushBack({ 0, 32, 32, 32 });

	obrOb.PushBack({ 0, 0, 32, 32 });
	obrOb.PushBack({ 32, 0, 32, 32 });
	obrOb.PushBack({ 64, 0, 32, 32 });
	obrOb.PushBack({ 96, 0, 32, 32 });
	obrOb.PushBack({ 128, 0, 32, 32 });
	obrOb.PushBack({ 32, 32, 32, 32 });
	obrOb.loop = false;
	obrOb.speed = 0.1f;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	// L03: DONE: Load map
	app->map->Load("mapa.tmx");
	spritePillar = app->tex->Load("Assets/textures/save_point_saving-x64.png");
	lifePlayer = app->tex->Load("Assets/textures/lifLife_X64.png");
	spriteorb = app->tex->Load("Assets/textures/orb.png");
    checkpointSound = app->audio->LoadFx("Assets/audio/sound/checkPoint.wav");
	NotSceneActived = false;
	PillarAnim = &pillar;
	CurrentAnimOrb = &obrN;

	Orbposition.x = 3150;
	Orbposition.y = 1175;

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	if (NotSceneActived)
	{
		app->render->camera.x = app->player->position.x + ((app->render->camera.w / 2) - app->player->playerWH / 2);		 
		app->render->camera.y = app->player->position.y + ((app->render->camera.h / 2) - app->player->playerWH / 2);
	}

	if (CheckCollisionRec(app->player->position, Orbposition) == true)
	{
		CurrentAnimOrb = &obrOb;
	}

	if (obrOb.FinishedAlready)
	{
		Orbposition.x = 0;
		Orbposition.y = 0;
	}

	// Draw map
	app->map->Draw();

	if (CheckPointActive) PillarAnim->Update();

	CurrentAnimOrb->Update();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = PillarAnim->GetCurrentFrame();
	app->render->DrawTexture(spritePillar, 2057, 1905, &rect);
	app->render->DrawTexture(spritePillar, 4552, 1008, &rect);
	app->render->DrawTexture(spritePillar, 6472, 624, &rect);

	for (int i = 0; i < app->player->life; i++)
	{
		app->render->DrawTexture(lifePlayer, -app->render->camera.x + (64*i), -app->render->camera.y);
	}

	SDL_Rect orbrect = CurrentAnimOrb->GetCurrentFrame();
	app->render->DrawTexture(spriteorb, Orbposition.x, Orbposition.y, &orbrect);
	
	return ret;
}

// Load Scene State (Underconstruction)
bool Scene::LoadState(pugi::xml_node& data)
{
	// Player
	app->player->position.x = data.child("Player").attribute("x").as_int();
	app->player->position.y = data.child("Player").attribute("y").as_int();
	app->player->Bposition.x = data.child("Player").attribute("Ballx").as_int();
	app->player->Bposition.y = data.child("Player").attribute("Bally").as_int();
	app->player->dead = data.child("Player").attribute("Wasdead").as_bool();
	app->player->deadRAnim.FinishedAlready = data.child("Player").attribute("FinishedDeadRAnim").as_bool();
	app->player->deadLAnim.FinishedAlready = data.child("Player").attribute("FinishedDeadLAnim").as_bool();
	app->player->jump = data.child("Player").attribute("Wasjumping").as_bool();
	app->player->LookingR = data.child("Player").attribute("Lookingright").as_bool();
	app->player->WasLookingR = data.child("Player").attribute("Waslookingright").as_bool();
	app->player->top = data.child("Player").attribute("WasTop").as_bool();
	app->player->shoot = data.child("Player").attribute("WasShooting").as_bool();
	app->player->gravity = data.child("Player").attribute("HasGravity").as_bool();
	app->player->maxJump = data.child("Player").attribute("maxJump").as_int();
	app->player->cooldown = data.child("Player").attribute("Cooldown").as_int();
	app->player->life = data.child("Player").attribute("PlayerLifes").as_int();
	app->player->Godmode = data.child("Player").attribute("WasInGodMode").as_bool();	

	// Mushroom
	app->wenemy->position.x = data.child("Mushroom").attribute("Wx").as_int();
	app->wenemy->position.y = data.child("Mushroom").attribute("Wy").as_int();
	app->wenemy->dead = data.child("Mushroom").attribute("WWasdead").as_bool();
	app->wenemy->IsDead = data.child("Mushroom").attribute("WIsWasdead").as_bool();
	app->wenemy->deadRAnim.FinishedAlready = data.child("Mushroom").attribute("WFinishedDeadRAnim").as_bool();
	app->wenemy->deadLAnim.FinishedAlready = data.child("Mushroom").attribute("WFinishedDeadLAnim").as_bool();
	app->wenemy->waslookingRight = data.child("Mushroom").attribute("WWaslookingright").as_bool();
	app->wenemy->gravity = data.child("Mushroom").attribute("WHasGravity").as_bool();
	app->wenemy->fall = data.child("Mushroom").attribute("WWasFalling").as_bool();
	app->wenemy->gravity = data.child("Mushroom").attribute("WHasGravity").as_bool();

	// Bat
	app->fenemy->position.x = data.child("Bat").attribute("Fx").as_int();
	app->fenemy->position.y = data.child("Bat").attribute("Fy").as_int();
	app->fenemy->dead = data.child("Bat").attribute("FWasdead").as_bool();
	app->fenemy->IsDead = data.child("Bat").attribute("FIsWasdead").as_bool();
	app->fenemy->deadRAnim.FinishedAlready = data.child("Bat").attribute("FFinishedDeadRAnim").as_bool();
	app->fenemy->deadLAnim.FinishedAlready = data.child("Bat").attribute("FFinishedDeadLAnim").as_bool();
	app->fenemy->waslookingRight = data.child("Bat").attribute("FWaslookingright").as_bool();

	return true;
}

// Save Scene State (Underconstruction)
bool Scene::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node playersave = data.append_child("Player");
	pugi::xml_node walkenemysave = data.append_child("Mushroom");
	pugi::xml_node flyenemysave = data.append_child("Bat");

	playersave.append_attribute("x") = app->player->position.x;
	playersave.append_attribute("y") = app->player->position.y;
	playersave.append_attribute("Ballx") = app->player->Bposition.x;
	playersave.append_attribute("Bally") = app->player->Bposition.y;
	playersave.append_attribute("Wasdead") = app->player->dead;
	playersave.append_attribute("FinishedDeadRAnim") = app->player->deadRAnim.FinishedAlready;
	playersave.append_attribute("FinishedDeadLAnim") = app->player->deadLAnim.FinishedAlready;
	playersave.append_attribute("Wasjumping") = app->player->jump;
	playersave.append_attribute("Lookingright") = app->player->LookingR;
	playersave.append_attribute("Waslookingright") = app->player->WasLookingR;
	playersave.append_attribute("WasTop") = app->player->top;
	playersave.append_attribute("WasShooting") = app->player->shoot;
	playersave.append_attribute("maxJump") = app->player->maxJump;
	playersave.append_attribute("Cooldown") = app->player->cooldown;
	playersave.append_attribute("Waslookingright") = app->player->LookingR;
	playersave.append_attribute("HasGravity") = app->player->gravity;
	playersave.append_attribute("WasInGodMode") = app->player->Godmode;
	playersave.append_attribute("PlayerLifes") = app->player->life;

	walkenemysave.append_attribute("Wx") = app->wenemy->position.x;
	walkenemysave.append_attribute("Wy") = app->wenemy->position.y;
	walkenemysave.append_attribute("WWasdead") = app->wenemy->dead;
	walkenemysave.append_attribute("WIsWasdead") = app->wenemy->IsDead;
	walkenemysave.append_attribute("WFinishedDeadRAnim") = app->wenemy->deadRAnim.FinishedAlready;
	walkenemysave.append_attribute("WFinishedDeadLAnim") = app->wenemy->deadLAnim.FinishedAlready;
	walkenemysave.append_attribute("WWaslookingright") = app->wenemy->waslookingRight;
	walkenemysave.append_attribute("WWasFalling") = app->wenemy->fall;
	walkenemysave.append_attribute("WHasGravity") = app->wenemy->gravity;

	flyenemysave.append_attribute("Fx") = app->fenemy->position.x;
	flyenemysave.append_attribute("Fy") = app->fenemy->position.y;
	flyenemysave.append_attribute("FWasdead") = app->fenemy->dead;
	flyenemysave.append_attribute("FIsWasdead") = app->fenemy->IsDead;
	flyenemysave.append_attribute("FFinishedDeadRAnim") = app->fenemy->deadRAnim.FinishedAlready;
	flyenemysave.append_attribute("FFinishedDeadLAnim") = app->fenemy->deadLAnim.FinishedAlready;
	flyenemysave.append_attribute("FWaslookingright") = app->fenemy->waslookingRight;

	return true;
}

bool Scene::CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapOrb)
{
	if ((-positionMapPlayer.x < (positionMapOrb.x + 52)) && ((-positionMapPlayer.x + 52) > positionMapOrb.x) &&
		(-positionMapPlayer.y < (positionMapOrb.y + 64)) && ((-positionMapPlayer.y + 64) > positionMapOrb.y)) return true;


	return false;
}

bool Scene::Reset()
{
	app->LoadGameRequest("StartValues.xml");

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
