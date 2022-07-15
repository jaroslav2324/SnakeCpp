#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "settings.h"
#include "DrawCircle.h"
#include "Grid.h"
#include "Menu.h"


using std::cout;
using std::endl;



int main(int argc, char** args) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){

        cout << "SDL initialisation failed!" << endl;
        return 1;
    }

    const int numRows = (SCREEN_HEIGHT - SNAKE_SEGMENT_WIDTH / 2) / SNAKE_SEGMENT_WIDTH;
    const int numCols = (SCREEN_WIDTH - SNAKE_SEGMENT_WIDTH / 2) / SNAKE_SEGMENT_WIDTH;


    /*calculate period of snake moving*/
    double numSecsInEveryMove;

    // game difficulty
    int difficulty = MEDIUM;

    switch(difficulty){
        case EASY:
            numSecsInEveryMove = 1;
            break;
        case MEDIUM:
            numSecsInEveryMove = (double)1 / 2;
            break;
        case HARD:
            numSecsInEveryMove = (double)1 / 3;
            break;
        case IMPOSSIBLE:
            numSecsInEveryMove = (double)1 / 4;
            break;
    }

    //TODO fix bresenham circle render to avoid rendering one pixel down and right

    Grid<numRows, numCols> Grid;
    

    SDL_Surface* screen_surface = NULL;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    window = SDL_CreateWindow(WINDOW_GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screen_surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // menu 
    Menu menu(renderer);

    /*add timer*/
    double prevTime = 0, currentTime = 0, deltaTime = 0, frameTime = 0;

    while(true){

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        //menu is opened
        while (true) {
            prevTime = currentTime;
            currentTime = SDL_GetTicks();

            deltaTime = (double)(currentTime - prevTime) / 1000; // in seconds

            frameTime += deltaTime;

            if (frameTime > numSecsInEveryMove) {
                frameTime -= numSecsInEveryMove;



                // handle mouse clicks
                SDL_Event event;

                bool quit = 0;

                while (SDL_PollEvent(&event)) {

                    if (event.type == SDL_QUIT) {
                        quit = 1;
                        break;
                    }

                    if (event.type == SDL_MOUSEBUTTONDOWN) {

                        if (event.button.button == SDL_BUTTON_LEFT) {

                            int mousePosX, mousePosY;

                            SDL_GetMouseState(&mousePosX, &mousePosY);

                            //check pressing on buttons
                            menu.checkButtonsPressed(mousePosX, mousePosY);
                        }
                    }
                }

                //checking hovering over buttons
                int mousePosX, mousePosY;

                SDL_GetMouseState(&mousePosX, &mousePosY);
                menu.checkHoveringOverButtons(mousePosX, mousePosY);

                menu.renderMenu(renderer);
                SDL_RenderPresent(renderer);

                // exit
                if (quit == true) {
                    SDL_Quit();
                    exit(0);
                }
                    
                if (menu.gameRunning == true)
                    break;
            }
        }

        //game is running
        while (true) {
            prevTime = currentTime;
            currentTime = SDL_GetTicks();

            deltaTime = (double)(currentTime - prevTime) / 1000; // in seconds

            frameTime += deltaTime;

            if (frameTime > numSecsInEveryMove) {
                frameTime -= numSecsInEveryMove;

                int returnCode = Grid.moveSnake();

                Grid.renderAll(renderer);

                SDL_RenderPresent(renderer);

                if (returnCode == EXIT_GAME_CODE) {
                    menu.gameRunning = false;
                    break;
                }
            }
        }
    }
    

    SDL_Quit();
    IMG_Quit();

	return 0;
}