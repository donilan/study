#pragma once

#define MAX_MONSTER_LOCATION 10

struct STRUCT_GAME_LOCATION {
	unsigned int x;
	unsigned int y;
	unsigned int cx;
	unsigned int cy;
	unsigned int status;
};
typedef STRUCT_GAME_LOCATION GAME_LOCATION, *PGAME_LOCATION;

const GAME_LOCATION MONSTER_LOCATIONS[MAX_MONSTER_LOCATION] = {
	{6, 255, 55, 16, 0},
	{70, 219, 55, 16, 0},
	{134, 183, 55, 16, 0},
	{198, 147, 55, 16, 0},
	{262, 111, 55, 16, 0},
	{58, 301, 55, 16, 0},
	{122, 267, 55, 16, 0},
	{186, 233, 55, 16, 0},
	{250, 197, 55, 16, 0},
	{314, 161, 55, 16, 0},
};