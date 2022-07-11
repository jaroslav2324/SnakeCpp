#pragma once

#include <iostream>
#include <string>

#include "SDL.h"
#include "SDL_image.h"


using std::cout;
using std::endl;

class Widget {
public:
	Widget(SDL_Renderer* ren, const SDL_Rect positionRect,  const std::string imagePath);
	~Widget();

	void renderWidget(SDL_Renderer* ren);

protected:
	// {posX, posY, width, height}
	SDL_Rect position;

	SDL_Texture* image = nullptr;
};


Widget::Widget(SDL_Renderer* ren, const SDL_Rect position, const std::string imagePath) {

	Widget::position = position;

	//load image and create temporary surface
	SDL_Surface* tempSurface = nullptr;
	tempSurface = IMG_Load(imagePath.c_str());

	if (tempSurface == nullptr) {
		cout << "No image was loaded for '" << imagePath << "'" << endl;
	}

	//convert surface to texture
	image = SDL_CreateTextureFromSurface(ren, tempSurface);

	renderWidget(ren);
}

Widget::~Widget() {

}

void Widget::renderWidget(SDL_Renderer* ren) {


	SDL_RenderCopy(ren, image, NULL, &position);
}

