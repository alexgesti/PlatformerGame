#ifndef __SCENEOPTIONS_H__
#define __SCENEOPTIONS_H__

#include "Module.h"

#include "Animation.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

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

	void ToggleFullscreen(SDL_Window* Window);

	// Declare on mouse click event
	bool OnGuiMouseClickEvent(GuiControl* control);

private:
	GuiSlider* sldMusic;
	GuiSlider* sldFx;
	GuiCheckBox* btnFull;
	GuiCheckBox* btnSync;
	GuiButton* btnBack;

	Animation back;
	Animation vsync;
	Animation fullscreen;

	SDL_Texture* statesGuide;
	SDL_Texture* statesVolume;
	SDL_Texture* statesMusic;
	SDL_Texture* statesFx;
	SDL_Texture* statesCheck;
	SDL_Texture* statesFullscreen;
	SDL_Texture* statesVsync;
	SDL_Texture* statesBack;

	uint buttonFx = 0;

	bool retU;

public:
	bool wait = false;
};

#endif // __SCENEINTRO_H__
