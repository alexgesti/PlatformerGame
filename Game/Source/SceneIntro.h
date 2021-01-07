#ifndef __SCENEINTRO_H__
#define __SCENEINTRO_H__

#include "Module.h"

//#include "GuiButton.h"

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
	//bool OnGuiMouseClickEvent(GuiControl* control);

private:
	SDL_Texture* SpriteIntro;
	SDL_Texture* SpaceStart;

public:
	bool OneTimeOnly;

	//GuiButton* btnStart; //CREACION DEL BOTON
	//GuiButton* btnExit; //CREACION DEL BOTON

};

#endif // __SCENEINTRO_H__
