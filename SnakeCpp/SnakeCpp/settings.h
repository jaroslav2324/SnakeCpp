#pragma once

#define FPS 30
#define WINDOW_GAME_NAME "Snake"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define SNAKE_SEGMENT_WIDTH 50 // in pixels


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