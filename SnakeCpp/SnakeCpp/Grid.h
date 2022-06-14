#include <list>
#include <utility>
#include <random>

#include "settings.h"
#include "Snake.h"

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

	std::pair<int, int> placeSnakeOnTiles();
	void generateFood();

	void renderAll(SDL_Renderer* ren);
	// 
	//makeGameTurn(move snake)

	GridRow<numCols>& operator[](int numRow);

	// debug
	void printGridTypes();
	void renderGrid(SDL_Renderer* ren);

private:
	int pixelsToGridCoord(int pixels);
	int gridCoordToPixels(int gridCoord);

	void resetGridTilesTypes();
	void renderGridTiles(SDL_Renderer* ren);
	void renderSnake(SDL_Renderer* ren);

	bool ifTileWall(int indexX, int indexY);
	bool ifTileSnake(int indexX, int indexY);
	bool ifTileFood(int indexX, int indexY);
	bool ifTileEmpty(int indexX, int indexY);
	bool ifTileHeadSnake(int indexX, int indexY);
	bool ifAllTilesSnake(); // amazing player

	GridRow<numCols> grid[numRows];

	//food

	Snake<int> snake;
};

template <int numRows, int numCols>
Grid<numRows, numCols>::Grid() {

	grid[0] = GridRow<numCols>(true);
	for (int i = 1; i < numRows - 1; i++)
		grid[i] = GridRow<numCols>();
	grid[numRows - 1] = GridRow<numCols>(true);
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
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++)
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

	//TODO
	//change Food Position 

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

			if(ifTileEmpty(indexRow, indexCol))
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
	//RenderFood
}

template <int numRows, int numCols>
void Grid<numRows, numCols>::renderSnake(SDL_Renderer* ren) {

	int x, y;

	for (auto iterSegment : snake.listSegmentCoords) {
		x = gridCoordToPixels(iterSegment.first);
		y = gridCoordToPixels(iterSegment.second);
		drawBresenhamCircle(ren, x, y, SNAKE_SEGMENT_WIDTH, true);
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
