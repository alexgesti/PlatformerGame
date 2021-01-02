#ifndef _SCENEBASE_H_
#define _SCENEBASE_H_

#include "Module.h"

#include "SString.h"

class Input;
class Render;
class Textures;

class GuiControl;

enum class SceneType
{
    LOGO,
    TITLE,
    GAMEPLAY,
    ENDING
};

class SceneBase : public Module
{
public:

    SceneBase() : active(true), loaded(false), transitionRequired(false) {}

    virtual bool Start()
    {
        return true;
    }

    virtual bool Update(float dt)
    {
        Input* input;
        return true; //Input* input, mirar si se necesita. No se porque pero me habia dado problemas.
    }

    virtual bool PostUpdate()
    {
        return true;
    }

    void TransitionToScene(SceneType scene)
    {
        transitionRequired = true;
        nextScene = scene;
    }

    // Define multiple Gui Event methods
    virtual bool OnGuiMouseClickEvent(GuiControl* control)
    {
        return true;
    }

public:

    bool active = true;
    SString name;         // Scene name identifier?

    bool loaded = false;
    // Posiblemente necesitemos más propiedades para las transciones

    bool transitionRequired;
    SceneType nextScene;
};

#endif
