#ifndef __SCENEPAUSE_H__
#define __SCENEPAUSE_H__

#include "Module.h"

#include "GuiButton.h"

struct SDL_Texture;

class ScenePause : public Module
{
public:

	ScenePause();

	// Destructor
	virtual ~ScenePause();

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

	// Called before quitting
	bool CleanUp();

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	GuiButton* btnresume;
	GuiButton* btnsettings;
	GuiButton* btntitle;
	GuiButton* btnexit;

	Animation resume;
	Animation sett;
	Animation title;
	Animation exit;

	SDL_Texture* statesresume;
	SDL_Texture* statessett;
	SDL_Texture* statestitle;
	SDL_Texture* statesexit;
	SDL_Texture* pause;

	uint buttonFx = 0;

public:
};

#endif // __SCENEINTRO_H__
