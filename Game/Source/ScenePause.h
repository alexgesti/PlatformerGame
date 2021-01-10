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
	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnTitle;
	GuiButton* btnExit;

	Animation resume;
	Animation sett;
	Animation title;
	Animation exit;

	SDL_Texture* statesResume;
	SDL_Texture* statesSett;
	SDL_Texture* statesTitle;
	SDL_Texture* statesExit;
	SDL_Texture* pause;

	bool retU;

public:
};

#endif // __SCENEINTRO_H__
