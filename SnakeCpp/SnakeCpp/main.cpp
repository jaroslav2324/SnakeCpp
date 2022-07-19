#include <iostream>

#include <SDL.h>
#include <SDL_image.h>

#include "settings.h"
#include "DrawCircle.h"
#include "Grid.h"
#include "Menu.h"
#include "MenuSettings.h"
#include "Timer.h"


using std::cout;
using std::endl;



int main(int argc, char** args) {

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0){

        cout << "SDL initialisation failed!" << endl;
        return 1;
    }

    const int numRows = (SCREEN_HEIGHT - SNAKE_SEGMENT_WIDTH / 2) / SNAKE_SEGMENT_WIDTH;
    const int numCols = (SCREEN_WIDTH - SNAKE_SEGMENT_WIDTH / 2) / SNAKE_SEGMENT_WIDTH;

    //TODO fix bresenham circle render to avoid rendering one pixel down and right

    SDL_Surface* screen_surface = NULL;
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    window = SDL_CreateWindow(WINDOW_GAME_NAME, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    screen_surface = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // menu 
    Menu menu(renderer); 
    MenuSettings settingsMenu(renderer);

    /*add timer*/
    double prevTime = 0, currentTime = 0, deltaTime = 0, frameTime = 0;

    while(true){

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        Timer FPS_Timer(1 / FPS);

        //menu is opened
        while (true) {
            

            if (FPS_Timer.tickIfNeeded()) {

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
                            if (settingsMenu.settingsMenuActive)
                                settingsMenu.checkButtonsPressed(mousePosX, mousePosY);
                            else
                                menu.checkButtonsPressed(mousePosX, mousePosY);
                        }
                    }
                }
                if (menu.activateSettingsMenu == true) {
                    menu.activateSettingsMenu = false;
                    settingsMenu.settingsMenuActive = true;
                }

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);

                //checking hovering over buttons
                int mousePosX, mousePosY;
                SDL_GetMouseState(&mousePosX, &mousePosY);

                if (settingsMenu.settingsMenuActive) {
                    settingsMenu.checkHoveringOverButtons(mousePosX, mousePosY);
                    settingsMenu.renderMenu(renderer);
                }
                else {
                    menu.checkHoveringOverButtons(mousePosX, mousePosY);
                    menu.renderMenu(renderer);
                }

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

        // game difficulty
        double numSecsInEveryMove;
        int difficulty = settingsMenu.getDifficulty();

        /*calculate period of snake moving*/
        switch (difficulty) {
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

        Timer snakeMovementTimer(numSecsInEveryMove * 1000);
        Grid<numRows, numCols> Grid;

        //game is running
        Grid.renderAll(renderer);

        while (true) {

            if (snakeMovementTimer.tickIfNeeded()) {
                
                int returnCode = Grid.moveSnake();

                Grid.renderAll(renderer);

                SDL_RenderPresent(renderer);

                if (returnCode == EXIT_GAME_CODE) {
                    menu.gameRunning = false;
                    break;
                }
            }

            else if (FPS_Timer.tickIfNeeded()) {
                Grid.renderAll(renderer);
                SDL_RenderPresent(renderer);
            }
        }
    }
    

    SDL_Quit();
    IMG_Quit();

	return 0;
}