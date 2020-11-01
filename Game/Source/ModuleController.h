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
	 
	// Load / Save (Underconstruction)
	//bool LoadState(pugi::xml_node&);
	//bool SaveState(pugi::xml_node&) const;

	// Add a new module to handle
	void AddModule(Module* module);

	//	Called everytime 1 screen changes
	bool CleanUp();

public:

	int currentscene;

private: 

	List<Module*> modules;

};

#endif // __MODULECONTROLLER_H__
