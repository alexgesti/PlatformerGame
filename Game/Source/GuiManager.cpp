#include "GuiManager.h"

#include "GuiButton.h"

GuiControl* GuiManager::CreateGuiControl(GuiControlType type)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		// Create the corresponding GuiControl type
		//case GuiControlType::BUTTON: control = new GuiButton(1234);  break;
	default: break;
	}

	// Created entities are added to the list
	if (control != nullptr) controls.add(control);

	return control;
}

bool GuiManager::Update(float dt)
{
	accumulatedtime += dt;
	if (accumulatedtime >= updatemscycle) dologic = true;

	//UpdateAll(dt, doLogic); //DESMUTEAR PARA TESTEAR

	if (dologic == true)
	{
		accumulatedtime = 0.0f;
		dologic = false;
	}

	return true;
}