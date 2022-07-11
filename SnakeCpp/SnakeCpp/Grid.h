#include <list>
#include <utility>
#include <random>
#include <iostream>

#include "settings.h"
#include "Snake.h"

using std::cout;
using std::endl;

template <int numTiles>
class GridRow {
public:
	GridRow(bool wallRow = false);
	~GridRow();
	TILE_TYPE& operator[](int index);

private:
	TILE_TYPE row[numTiles];
};

template <int numRows, int numCols>
class Grid {

public:
	Grid();
	~Grid();

	void renderAll(SDL_Renderer* ren);
	
	//makeGameTurn(move snake)
	void moveSnake();
	
	// debug
	void printGridTypes();
	void renderGrid(SDL_Renderer* ren);

private:
	int pixelsToGridCoord(int pixels);
	int gridCoordToPixels(int gridCoord);

	void resetGridTilesTypes();
	void placeSnakeOnTiles();
	void generateFood();

	void renderGridTiles(SDL_Renderer* ren);
	void renderSnake(SDL_Renderer* ren);
	void renderFood(SDL_Renderer* ren);

	bool ifTileWall(int indexX, int indexY);
	bool ifTileSnake(int indexX, int indexY);
	bool ifTileFood(int indexX, int indexY);
	bool ifTileEmpty(int indexX, int indexY);
	bool ifTileHeadSnake(int indexX, int indexY);
	bool ifAllTilesSnake(); // amazing player

	bool ifFoodConsumed(char direction);
	bool ifSnakeDies(char direction);

	GridRow<numCols>& operator[](int numRow);

	GridRow<numCols> grid[numRows];

	Snake<int> snake;
	std::pair<int, int> foodPosition;
};

template <int numRows, int numCols>
Grid<numRows, numCols>::Grid() {

	//fill grid types
	grid[0] = GridRow<numCols>(true);
	for (int i = 1; i < numRows - 1; i++)
		grid[i] = GridRow<numCols>();
	grid[numRows - 1] = GridRow<numCols>(true);

	//add snake head
	int snakeStartPosX = (SCREEN_WIDTH / SNAKE_SEGMENT_WIDTH) / 2;
	int snakeStartPosY = (SCREEN_HEIGHT / SNAKE_SEGMENT_WIDTH) / 2;

	snake.addSegmentCoords(snakeStartPosX, snakeStartPosY);

	// generate food
	generateFood();

}

template <int numRows, int numCols>
Grid<numRows, numCols>::~Grid() {

}

