#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name.Create("scene");
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

	spriteSheet = app->tex->Load("Assets/textures/hero_idle.gif");
	
	// Load music
	app->audio->PlayMusic("Assets/audio/music/music_spy.ogg");

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
    // L02: DONE 3: Request Load / Save when pressing L/S
	if(app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest("savegame.xml");

	if(app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest("savegame.xml");

	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		if (Godmode == false)
			Godmode = true;
		else if (Godmode == true)
			Godmode = false;

	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		if (app->map->showCollider == false)
			app->map->showCollider = true;
		else if (app->map->showCollider == true)
			app->map->showCollider = false;
	}

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN);
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		LOG("%d\t%d", app->render->camera.w, app->render->camera.h);


	//Player
	{
		if (Godmode == false)
		{
			//Gravity
			/*if (gravity == true)
				position.y -= speedy;*/

			//Mov left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
				app->render->camera.x += speedx;

			//Mov right
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
				app->render->camera.x -= speedx;

			//Jump
			if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && gravity == false)
			{
				jump = true;
				maxJump = app->render->camera.y;
			}
			if (jump == true && app->render->camera.y < maxJump + 40)
			{
				app->render->camera.y += (2 * speedy);
			}
		}

		//Godmode
		if (Godmode == true)
		{
			gravity = false;

			//Mov left
			if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_D) == KEY_IDLE)
				app->render->camera.x += 1;

			//Mov right
			if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_A) == KEY_IDLE)
				app->render->camera.x -= 1;

			//Mov up
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_S) == KEY_IDLE)
				app->render->camera.y += 1;

			//Mov down
			if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_W) == KEY_IDLE)
				app->render->camera.y -= 1;
		}

		app->render->DrawTexture(spriteSheet, app->render->camera.w / 2 - app->render->camera.x, app->render->camera.h / 2 - app->render->camera.y);
	}
	

	// Draw map
	app->map->Draw();

	ListItem<TileSet*>* itemL;
	itemL = app->map->data.tilesets.start;

	while (itemL != NULL)
	{
		app->render->DrawTexture(itemL->data->texture, 0, 0);

		itemL = itemL->next;
	}

	// L03: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Camera position:%d %d",
				   app->map->data.width, app->map->data.height,
				   app->map->data.tileWidth, app->map->data.tileHeight,
				   app->map->data.tilesets.count(),
				   app->render->camera.x,
				   app->render->camera.y);

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}
