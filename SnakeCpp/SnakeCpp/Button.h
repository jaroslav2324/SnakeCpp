#pragma once

#include <functional>

#include "Widget.h"

class Button final: public Widget {
public:
	Button(SDL_Renderer* ren, const SDL_Rect positionRect, const std::string imagePath, std::function<void()> doFunction) : Widget(ren, positionRect, imagePath) {
	
		doFuncOnButtonPressed = doFunction;
	};
	~Button() {};

	void onButtonPressed(int mousePosX, int mousePosY);

private:
	bool ifButtonPressed(int mousePosX, int mousePosY);
	std::function<void()> doFuncOnButtonPressed;

};

bool Button::ifButtonPressed(int mousePosX, int mousePosY) {

	if (mousePosX >= position.x and mousePosY >= position.y
		and mousePosX <= position.x + position.w and mousePosY <= position.y + position.h)
		return true;

	return false;
}

void Button::onButtonPressed(int mousePosX, int mousePosY) {

	if (ifButtonPressed(mousePosX, mousePosY))
		doFuncOnButtonPressed();

}