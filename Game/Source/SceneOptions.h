#ifndef __SCENEOPTIONS_H__
#define __SCENEOPTIONS_H__

#include "Module.h"

#include "GuiButton.h"
#include "GuiSlider.h"

struct SDL_Texture;

class SceneOptions : public Module
{
public:

	SceneOptions();

	// Destructor
	virtual ~SceneOptions();

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
	GuiSlider* sldMusic;
	GuiSlider* sldFx;
	GuiButton* btnFull;
	GuiButton* btnSync;
	GuiButton* btnExit;

	Animation fullscreen;
	Animation vsync;
	Animation exit;

	SDL_Texture* statesFullscreen;
	SDL_Texture* statesVsync;
	SDL_Texture* statesExit;
	bool retU;

public:
};

#endif // __SCENEINTRO_H__
