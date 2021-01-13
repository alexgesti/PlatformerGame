#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"

#include "Animation.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Load / Save (Underconstruction)
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;

	// CollisionOrb
	bool CheckCollisionRec(iPoint positionMapPlayer, iPoint positionMapOrb);

	//Called when reseting the level
	bool Reset();

	// Called before quitting
	bool CleanUp();

public:
	Animation* pillaranim;
	Animation* currentanimorb;
	Animation* currentanimcollorb;
	Animation* currentanimcolllife;

	Animation pillar;

	Animation obrn;
	Animation obrob;

	Animation p0;
	Animation p1;

	Animation collisionorb;
	Animation collisionlife;

private:
	SDL_Texture* img;
	SDL_Texture* spritepillar;
	SDL_Texture* lifeplayer;
	SDL_Texture* psup;
	SDL_Texture* spriteorb;
	SDL_Texture* collision = nullptr;

	int camX;
	int camY;

public:

	bool notsceneactived;
	bool checkpointactive; 
	bool soundonetime;
	bool orbobtained;
	bool onlyonceorb;

	float countercheckpointsound;

	iPoint orbposition;
	iPoint psposition;

	// Checkpoint
	iPoint poscheck1 = { 2057, 1905 };
	iPoint poscheck2 = { 4552, 1008 };
	iPoint poscheck3 = { 6472, 624 };

	// Sound
	uint checkpointsound;
	uint oneupfx;
	uint coinfx;

};

#endif // __SCENE_H__