#ifndef __GUIMANAGER_H__
#define __GUIMANAGER_H__

#include "Module.h"
#include "GuiControl.h"

#include "List.h"

class GuiManager : public Module
{
public:

	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type);
	void DestroyGuiControl(GuiControl* entity);

	void AddGuiControl(GuiControl* entity);

public:

	List<GuiControl*> controls;

	float accumulatedtime = 0.0f;
	float updatemscycle = 0.0f;
	bool dologic = false;
};

#endif // __GUIMANAGER_H__
