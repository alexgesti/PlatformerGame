#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class Scene;
class SceneIntro;
class SceneLogo;
class SceneLose;
class SceneWin;
class SceneOptions;
class ScenePause;
class Map;
class Player;
class ModuleController;
class WalkingEnemy;
class FlyEnemy;
class EntityManager;
class PathFinding;
class GameplayHUD;
class FadeController;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

    // L02: DONE 1: Create methods to request Load / Save
	void LoadGameRequest(const char* fileName);
	void SaveGameRequest(const char* fileName) const;

private:

	// Load config file
	// NOTE: It receives config document
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGame();
	bool SaveGame() const;

	void FramerateLogic();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	Scene* scene;
	SceneIntro* sceneintro;
	SceneLogo* scenelogo;
	SceneLose* scenelose;
	SceneWin* scenewin;
	SceneOptions* sceneopts;
	ScenePause* scenepause;
	Map* map;
	Player* player;
	ModuleController* modcontrol;
	WalkingEnemy* wenemy;
	FlyEnemy* fenemy;
	EntityManager* entity;
	PathFinding* pathfinding;
	GameplayHUD* gamehud;
	FadeController* fade;

	uint32 cappedms;

	bool savedataexist;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module *> modules;

	// L01: DONE 2: Create new variables from pugui namespace
	// NOTE: Redesigned LoadConfig() to avoid storing this variables
	//pugi::xml_document configFile;
	//pugi::xml_node config;
	//pugi::xml_node configApp;

	uint frames;


	mutable bool savegamerequested;
	bool loadgamerequested;
	SString loadedgame;	
	mutable SString savedgame;

	// L07: TODO 4: Calculate some timing measures
	// required variables are provided:
	PerfTimer ptimer;
	PerfTimer delayTimer;

	float averagefps = 0.0f;
	float secondssincestartup = 0.0f;
	uint32 lastframems = 0;
	uint32 framesonlastupdate = 0;
	uint32 delaytime = 0;

	Timer startuptime;
	Timer frametime;
	Timer lastsecframetime;
	uint32 lastsecframecount = 0;
	uint32 prevlastsecframecount = 0;
	float dt = 0.0f;

public:

	uint64 framecount = 0;

};

extern App* app;

#endif	// __APP_H__