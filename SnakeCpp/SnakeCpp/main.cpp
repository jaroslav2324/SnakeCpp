#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "settings.h"
#include "DrawCircle.h"
#include "Grid.h"


using std::cout;
using std::endl;



int main(int argc, char** args) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        cout << "SDL initialisation failed!" << endl;
        return 1;
    }

    const int numRows = SCREEN_HEIGHT / SNAKE_SEGMENT_WIDTH;
    const int numCols = SCREEN_WIDTH / SNAKE_SEGMENT_WIDTH;

    Grid<numRows, numCols> Grid;
    Grid.printGridTypes();

    SDL_Surface* screen_surface = NULL;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    window = SDL_CreateWindow(WINDOW_GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screen_surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    drawBresenhamCircle(renderer, 75, 50, 25, true);
    //SDL_UpdateWindowSurface(window);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

	return 0;
}