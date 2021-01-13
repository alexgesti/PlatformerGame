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
#include "FadeController.h"

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
	obrn.PushBack({ 0, 32, 32, 32 });

	obrob.PushBack({ 0, 0, 32, 32 });
	obrob.PushBack({ 32, 0, 32, 32 });
	obrob.PushBack({ 64, 0, 32, 32 });
	obrob.PushBack({ 96, 0, 32, 32 });
	obrob.PushBack({ 128, 0, 32, 32 });
	obrob.PushBack({ 32, 32, 32, 32 });
	obrob.loop = false;
	obrob.speed = 0.1f;

	// Points
	p0.PushBack({ 0, 0, 432, 48 });
	p1.PushBack({ 0, 49, 432, 48 });

	//Collisions
	collisionorb.PushBack({ 160, 0, 24, 24 });
	collisionlife.PushBack({ 184, 0, 26, 24 });

	currentanimcollorb = &collisionorb;
	currentanimcolllife = &collisionlife;
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
	spritepillar = app->tex->Load("Assets/Screens/Gameplay/save_point_saving_x64.png");
	lifeplayer = app->tex->Load("Assets/Screens/Gameplay/lifLife_X64.png");
	psup = app->tex->Load("Assets/Screens/Gameplay/lifLife_X32.png");
	spriteorb = app->tex->Load("Assets/Screens/Gameplay/orb.png");
	collision = app->tex->Load("Assets/Screens/Gameplay/collision_entities.png");

    checkpointsound = app->audio->LoadFx("Assets/Audio/Fx/checkPoint.wav");
	oneupfx = app->audio->LoadFx("Assets/Audio/Fx/1up.wav");
	coinfx = app->audio->LoadFx("Assets/Audio/Fx/coin.wav");

	notsceneactived = false;
	pillaranim = &pillar;
	currentanimorb = &obrn;


	orbposition = { 3150, 1175 };
	psposition = { 3950, 1280 };

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
	if (notsceneactived)
	{
		app->render->camera.x = app->player->position.x + ((app->render->camera.w / 2) - app->player->playerWH / 2);		 
		app->render->camera.y = app->player->position.y + ((app->render->camera.h / 2) - app->player->playerWH / 2);
	}

	if (CheckCollisionRec(app->player->position, orbposition) == true && onlyonceorb == false)
	{
		currentanimorb = &obrob;
		app->audio->PlayFx(coinfx);
		onlyonceorb = true;
	}

	if (CheckCollisionRec(app->player->position, psposition) == true)
	{
		psposition = { 0, 0 };
		app->player->life++;
		app->audio->PlayFx(oneupfx);
		app->gamehud->points[1]++;
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (app->fade->canfade == false) app->scenepause->active = !app->scenepause->active;

		if (app->sceneopts->active == true) app->sceneopts->active = false;
	}

	if (obrob.FinishedAlready)
	{
		orbposition = { 0, 0 };
		orbobtained = true;
	}
	
	if (orbobtained)
	{
		app->gamehud->points[1]++;
		app->gamehud->coins[1]++;
		obrob.Reset();
		currentanimorb = &obrn;
		orbobtained = false;
	}

	// Draw map
	app->map->Draw();

	if (checkpointactive)
	{
		if (soundonetime == false && countercheckpointsound == 0)
		{
			app->audio->PlayFx(checkpointsound);
			soundonetime = true;
		}

		countercheckpointsound += dt;

		pillaranim->Update();
	}
	else
	{
		if (countercheckpointsound > 0) countercheckpointsound += dt;
	}

	if (orbobtained == false) currentanimorb->Update();

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	SDL_Rect rect = pillaranim->GetCurrentFrame();
	app->render->DrawTexture(spritepillar, poscheck1.x, poscheck1.y, &rect);
	app->render->DrawTexture(spritepillar, poscheck2.x, poscheck2.y, &rect);
	app->render->DrawTexture(spritepillar, poscheck3.x, poscheck3.y, &rect);

	SDL_Rect orbrect = currentanimorb->GetCurrentFrame();
	app->render->DrawTexture(spriteorb, orbposition.x, orbposition.y, &orbrect);

	app->render->DrawTexture(psup, psposition.x, psposition.y);
	
	if (app->modcontrol->showcollider)
	{
		SDL_Rect rectColOrb = currentanimcollorb->GetCurrentFrame();
		app->render->DrawTexture(collision, orbposition.x + 4, orbposition.y + 4, &rectColOrb);

		SDL_Rect rectColLife = currentanimcolllife->GetCurrentFrame();
		app->render->DrawTexture(collision, psposition.x + 2, psposition.y + 4, &rectColLife);
	}

	return ret;
}

