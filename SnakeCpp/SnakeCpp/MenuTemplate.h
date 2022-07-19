#pragma once

#include <vector>

#include "SDL.h"

#include "Button.h"


/*MenuTemplate class without buttons, buttons must be added in derived classes*/
class MenuTemplate {

public:
	MenuTemplate() {};
	~MenuTemplate() {};
	virtual void renderMenu(SDL_Renderer* ren);

	virtual void checkButtonsPressed(int mouseposX, int mouseposY);
	virtual void checkHoveringOverButtons(int mousePosX, int mousePosY) ;

protected:
	std::vector<Button> buttonsList;
};

void MenuTemplate::renderMenu(SDL_Renderer* ren) {

	for (auto& button : buttonsList)
		button.renderWidget(ren);
	
}


void MenuTemplate::checkButtonsPressed(int mousePosX, int mousePosY) {

	for (auto& button : buttonsList)
		button.onButtonPressed(mousePosX, mousePosY);

}

void MenuTemplate::checkHoveringOverButtons(int mousePosX, int mousePosY) {

	for (auto& button : buttonsList)
		button.onButtonHoveredOver(mousePosX, mousePosY);

}