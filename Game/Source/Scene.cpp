#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
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
		app->player->Godmode = true;

	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN);
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN);
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN);
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN);

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) app->render->camera.y--;
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) app->render->camera.y++;
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) app->render->camera.x--;
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) app->render->camera.x++;

	app->render->camera.x;

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
