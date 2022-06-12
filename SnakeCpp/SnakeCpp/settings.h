#pragma once

#define FPS 30
#define WINDOW_GAME_NAME "Snake"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 640

#define SNAKE_SEGMENT_WIDTH 50 // in pixels


typedef int TILE_TYPE;

enum TileTypes
{
	EMPTY_TILE = 0,
	WALL_TILE,
	FOOD_TILE,
	SNAKE_HEAD_TILE,
	SNAKE_TILE
};