// Load Scene State (Underconstruction)
bool Scene::LoadState(pugi::xml_node& data)
{
	// Player
	app->player->position.x = data.child("Player").attribute("x").as_int();
	app->player->position.y = data.child("Player").attribute("y").as_int();
	app->player->bposition.x = data.child("Player").attribute("Ballx").as_int();
	app->player->bposition.y = data.child("Player").attribute("Bally").as_int();
	app->player->dead = data.child("Player").attribute("Wasdead").as_bool();
	app->player->deadranim.FinishedAlready = data.child("Player").attribute("FinishedDeadRAnim").as_bool();
	app->player->deadlanim.FinishedAlready = data.child("Player").attribute("FinishedDeadLAnim").as_bool();
	app->player->jump = data.child("Player").attribute("Wasjumping").as_bool();
	app->player->lookingr = data.child("Player").attribute("Lookingright").as_bool();
	app->player->waslookingr = data.child("Player").attribute("Waslookingright").as_bool();
	app->player->top = data.child("Player").attribute("WasTop").as_bool();
	app->player->shoot = data.child("Player").attribute("WasShooting").as_bool();
	app->player->oncesound = data.child("Player").attribute("OnceSound").as_bool(); 
	app->player->gravity = data.child("Player").attribute("HasGravity").as_bool();
	app->player->maxjump = data.child("Player").attribute("maxJump").as_int();
	app->player->cooldown = data.child("Player").attribute("Cooldown").as_int();
	app->player->life = data.child("Player").attribute("PlayerLifes").as_int();
	app->player->godmode = data.child("Player").attribute("WasInGodMode").as_bool();	

	// Mushroom
	app->wenemy->position.x = data.child("Mushroom").attribute("Wx").as_int();
	app->wenemy->position.y = data.child("Mushroom").attribute("Wy").as_int();
	app->wenemy->dead = data.child("Mushroom").attribute("WWasdead").as_bool();
	app->wenemy->isdead = data.child("Mushroom").attribute("WIsWasdead").as_bool();
	app->wenemy->deadranim.FinishedAlready = data.child("Mushroom").attribute("WFinishedDeadRAnim").as_bool();
	app->wenemy->deadlanim.FinishedAlready = data.child("Mushroom").attribute("WFinishedDeadLAnim").as_bool();
	app->wenemy->waslookingright = data.child("Mushroom").attribute("WWaslookingright").as_bool();
	app->wenemy->gravity = data.child("Mushroom").attribute("WHasGravity").as_bool();
	app->wenemy->hitingplayer = data.child("Mushroom").attribute("WHitPlayer").as_bool();
	app->wenemy->oncesound = data.child("Mushroom").attribute("WOnceSound").as_bool();

	// Bat
	app->fenemy->position.x = data.child("Bat").attribute("Fx").as_int();
	app->fenemy->position.y = data.child("Bat").attribute("Fy").as_int();
	app->fenemy->dead = data.child("Bat").attribute("FWasdead").as_bool();
	app->fenemy->isdead = data.child("Bat").attribute("FIsWasdead").as_bool();
	app->fenemy->deadranim.FinishedAlready = data.child("Bat").attribute("FFinishedDeadRAnim").as_bool();
	app->fenemy->deadlanim.FinishedAlready = data.child("Bat").attribute("FFinishedDeadLAnim").as_bool();
	app->fenemy->waslookingright = data.child("Bat").attribute("FWaslookingright").as_bool();
	app->fenemy->hitingplayer = data.child("Bat").attribute("FHitPlayer").as_bool();
	app->fenemy->oncesound = data.child("Bat").attribute("FOnceSound").as_bool();

	// Scene
	checkpointactive = data.child("Scene").attribute("checkpointActive").as_bool();
	orbobtained = data.child("Scene").attribute("OrbObtained").as_bool();
	countercheckpointsound = data.child("Scene").attribute("counterCheckPointSound").as_float();
	orbposition.x = data.child("Scene").attribute("OrbPosX").as_int();
	orbposition.y = data.child("Scene").attribute("OrbPosY").as_int();
	obrob.FinishedAlready = data.child("Scene").attribute("AnimOrbFinished").as_bool();
	onlyonceorb = data.child("Scene").attribute("OnlyOnceOrb").as_bool();
	psposition.x = data.child("Scene").attribute("PSpositionX").as_int();
	psposition.y = data.child("Scene").attribute("PSpositionY").as_int();
	poscheck1.x = data.child("Scene").attribute("PosCheck1X").as_int();
	poscheck1.y	= data.child("Scene").attribute("PosCheck1Y").as_int();
	poscheck2.x	= data.child("Scene").attribute("PosCheck2X").as_int();
	poscheck2.y	= data.child("Scene").attribute("PosCheck2Y").as_int();
	poscheck3.x	= data.child("Scene").attribute("PosCheck3X").as_int();
	poscheck3.y	= data.child("Scene").attribute("PosCheck3Y").as_int();

	// GameplayHUD
	app->gamehud->digits[0] = data.child("GameHUD").attribute("clockdigit0").as_int();
	app->gamehud->digits[1] = data.child("GameHUD").attribute("clockdigit1").as_int();
	app->gamehud->digits[2] = data.child("GameHUD").attribute("clockdigit2").as_int();
	app->gamehud->counter = data.child("GameHUD").attribute("CounterClock").as_float();
	app->gamehud->points[0] = data.child("GameHUD").attribute("points0").as_int();
	app->gamehud->points[1] = data.child("GameHUD").attribute("points1").as_int();
	app->gamehud->points[2] = data.child("GameHUD").attribute("points2").as_int();
	app->gamehud->points[3] = data.child("GameHUD").attribute("points3").as_int();
	app->gamehud->points[4] = data.child("GameHUD").attribute("points4").as_int();
	app->gamehud->coins[0] = data.child("GameHUD").attribute("coin1").as_int();
	app->gamehud->coins[1] = data.child("GameHUD").attribute("coin2").as_int();

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
	playersave.append_attribute("Ballx") = app->player->bposition.x;
	playersave.append_attribute("Bally") = app->player->bposition.y;
	playersave.append_attribute("Wasdead") = app->player->dead;
	playersave.append_attribute("FinishedDeadRAnim") = app->player->deadranim.FinishedAlready;
	playersave.append_attribute("FinishedDeadLAnim") = app->player->deadlanim.FinishedAlready;
	playersave.append_attribute("Wasjumping") = app->player->jump;
	playersave.append_attribute("Lookingright") = app->player->lookingr;
	playersave.append_attribute("Waslookingright") = app->player->waslookingr;
	playersave.append_attribute("OnceSound") = app->player->oncesound;
	playersave.append_attribute("WasTop") = app->player->top;
	playersave.append_attribute("WasShooting") = app->player->shoot;
	playersave.append_attribute("maxJump") = app->player->maxjump;
	playersave.append_attribute("Cooldown") = app->player->cooldown;
	playersave.append_attribute("Waslookingright") = app->player->lookingr;
	playersave.append_attribute("HasGravity") = app->player->gravity;
	playersave.append_attribute("WasInGodMode") = app->player->godmode;
	playersave.append_attribute("PlayerLifes") = app->player->life;

	// Mushroom
	walkenemysave.append_attribute("Wx") = app->wenemy->position.x;
	walkenemysave.append_attribute("Wy") = app->wenemy->position.y;
	walkenemysave.append_attribute("WWasdead") = app->wenemy->dead;
	walkenemysave.append_attribute("WIsWasdead") = app->wenemy->isdead;
	walkenemysave.append_attribute("WFinishedDeadRAnim") = app->wenemy->deadranim.FinishedAlready;
	walkenemysave.append_attribute("WFinishedDeadLAnim") = app->wenemy->deadlanim.FinishedAlready;
	walkenemysave.append_attribute("WWaslookingright") = app->wenemy->waslookingright;
	walkenemysave.append_attribute("WHasGravity") = app->wenemy->gravity;
	walkenemysave.append_attribute("WHitPlayer") = app->wenemy->hitingplayer;
	walkenemysave.append_attribute("WOnceSound") = app->wenemy->oncesound;

	// Bat
	flyenemysave.append_attribute("Fx") = app->fenemy->position.x;
	flyenemysave.append_attribute("Fy") = app->fenemy->position.y;
	flyenemysave.append_attribute("FWasdead") = app->fenemy->dead;
	flyenemysave.append_attribute("FIsWasdead") = app->fenemy->isdead;
	flyenemysave.append_attribute("FFinishedDeadRAnim") = app->fenemy->deadranim.FinishedAlready;
	flyenemysave.append_attribute("FFinishedDeadLAnim") = app->fenemy->deadlanim.FinishedAlready;
	flyenemysave.append_attribute("FWaslookingright") = app->fenemy->waslookingright;
	flyenemysave.append_attribute("FHitPlayer") = app->fenemy->hitingplayer;
	flyenemysave.append_attribute("FOnceSound") = app->fenemy->oncesound;

	// Scene
	scenesave.append_attribute("checkpointActive") = checkpointactive;
	scenesave.append_attribute("OrbObtained") = orbobtained;
	scenesave.append_attribute("counterCheckPointSound") = countercheckpointsound;
	scenesave.append_attribute("OrbPosX") = orbposition.x;
	scenesave.append_attribute("OrbPosY") = orbposition.y;
	scenesave.append_attribute("AnimOrbFinished") = obrob.FinishedAlready;
	scenesave.append_attribute("OnlyOnceOrb") = onlyonceorb;
	scenesave.append_attribute("PSpositionX") = psposition.x;
	scenesave.append_attribute("PSpositionY") = psposition.y;
	scenesave.append_attribute("PosCheck1X") = poscheck1.x;
	scenesave.append_attribute("PosCheck1Y") = poscheck1.y;
	scenesave.append_attribute("PosCheck2X") = poscheck2.x;
	scenesave.append_attribute("PosCheck2Y") = poscheck2.y;
	scenesave.append_attribute("PosCheck3X") = poscheck3.x;
	scenesave.append_attribute("PosCheck3Y") = poscheck3.y;

	// GameplayHUD
	gamehud.append_attribute("clockdigit0") = app->gamehud->digits[0];
	gamehud.append_attribute("clockdigit1") = app->gamehud->digits[1];
	gamehud.append_attribute("clockdigit2") = app->gamehud->digits[2];
	gamehud.append_attribute("CounterClock") = app->gamehud->counter;
	gamehud.append_attribute("points0") = app->gamehud->points[0];
	gamehud.append_attribute("points1") = app->gamehud->points[1];
	gamehud.append_attribute("points2") = app->gamehud->points[2];
	gamehud.append_attribute("points3") = app->gamehud->points[3];
	gamehud.append_attribute("points4") = app->gamehud->points[4];
	gamehud.append_attribute("coin1") = app->gamehud->coins[0];
	gamehud.append_attribute("coin2") = app->gamehud->coins[1];

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
