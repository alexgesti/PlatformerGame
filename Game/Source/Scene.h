#ifndef __SCENE_H__
#define __SCENE_H__

#include "SceneManager.h"

#include "Map.h"
#include "Player.h"
#include "Animation.h"

struct SDL_Texture;

class Scene : public SceneManager
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start(Map* map);

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt, Map* map, Player* player);

	// Called before all Updates
	bool PostUpdate(Player* player);

	/*
	// Load / Save (Underconstruction)
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	*/

	// CollisionOrb
	bool CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapOrb);

	//Called when reseting the level
	bool Reset();

	// Called before quitting
	bool CleanUp();

public:
	Animation* PillarAnim;
	Animation* CurrentAnimOrb;
	Animation* pointsAnim;

	Animation pillar;

	Animation obrN;
	Animation obrOb;

	Animation p0;
	Animation p1;

private:
	SDL_Texture* img;
	SDL_Texture* spritePillar;
	SDL_Texture* lifePlayer;
	SDL_Texture* PSup;
	SDL_Texture* spriteorb;
	SDL_Texture* pointsSprite;
	int camX;
	int camY;
public:
	bool CheckPointActive; 
	bool SoundOneTime;
	bool OrbObtained;
	bool OnlyOnceOrb;

	float counterCheckPointSound;

	iPoint Orbposition;
	iPoint PSposition;

	// Sound
	uint checkpointSound;
	uint oneupFx;
	uint coinFx;

};

#endif // __SCENE_H__