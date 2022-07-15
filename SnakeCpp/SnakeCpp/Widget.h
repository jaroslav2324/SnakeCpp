#pragma once

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"


using std::cout;
using std::endl;

class Widget {
public:
	Widget(SDL_Renderer* ren, const SDL_Rect positionRect,  const std::string imagePath, const std::string onHoverImagePath = "");
	~Widget();

	void renderWidget(SDL_Renderer* ren);

	bool cursorHoveredOver = false;

protected:
	// {posX, posY, width, height}
	SDL_Rect position;

	SDL_Texture* image = nullptr;
	SDL_Texture* imageOnHover = nullptr;
};


Widget::Widget(SDL_Renderer* ren, const SDL_Rect position, const std::string imagePath, const std::string onHoverImagePath) {

	Widget::position = position;

	//load image and create temporary surface
	SDL_Surface* tempSurface = nullptr;
	tempSurface = IMG_Load(imagePath.c_str());

	if (tempSurface == nullptr) {
		cout << "No image was loaded for '" << imagePath << "'" << endl;
	}

	//convert surface to texture
	image = SDL_CreateTextureFromSurface(ren, tempSurface);

	// load on hover image and create temporary surface
	if (onHoverImagePath != "") {

		tempSurface = IMG_Load(onHoverImagePath.c_str());

		if (tempSurface == nullptr) {
			cout << "No 'on hover' image was loaded for '" << imagePath << "'" << endl;
		}
	
		imageOnHover = SDL_CreateTextureFromSurface(ren, tempSurface);
	}

	renderWidget(ren);
}

Widget::~Widget() {

}

void Widget::renderWidget(SDL_Renderer* ren) {

	if (cursorHoveredOver and imageOnHover != nullptr)
		SDL_RenderCopy(ren, imageOnHover, NULL, &position);
	else
		SDL_RenderCopy(ren, image, NULL, &position);
}

