#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"

#include "GuiButton.h"

struct SDL_Texture;

class SceneIntro : public Module
{
public:

	SceneIntro();

	// Destructor
	virtual ~SceneIntro();

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
	SDL_Texture* spriteintro;

	GuiButton* btnstart;
	GuiButton* btncontinue;
	GuiButton* btnsettings;
	GuiButton* btncredits;
	GuiButton* btnexit;

	Animation play;
	Animation cont;
	Animation sett;
	Animation credits;
	Animation exit;

	SDL_Texture* statesplay;
	SDL_Texture* statescont;
	SDL_Texture* statessett;
	SDL_Texture* statescredits;
	SDL_Texture* statesexit;
	SDL_Texture* creditstext;

	bool creditsbut;

	uint buttonfx = 0;

public:
	bool onetimeonly;
};

#endif // __SCENEINTRO_H__
