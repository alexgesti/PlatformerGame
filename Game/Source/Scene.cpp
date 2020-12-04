#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"
#include "Player.h"

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
	pillar.speed = 0.25f;
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

	NotSceneActived = false;
	PillarAnim = &pillar;

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

	// Draw map
	app->map->Draw();

	PillarAnim->Update();

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

	return ret;
}

// Load Scene State (Underconstruction)
bool Scene::LoadState(pugi::xml_node& data)
{
	app->player->position.x = data.child("Player").attribute("x").as_int();
	app->player->position.y = data.child("Player").attribute("y").as_int();
	app->player->dead = data.child("Player").attribute("Wasdead").as_bool();
	app->player->deadRAnim.FinishedAlready = data.child("Player").attribute("FinishedDeadRAnim").as_bool();
	app->player->deadLAnim.FinishedAlready = data.child("Player").attribute("FinishedDeadLAnim").as_bool();
	app->player->jump = data.child("Player").attribute("Wasjumping").as_bool();
	app->player->LookingR = data.child("Player").attribute("Waslookingright").as_bool();
	app->player->gravity = data.child("Player").attribute("HasGravity").as_bool();
	app->player->Godmode = data.child("Player").attribute("WasInGodMode").as_bool();

	return true;
}

// Save Scene State (Underconstruction)
bool Scene::SaveState(pugi::xml_node& data) const
{
	pugi::xml_node playersave = data.append_child("Player");

	playersave.append_attribute("x") = app->player->position.x;
	playersave.append_attribute("y") = app->player->position.y;
	playersave.append_attribute("Wasdead") = app->player->dead;
	playersave.append_attribute("FinishedDeadRAnim") = app->player->deadRAnim.FinishedAlready;
	playersave.append_attribute("FinishedDeadLAnim") = app->player->deadLAnim.FinishedAlready;
	playersave.append_attribute("Wasjumping") = app->player->jump;
	playersave.append_attribute("Waslookingright") = app->player->LookingR;
	playersave.append_attribute("HasGravity") = app->player->gravity;
	playersave.append_attribute("WasInGodMode") = app->player->Godmode;

	return true;
}

bool Scene::Reset()
{
	app->player->position.x = -1544;
	app->player->position.y = -1920;
	app->player->dead = false;
	app->player->jump = false;
	app->player->LookingR = true;
	app->player->gravity = false;
	app->player->Godmode = false;

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
