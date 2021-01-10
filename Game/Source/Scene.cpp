#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "ScenePause.h"
#include "SceneOptions.h"
#include "Map.h"
#include "Player.h"
#include "WalkingEnemy.h"
#include "FlyEnemy.h"
#include "Audio.h"
#include "GameplayHUD.h"
#include "ModuleController.h"

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
	pillar.speed = 0.12f;

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

	// Points
	p0.PushBack({ 0, 0, 432, 48 });
	p1.PushBack({ 0, 49, 432, 48 });

	//Collisions
	CollisionOrb.PushBack({ 160, 0, 24, 24 });
	CollisionLife.PushBack({ 184, 0, 26, 24 });

	currentAnimCollOrb = &CollisionOrb;
	currentAnimCollLife = &CollisionLife;
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
	spritePillar = app->tex->Load("Assets/Screens/Gameplay/save_point_saving_x64.png");
	lifePlayer = app->tex->Load("Assets/Screens/Gameplay/lifLife_X64.png");
	PSup = app->tex->Load("Assets/Screens/Gameplay/lifLife_X32.png");
	spriteorb = app->tex->Load("Assets/Screens/Gameplay/orb.png");
	collision = app->tex->Load("Assets/Screens/Gameplay/collision_entities.png");

    checkpointSound = app->audio->LoadFx("Assets/Audio/Fx/checkPoint.wav");
	oneupFx = app->audio->LoadFx("Assets/Audio/Fx/1up.wav");
	coinFx = app->audio->LoadFx("Assets/Audio/Fx/coin.wav");

	NotSceneActived = false;
	PillarAnim = &pillar;
	CurrentAnimOrb = &obrN;


	Orbposition = { 3150, 1175 };
	PSposition = { 3950, 1280 };

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

	if (CheckCollisionRec(app->player->position, Orbposition) == true && OnlyOnceOrb == false)
	{
		CurrentAnimOrb = &obrOb;
		app->audio->PlayFx(coinFx);
		OnlyOnceOrb = true;
	}

	if (CheckCollisionRec(app->player->position, PSposition) == true)
	{
		PSposition = { 0, 0 };
		app->player->life++;
		app->audio->PlayFx(oneupFx);
		app->GameHUD->points[1]++;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		app->scenePause->active = !app->scenePause->active;

		if (app->sceneOpts->active == true) app->sceneOpts->active = false;
	}

	if (obrOb.FinishedAlready)
	{
		Orbposition = { 0, 0 };
		OrbObtained = true;
	}
	
	if (OrbObtained)
	{
		app->GameHUD->points[1]++;
		app->GameHUD->coins[1]++;
		obrOb.Reset();
		CurrentAnimOrb = &obrN;
		OrbObtained = false;
	}

	// Draw map
	app->map->Draw();

	if (CheckPointActive)
	{
		if (SoundOneTime == false && counterCheckPointSound == 0)
		{
			app->audio->PlayFx(checkpointSound);
			SoundOneTime = true;
		}

		counterCheckPointSound += dt;

		PillarAnim->Update();
	}
	else
	{
		if (counterCheckPointSound > 0) counterCheckPointSound += dt;
	}

	if (OrbObtained == false) CurrentAnimOrb->Update();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = PillarAnim->GetCurrentFrame();
	app->render->DrawTexture(spritePillar, PosCheck1.x, PosCheck1.y, &rect);
	app->render->DrawTexture(spritePillar, PosCheck2.x, PosCheck2.y, &rect);
	app->render->DrawTexture(spritePillar, PosCheck3.x, PosCheck3.y, &rect);

	SDL_Rect orbrect = CurrentAnimOrb->GetCurrentFrame();
	app->render->DrawTexture(spriteorb, Orbposition.x, Orbposition.y, &orbrect);

	app->render->DrawTexture(PSup, PSposition.x, PSposition.y);
	
	if (app->modcontrol->showCollider)
	{
		SDL_Rect rectColOrb = currentAnimCollOrb->GetCurrentFrame();
		app->render->DrawTexture(collision, Orbposition.x + 4, Orbposition.y + 4, &rectColOrb);

		SDL_Rect rectColLife = currentAnimCollLife->GetCurrentFrame();
		app->render->DrawTexture(collision, PSposition.x + 2, PSposition.y + 4, &rectColLife);
	}

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
	app->player->oncesound = data.child("Player").attribute("OnceSound").as_bool(); 
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
	app->wenemy->hitingPlayer = data.child("Mushroom").attribute("WHitPlayer").as_bool();
	app->wenemy->oncesound = data.child("Mushroom").attribute("WOnceSound").as_bool();

	// Bat
	app->fenemy->position.x = data.child("Bat").attribute("Fx").as_int();
	app->fenemy->position.y = data.child("Bat").attribute("Fy").as_int();
	app->fenemy->dead = data.child("Bat").attribute("FWasdead").as_bool();
	app->fenemy->IsDead = data.child("Bat").attribute("FIsWasdead").as_bool();
	app->fenemy->deadRAnim.FinishedAlready = data.child("Bat").attribute("FFinishedDeadRAnim").as_bool();
	app->fenemy->deadLAnim.FinishedAlready = data.child("Bat").attribute("FFinishedDeadLAnim").as_bool();
	app->fenemy->waslookingRight = data.child("Bat").attribute("FWaslookingright").as_bool();
	app->fenemy->hitingPlayer = data.child("Bat").attribute("FHitPlayer").as_bool();
	app->fenemy->oncesound = data.child("Bat").attribute("FOnceSound").as_bool();

	// Scene
	CheckPointActive = data.child("Scene").attribute("checkpointActive").as_bool();
	OrbObtained = data.child("Scene").attribute("OrbObtained").as_bool();
	counterCheckPointSound = data.child("Scene").attribute("counterCheckPointSound").as_float();
	Orbposition.x = data.child("Scene").attribute("OrbPosX").as_int();
	Orbposition.y = data.child("Scene").attribute("OrbPosY").as_int();
	obrOb.FinishedAlready = data.child("Scene").attribute("AnimOrbFinished").as_bool();
	OnlyOnceOrb = data.child("Scene").attribute("OnlyOnceOrb").as_bool();
	PSposition.x = data.child("Scene").attribute("PSpositionX").as_int();
	PSposition.y = data.child("Scene").attribute("PSpositionY").as_int();
	PosCheck1.x = data.child("Scene").attribute("PosCheck1X").as_int();
	PosCheck1.y	= data.child("Scene").attribute("PosCheck1Y").as_int();
	PosCheck2.x	= data.child("Scene").attribute("PosCheck2X").as_int();
	PosCheck2.y	= data.child("Scene").attribute("PosCheck2Y").as_int();
	PosCheck3.x	= data.child("Scene").attribute("PosCheck3X").as_int();
	PosCheck3.y	= data.child("Scene").attribute("PosCheck3Y").as_int();

	// GameplayHUD
	app->GameHUD->digits[0] = data.child("GameHUD").attribute("clockdigit0").as_int();
	app->GameHUD->digits[1] = data.child("GameHUD").attribute("clockdigit1").as_int();
	app->GameHUD->digits[2] = data.child("GameHUD").attribute("clockdigit2").as_int();
	app->GameHUD->Counter = data.child("GameHUD").attribute("CounterClock").as_float();
	app->GameHUD->points[0] = data.child("GameHUD").attribute("points0").as_int();
	app->GameHUD->points[1] = data.child("GameHUD").attribute("points1").as_int();
	app->GameHUD->points[2] = data.child("GameHUD").attribute("points2").as_int();
	app->GameHUD->points[3] = data.child("GameHUD").attribute("points3").as_int();
	app->GameHUD->points[4] = data.child("GameHUD").attribute("points4").as_int();
	app->GameHUD->coins[0] = data.child("GameHUD").attribute("coin1").as_int();
	app->GameHUD->coins[1] = data.child("GameHUD").attribute("coin2").as_int();

	return true;
}

