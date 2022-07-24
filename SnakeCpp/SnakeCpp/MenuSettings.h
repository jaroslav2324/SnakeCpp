

#include "SDL.h"

#include "settings.h"
#include "Widget.h"
#include "Button.h"
#include "MenuTemplate.h"

class MenuSettings : public MenuTemplate {
public:
	MenuSettings(SDL_Renderer* ren);
	~MenuSettings();

	void renderMenu(SDL_Renderer* ren);

	int getDifficulty();

	bool settingsMenuActive = false;

protected:
	int difficulty = HARD;
	Widget* diffifcultyWidget;
};

MenuSettings::MenuSettings(SDL_Renderer* ren) {

	// create buttons and widgets

	int screenCenterX = SCREEN_WIDTH / 2;
	int screenCenterY = SCREEN_HEIGHT / 2;

	int btnWidth = SCREEN_WIDTH / 2;
	int btnHeight = SCREEN_HEIGHT / 6;

	int backBtnHeight = SCREEN_HEIGHT / 12;
	int backBtnWidth = SCREEN_WIDTH / 12;

	// 500 for buttons and widgets, 20 for padding, 5 buttons and 4 intervals between them
	int differenceBetweenButtons = (SCREEN_HEIGHT - 500 - 20) / 4;

	// same for all buttons
	int btnPosX = screenCenterX - btnWidth / 2;

	// create widget
	SDL_Rect backButtonPos = { 10, 10, backBtnWidth, backBtnHeight };
	Button backBtn(ren, backButtonPos, "images/backButton.png", "images/backButtonHovered.png", [this]() { settingsMenuActive = false; });
	
	int btnPosY = 10;

	// create all buttons
	SDL_Rect difficultyWidgetPos = { btnPosX,  btnPosY, btnWidth, btnHeight};
	Widget* difficultyWidget = new Widget(ren, difficultyWidgetPos, "images/difficultyWidget.png");
	MenuSettings::diffifcultyWidget = difficultyWidget;

	btnPosY += btnHeight + differenceBetweenButtons;
	SDL_Rect easyBtnPos = { btnPosX, btnPosY, btnWidth, btnHeight };
	Button easyBtn(ren, easyBtnPos, "images/easyButton.png", "images/easyButtonHovered.png", [this]() {difficulty = EASY;
																									   settingsMenuActive = false; });

	btnPosY += btnHeight + differenceBetweenButtons;
	SDL_Rect mediumBtnPos = { btnPosX, btnPosY, btnWidth, btnHeight };
	Button mediumBtn(ren, mediumBtnPos, "images/mediumButton.png", "images/mediumButtonHovered.png", [this]() {difficulty = MEDIUM;
																											   settingsMenuActive = false;  });

	btnPosY += btnHeight + differenceBetweenButtons;
	SDL_Rect hardBtnPos = { btnPosX, btnPosY, btnWidth, btnHeight };
	Button hardBtn(ren, hardBtnPos, "images/hardButton.png", "images/hardButtonHovered.png", [this]() {difficulty = HARD;
																									   settingsMenuActive = false;  });

	btnPosY += btnHeight + differenceBetweenButtons;
	SDL_Rect impossibleBtnPos = { btnPosX, btnPosY, btnWidth, btnHeight };
	Button impossibleBtn(ren, impossibleBtnPos, "images/impossibleButton.png", "images/impossibleButtonHovered.png", [this]() {difficulty = IMPOSSIBLE;
																															   settingsMenuActive = false;  });

	buttonsList.push_back(backBtn);
	buttonsList.push_back(easyBtn);
	buttonsList.push_back(mediumBtn);
	buttonsList.push_back(hardBtn);
	buttonsList.push_back(impossibleBtn);
}


MenuSettings::~MenuSettings() {
	delete diffifcultyWidget;
}

void MenuSettings::renderMenu(SDL_Renderer* ren) {

	diffifcultyWidget->renderWidget(ren);
	MenuTemplate::renderMenu(ren);
}


int MenuSettings::getDifficulty() {

	return difficulty;
}