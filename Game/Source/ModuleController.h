#ifndef __MODULECONTROLLER_H__
#define __MODULECONTROLLER_H__

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"

class ModuleController : public Module
{
public:

	ModuleController();

	// Destructor
	virtual ~ModuleController();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	// Add a new module to handle
	void AddModule(Module* module);

	//	Called everytime 1 screen changes
	bool CleanUp();

public:
	bool capped = false;
	bool showcollider = false;
	bool showbuttons = false;

	int currentscene;

private: 
	List<Module*> modules;

	SDL_Color blue = { 153, 225, 204 };
	SDL_Color black = { 0, 0, 0 };
};

#endif // __MODULECONTROLLER_H__
