#include <vector>

#include <SDL.h>

#include "settings.h"
#include "Button.h"


class Menu {

public: 
	Menu(SDL_Renderer* ren);
	~Menu() {};
	void renderMenu(SDL_Renderer* ren);
	void quitGame();

	//if true game is running otherwise menu is shown;
	bool gameStarted;

private:
	std::vector<Button> buttonsList;

};

Menu::Menu(SDL_Renderer* ren) {

	gameStarted = false;

	int screenCenterX = SCREEN_WIDTH / 2;
	int screenCenterY = SCREEN_HEIGHT / 2;

	int btnWidth = SCREEN_WIDTH / 2;
	int btnHeight = SCREEN_HEIGHT / 6;

	// same for all buttons
	int btnPosX = screenCenterX - btnWidth / 2;

	// create buttons

	int playBtnPosY = screenCenterY - 2 * btnHeight;

	SDL_Rect playBtnPos = {btnPosX,  playBtnPosY, btnWidth, btnHeight};
	Button playBtn(ren, playBtnPos, "playButton.png", [this]() {gameStarted = true; });

	SDL_Rect optionsBtnPos = {btnPosX, screenCenterY - btnHeight / 2, btnWidth, btnHeight};
	//TODO openOptions
	Button optionsBtn(ren, optionsBtnPos, "optionsButton.png", [this]() {; });

	int quitBtnPosY = screenCenterY + btnHeight;

	SDL_Rect quitBtnPos = {btnPosX, quitBtnPosY, btnWidth, btnHeight };
	Button quitBtn(ren, quitBtnPos, "quitButton.png", [this](){quitGame(); });

	// render buttons
	renderMenu(ren);
}

void Menu::renderMenu(SDL_Renderer* ren) {

	for (auto button : buttonsList) {
		button.renderWidget(ren);
	}
}

void Menu::quitGame() {

	SDL_Quit();
	exit(0);
}