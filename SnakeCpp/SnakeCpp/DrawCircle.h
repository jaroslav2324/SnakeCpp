#include "SDL.h"


/*drawing circle using Bresenham algorithm*/
void drawBresenhamCircle(SDL_Renderer* renderer, int centereX, int centereY, int radius, bool fill = false) {
	
	// start point on the top of the circle
	int x = centereX, y = centereY - radius;
	int functionVal1, functionVal2, functionVal3; // x^2 + y^2 - R^2


	while (x - centereX <= radius && y - centereY <= 0){
		// mirroring points
		// first coord quarter
		SDL_RenderDrawPoint(renderer, x, y);
		// second coord quarter
		SDL_RenderDrawPoint(renderer, (-(x - centereX)) + centereX, y);
		// third coord quarter 
		SDL_RenderDrawPoint(renderer, (-(x - centereX)) + centereX, (-(y - centereY)) + centereY);
		// fourth coord quarter
		SDL_RenderDrawPoint(renderer, x, (-(y - centereY)) + centereY);

		// filling
		// draw lines between points
		if (fill) {
			SDL_RenderDrawLine(renderer, x, y, (-(x - centereX)) + centereX, y);
			SDL_RenderDrawLine(renderer, (-(x - centereX)) + centereX, (-(y - centereY)) + centereY, x, (-(y - centereY)) + centereY);
		}

		// (x + 1; y)
		functionVal1 = (int)abs(pow(x - centereX + 1, 2) + pow(y - centereY, 2) - pow(radius, 2));
		// (x + 1; y + 1)
		functionVal2 = (int)abs(pow(x - centereX + 1, 2) + pow(y - centereY + 1, 2) - pow(radius, 2));
		// (x; y + 1)
		functionVal3 = (int)abs(pow(x - centereX, 2) + pow(y - centereY + 1, 2) - pow(radius, 2));

		// choose min difference between function and one of the points
		if (functionVal1 < functionVal2 && functionVal1 < functionVal3) {
			++x;
		}
		else if (functionVal3 < functionVal1 && functionVal3 < functionVal2) {
			++y;
		}
		else {
			++x;
			++y;
		}
	}

}