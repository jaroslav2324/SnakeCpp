#pragma once

#include <functional>

#include "Widget.h"

class Button final: public Widget {
public:
	Button(SDL_Renderer* ren, const SDL_Rect positionRect, const std::string imagePath, const std::string onHoverImagePath, std::function<void()> doFunction) : Widget(ren, positionRect, imagePath, onHoverImagePath) {
	
		doFuncOnButtonPressed = doFunction;
	};
	~Button() {};

	void onButtonPressed(int mousePosX, int mousePosY);
	void onButtonHoveredOver(int mousePosX, int mousePosY);

private:
	bool ifMouseInButtonRect(int mousePosX, int mousePosY);
	std::function<void()> doFuncOnButtonPressed;

};

bool Button::ifMouseInButtonRect(int mousePosX, int mousePosY) {

	if (mousePosX >= position.x and mousePosY >= position.y
		and mousePosX <= position.x + position.w and mousePosY <= position.y + position.h)
		return true;

	return false;
}

void Button::onButtonPressed(int mousePosX, int mousePosY) {

	if (ifMouseInButtonRect(mousePosX, mousePosY))
		doFuncOnButtonPressed();

}

void Button::onButtonHoveredOver(int mousePosX, int mousePosY) {

	if (ifMouseInButtonRect(mousePosX, mousePosY))
		cursorHoveredOver = true;
	else
		cursorHoveredOver = false;
}