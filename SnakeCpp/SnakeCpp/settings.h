#pragma once

#define FPS 30
#define WINDOW_GAME_NAME "Snake"

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

#define SNAKE_SEGMENT_WIDTH 50 // in pixels

#define EXIT_GAME_CODE -1
#define GAME_STILL_RUNNING_CODE 1

typedef int TILE_TYPE;

#define and &&
#define or ||
#define not !

enum TileTypes{
	EMPTY_TILE = 0,
	WALL_TILE,
	FOOD_TILE,
	SNAKE_HEAD_TILE,
	SNAKE_TILE
};

// char values
enum MovingDirections {
	UP = 0,
	DOWN,
	RIGHT,
	LEFT,
};

enum Difficulty {
	EASY = 1,
	MEDIUM,
	HARD,
	IMPOSSIBLE
};