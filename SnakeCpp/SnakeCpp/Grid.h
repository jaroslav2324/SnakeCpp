#include <utility>
#include <random>

#include "settings.h"


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
	// ifAllTilesSnake

	//void renderAllItemsOnGrid(SDL_Renderer& ren);
	//makeGameTurn(move snake)

	bool ifTileWall(int coord_x_in_pixels, int coord_y_in_pixels);
	bool ifTileSnake(int coord_x_in_pixels, int coord_y_in_pixels);
	bool ifTileFood(int coord_x_in_pixels, int coord_y_in_pixels);
	bool ifTilewmpty(int coord_x_in_pixels, int coord_y_in_pixels);
	bool ifTileHeadSnake(int coord_x_in_pixels, int coord_y_in_pixels);

	GridRow<numCols>& operator[](int numRow);

	// debug
	void printGridTypes();

private:
	int pixelsToGridCoord(int pixels);
	int gridCoordToPixels(int gridCoord);
	void clearGrid();

	GridRow<numCols> grid[numRows];

	
	//food
	//pointer to snake or reference
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
void Grid<numRows, numCols>::clearGrid() {

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
bool Grid<numRows, numCols>::ifTileWall(int coord_x_in_pixels, int coord_y_in_pixels) {

	int gridCoordX = pixelsToGridCoord(coord_x_in_pixels);
	int gridCoordY = pixelsToGridCoord(coord_y_in_pixels);

	if (grid[gridCoordX][gridCoordY] == WALL_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileSnake(int coord_x_in_pixels, int coord_y_in_pixels) {

	int gridCoordX = pixelsToGridCoord(coord_x_in_pixels);
	int gridCoordY = pixelsToGridCoord(coord_y_in_pixels);

	if (grid[gridCoordX][gridCoordY] == SNAKE_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileFood(int coord_x_in_pixels, int coord_y_in_pixels) {

	int gridCoordX = pixelsToGridCoord(coord_x_in_pixels);
	int gridCoordY = pixelsToGridCoord(coord_y_in_pixels);

	if (grid[gridCoordX][gridCoordY] == FOOD_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTilewmpty(int coord_x_in_pixels, int coord_y_in_pixels) {

	int gridCoordX = pixelsToGridCoord(coord_x_in_pixels);
	int gridCoordY = pixelsToGridCoord(coord_y_in_pixels);

	if (grid[gridCoordX][gridCoordY] == EMPTY_TILE)
		return true;
	else
		return false;
}

template <int numRows, int numCols>
bool Grid<numRows, numCols>::ifTileHeadSnake(int coord_x_in_pixels, int coord_y_in_pixels) {

	int gridCoordX = pixelsToGridCoord(coord_x_in_pixels);
	int gridCoordY = pixelsToGridCoord(coord_y_in_pixels);

	if (grid[gridCoordX][gridCoordY] == SNAKE_HEAD_TILE)
		return true;
	else
		return false;
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
	clearGrid();

	//placeSnake
	placeSnakeOnTiles();

	//place food
	grid[numRandomRow][numRandomCol] = FOOD_TILE;

	//TODO
	//change Food Position 

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