template <int numRows, int numCols>
GridRow<numCols>& Grid<numRows, numCols>::operator[](int numRow) {
	if (numRow < 0 || numRow >= numRows)
		throw std::exception("Array index out of bounds in Grid[]");
	return *grid[numRow];
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::resetGridTilesTypes() {

	for (int i = 0; i < numCols; i++)
		grid[0][i] = WALL_TILE;

	for (int j = 1; j < numRows - 1; j++) {
		for (int i = 0; i < numCols; i++)
			grid[j][i] = EMPTY_TILE;
		grid[j][0] = WALL_TILE;
		grid[j][numCols - 1] = WALL_TILE;
	}

	for (int i = 0; i < numCols; i++)
		grid[numCols - 1][i] = WALL_TILE;
}

/*coord to center of pixel rectangle*/
template <int numRows, int numCols>
int Grid<numRows, numCols>::gridCoordToPixels(int gridCoord) {
	return (gridCoord * SNAKE_SEGMENT_WIDTH) + SNAKE_SEGMENT_WIDTH / 2;
}

/*center of pixel rectangle to coord*/
template <int numRows, int numCols>
int Grid<numRows, numCols>::pixelsToGridCoord(int pixels) {
	return (pixels - SNAKE_SEGMENT_WIDTH / 2) / SNAKE_SEGMENT_WIDTH;
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::printGridTypes() {
	for (int j = 0; j < numRows; j++) {
		for (int i = 0; i < numCols; i++)
			std::cout << grid[i][j] << ' ';
			std::cout << std::endl;
	}
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileWall(int indexX, int indexY) {

	if (grid[indexX][indexY] == WALL_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileSnake(int indexX, int indexY) {

	if (grid[indexX][indexY] == SNAKE_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileFood(int indexX, int indexY) {

	if (grid[indexX][indexY] == FOOD_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileEmpty(int indexX, int indexY) {

	if (grid[indexX][indexY] == EMPTY_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileHeadSnake(int indexX, int indexY) {

	if (grid[indexX][indexY] == SNAKE_HEAD_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifAllTilesSnake() {

	for (int i = 1; i < numRows - 1; i++)
		for (int j = 1; j < numCols - 1; j++)
			if (not (grid[i][j] == SNAKE_TILE or grid[i][j] == SNAKE_HEAD_TILE))
				return false;

	return true;
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::generateFood() {

	srand(time(NULL));

	/*generate random position on the grid*/
	int numRandomRow = rand() % numRows;
	int numRandomCol = rand() % numCols;

	while (grid[numRandomRow][numRandomCol] != EMPTY_TILE){
		numRandomRow = rand() % numRows;
		numRandomCol = rand() % numCols;
	}

	//clear grid
	resetGridTilesTypes();

	//placeSnake
	placeSnakeOnTiles();

	//place food
	grid[numRandomRow][numRandomCol] = FOOD_TILE;

	foodPosition = std::make_pair(numRandomRow, numRandomCol);
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::renderGrid(SDL_Renderer* ren) {

	SDL_SetRenderDrawColor(ren, 120, 120, 120, 255);
	int numR, numC;
	numR = numRows;
	numC = numCols;

	for (int i = -1; i <= numRows; i++) {
		int x1, x2, y1, y2;
		x1 = gridCoordToPixels(-1);
		x2 = gridCoordToPixels(numC);
		y1 = gridCoordToPixels(i);
		y2 = y1;
		SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	}

	for (int i = -1; i <= numCols; i++) {
		int x1, x2, y1, y2;
		y1 = gridCoordToPixels(-1);
		y2 = gridCoordToPixels(numR);
		x1 = gridCoordToPixels(i);
		x2 = x1;
		SDL_RenderDrawLine(ren, x1, y1, x2, y2);
	}
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::renderGridTiles(SDL_Renderer* ren) {

	SDL_Rect rect;

	rect.w = SNAKE_SEGMENT_WIDTH;
	rect.h = SNAKE_SEGMENT_WIDTH;

	// render grey wall and black empty tiles
	for (int indexRow = 0; indexRow < numRows; indexRow++)
		for (int indexCol = 0; indexCol < numCols; indexCol++) {

			if(ifTileWall(indexRow, indexCol))
				SDL_SetRenderDrawColor(ren, 120, 120, 120, 255);
			else
				SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

			rect.x = gridCoordToPixels(indexRow) - SNAKE_SEGMENT_WIDTH / 2;
			rect.y = gridCoordToPixels(indexCol) - SNAKE_SEGMENT_WIDTH / 2;

			SDL_RenderDrawRect(ren, &rect);
			SDL_RenderFillRect(ren, &rect);
		}
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::renderAll(SDL_Renderer* ren) {

	renderGridTiles(ren);
	renderSnake(ren);
	renderFood(ren);
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::renderSnake(SDL_Renderer* ren) {

	int x, y;

	SDL_SetRenderDrawColor(ren, 0, 200, 0, 255);

	for (auto iterSegment : snake.listSegmentCoords) {
		x = gridCoordToPixels(iterSegment.first) - 1;
		y = gridCoordToPixels(iterSegment.second) - 1;
		drawBresenhamCircle(ren, x, y, SNAKE_SEGMENT_WIDTH / 2, true);
	}
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::renderFood(SDL_Renderer* ren) {

	int coordX = gridCoordToPixels(foodPosition.first);
	int coordY = gridCoordToPixels(foodPosition.second);

	SDL_SetRenderDrawColor(ren, 255, 255, 0, 255);

	drawBresenhamCircle(ren, coordX, coordY, SNAKE_SEGMENT_WIDTH / 2, true);
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::placeSnakeOnTiles() {

	for (auto iterSegment : snake.listSegmentCoords)
		grid[iterSegment.first][iterSegment.second] = SNAKE_TILE;

	auto headCoords = snake.listSegmentCoords.begin();
	grid[headCoords->first][headCoords->second] = SNAKE_HEAD_TILE;
}

// check tile where head will be in the next turn
template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifFoodConsumed(char direction) {

	auto headCoords = snake.listSegmentCoords.begin();
	switch (direction) {
	case UP:
		if (ifTileFood(headCoords->first, headCoords->second - 1))
			return true;
		break;

	case DOWN:
		if (ifTileFood(headCoords->first, headCoords->second + 1))
			return true;
		break;

	case RIGHT:

		if (ifTileFood(headCoords->first + 1, headCoords->second))
			return true;
		break;

	case LEFT:

		if (ifTileFood(headCoords->first - 1, headCoords->second))
			return true;
		break;
	}

	return false;
}

// check tile where head will be in the next turn
template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifSnakeDies(char direction) {

	auto headCoords = snake.listSegmentCoords.front();
	auto tailCoords = snake.listSegmentCoords.back();
	switch (direction) {
	case UP:
		// if snake bumps into itself or into wall, checking if snake tail manages to leave
		if ( not (ifTileEmpty(headCoords.first, headCoords.second - 1) or ifTileFood(headCoords.first, headCoords.second - 1))
			and (tailCoords.first != headCoords.first or tailCoords.second != headCoords.second - 1)) {
			return true;
			// debug
			cout << "Snake dies moving UP!" << endl;
		}
		break;

	case DOWN:
		// if snake bumps into itself or into wall, checking if snake tail manages to leave
		if ( not (ifTileEmpty(headCoords.first, headCoords.second + 1) or ifTileFood(headCoords.first, headCoords.second + 1))
			and (tailCoords.first != headCoords.first or tailCoords.second != headCoords.second + 1)) {
			return true;
			// debug
			cout << "Snake dies moving DOWN!" << endl;
		}
		break;

	case RIGHT:
		// if snake bumps into itself or into wall, checking if snake tail manages to leave
		if ( not (ifTileEmpty(headCoords.first + 1, headCoords.second) or ifTileFood(headCoords.first + 1, headCoords.second))
			and (tailCoords.first != headCoords.first + 1 or tailCoords.second != headCoords.second)) {
			return true;
			// debug
			cout << "Snake dies moving RIGHT!" << endl;
		}
		break;

	case LEFT:
		// if snake bumps into itself or into wall, checking if snake tail manages to leave
		if (not (ifTileEmpty(headCoords.first - 1, headCoords.second) or ifTileFood(headCoords.first - 1, headCoords.second))
			and (tailCoords.first != headCoords.first - 1 or tailCoords.second != headCoords.second)) {
			return true;
			// debug
			cout << "Snake dies moving LEFT!" << endl;
		}
		break;
	}

	return false;
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::moveSnake() {

	//choose direction
	SDL_Event event;
	char direction = -1;
	bool quit = 0;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT){
			quit = 1;
			break;
		}
		if (event.type == SDL_KEYDOWN) {
			if (event.key.keysym.sym == SDLK_a or event.key.keysym.sym == SDLK_LEFT)
				direction = LEFT;
			if (event.key.keysym.sym == SDLK_w or event.key.keysym.sym == SDLK_UP)
				direction = UP;
			if (event.key.keysym.sym == SDLK_s or event.key.keysym.sym == SDLK_DOWN)
				direction = DOWN;
			if (event.key.keysym.sym == SDLK_d or event.key.keysym.sym == SDLK_RIGHT)
				direction = RIGHT;
		}
	}

	if (quit){
		SDL_Quit();
		exit(0);
	}

	if (direction == -1)
		direction = snake.previousDirection;


	// if direction correct check death
	// checking correctness of direction
	bool correctDir = 1;
	char prevDir = snake.previousDirection;
	if (direction == RIGHT and prevDir == LEFT or direction == LEFT and prevDir == RIGHT or direction == UP and prevDir == DOWN
		or direction == DOWN and prevDir == UP)
		correctDir = 0;
	
	//check death
	if (ifSnakeDies(direction) and correctDir) {
		SDL_Quit();
		exit(0);
	}

	bool growth = 0;
	int tailX, tailY;
	//check food
	if (ifFoodConsumed(direction)){
		growth = 1;

		auto tailCoords = snake.listSegmentCoords.begin();
		// save tail coords
		if (snake.listSegmentCoords.size() > 1)
			auto tailCoords = snake.listSegmentCoords.end();


		tailX = tailCoords->first;
		tailY = tailCoords->second;
	}

	// move
	snake.moveForward(direction);
	//clear grid
	resetGridTilesTypes();

	//placeSnake
	placeSnakeOnTiles();

	//place food
	grid[foodPosition.first][foodPosition.second] = FOOD_TILE;


	//if growth, add segment with coords of tail after moving
	if (growth) {
		snake.addSegmentCoords(tailX, tailY);
		// generateFood
		generateFood();
	}
}



// GridRow

template <int numTiles>
GridRow<numTiles>::GridRow(bool wallRow) {

	if (wallRow) {
		for (int i = 0; i < numTiles; i++) {
			row[i] = WALL_TILE;
		}
	}
	else {
		for (int i = 0; i < numTiles; i++) {
			row[i] = EMPTY_TILE;
		}
		row[0] = WALL_TILE;
		row[numTiles - 1] = WALL_TILE;
	}
}

template <int numTiles>
GridRow<numTiles>::~GridRow() {

	//memory clearing for safety
	for (int i = 0; i < numTiles; i++)
		row[i] = NULL;
}

template <int numTiles>
TILE_TYPE& GridRow<numTiles>::operator[](int index) {
	if (index < 0 || index >= numTiles)
		throw std::exception("Array index out of bounds in GridRow");
	return row[index];
}