// Save Scene State (Underconstruction)
bool Scene::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node playersave = data.append_child("Player");
	pugi::xml_node walkenemysave = data.append_child("Mushroom");
	pugi::xml_node flyenemysave = data.append_child("Bat");
	pugi::xml_node scenesave = data.append_child("Scene");
	pugi::xml_node gamehud = data.append_child("GameHUD");

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
	playersave.append_attribute("OnceSound") = app->player->oncesound;
	playersave.append_attribute("WasTop") = app->player->top;
	playersave.append_attribute("WasShooting") = app->player->shoot;
	playersave.append_attribute("maxJump") = app->player->maxJump;
	playersave.append_attribute("Cooldown") = app->player->cooldown;
	playersave.append_attribute("Waslookingright") = app->player->LookingR;
	playersave.append_attribute("HasGravity") = app->player->gravity;
	playersave.append_attribute("WasInGodMode") = app->player->Godmode;
	playersave.append_attribute("PlayerLifes") = app->player->life;

	// Mushroom
	walkenemysave.append_attribute("Wx") = app->wenemy->position.x;
	walkenemysave.append_attribute("Wy") = app->wenemy->position.y;
	walkenemysave.append_attribute("WWasdead") = app->wenemy->dead;
	walkenemysave.append_attribute("WIsWasdead") = app->wenemy->IsDead;
	walkenemysave.append_attribute("WFinishedDeadRAnim") = app->wenemy->deadRAnim.FinishedAlready;
	walkenemysave.append_attribute("WFinishedDeadLAnim") = app->wenemy->deadLAnim.FinishedAlready;
	walkenemysave.append_attribute("WWaslookingright") = app->wenemy->waslookingRight;
	walkenemysave.append_attribute("WHasGravity") = app->wenemy->gravity;
	walkenemysave.append_attribute("WHitPlayer") = app->wenemy->hitingPlayer;
	walkenemysave.append_attribute("WOnceSound") = app->wenemy->oncesound;

	// Bat
	flyenemysave.append_attribute("Fx") = app->fenemy->position.x;
	flyenemysave.append_attribute("Fy") = app->fenemy->position.y;
	flyenemysave.append_attribute("FWasdead") = app->fenemy->dead;
	flyenemysave.append_attribute("FIsWasdead") = app->fenemy->IsDead;
	flyenemysave.append_attribute("FFinishedDeadRAnim") = app->fenemy->deadRAnim.FinishedAlready;
	flyenemysave.append_attribute("FFinishedDeadLAnim") = app->fenemy->deadLAnim.FinishedAlready;
	flyenemysave.append_attribute("FWaslookingright") = app->fenemy->waslookingRight;
	flyenemysave.append_attribute("FHitPlayer") = app->fenemy->hitingPlayer;
	flyenemysave.append_attribute("FOnceSound") = app->fenemy->oncesound;

	// Scene
	scenesave.append_attribute("checkpointActive") = CheckPointActive;
	scenesave.append_attribute("OrbObtained") = OrbObtained;
	scenesave.append_attribute("counterCheckPointSound") = counterCheckPointSound;
	scenesave.append_attribute("OrbPosX") = Orbposition.x;
	scenesave.append_attribute("OrbPosY") = Orbposition.y;
	scenesave.append_attribute("AnimOrbFinished") = obrOb.FinishedAlready;
	scenesave.append_attribute("OnlyOnceOrb") = OnlyOnceOrb;
	scenesave.append_attribute("PSpositionX") = PSposition.x;
	scenesave.append_attribute("PSpositionY") = PSposition.y;
	scenesave.append_attribute("PosCheck1X") = PosCheck1.x;
	scenesave.append_attribute("PosCheck1Y") = PosCheck1.y;
	scenesave.append_attribute("PosCheck2X") = PosCheck2.x;
	scenesave.append_attribute("PosCheck2Y") = PosCheck2.y;
	scenesave.append_attribute("PosCheck3X") = PosCheck3.x;
	scenesave.append_attribute("PosCheck3Y") = PosCheck3.y;

	// GameplayHUD
	gamehud.append_attribute("clockdigit0") = app->GameHUD->digits[0];
	gamehud.append_attribute("clockdigit1") = app->GameHUD->digits[1];
	gamehud.append_attribute("clockdigit2") = app->GameHUD->digits[2];
	gamehud.append_attribute("CounterClock") = app->GameHUD->Counter;
	gamehud.append_attribute("points0") = app->GameHUD->points[0];
	gamehud.append_attribute("points1") = app->GameHUD->points[1];
	gamehud.append_attribute("points2") = app->GameHUD->points[2];
	gamehud.append_attribute("points3") = app->GameHUD->points[3];
	gamehud.append_attribute("points4") = app->GameHUD->points[4];
	gamehud.append_attribute("coin1") = app->GameHUD->coins[0];
	gamehud.append_attribute("coin2") = app->GameHUD->coins[1];

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
