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

	//Called when reseting the level
	bool Reset();

	// Called before quitting
	bool CleanUp();

public:
	Animation* PillarAnim;

	Animation pillar;

private:
	SDL_Texture* img;
	SDL_Texture* spritePillar = nullptr;

	int camX;
	int camY;

public:

	bool NotSceneActived;

};

#endif // __SCENE_H__