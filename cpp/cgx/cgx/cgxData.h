#pragma once

#define MAX_MONSTER_LOCATION 10
#define MAX_LOCATION 100

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

enum {BTN_PK, BTN_WATCH_PK, BTN_JOIN, BTN_EXCHANGE, BTN_MAP, BTN_DISABLE_MONSTER, BTN_SALE, BTN_DUMP, 
	FIT_BTN_HIT, FIT_BTN_SKILL, FIT_BTN_MONSTER,
	FIT_BTN_DEFIND, FIT_BTN_GOODS, FIT_BTN_LOCATION, FIT_BTN_RUN,
	FIT_BTN_MONSTER_SKILL};
const GAME_LOCATION ALL_LOCATIONS[MAX_LOCATION] = {
	{410, 32, 19, 19, 0},
	{431, 32, 19, 19, 0},
	{452, 32, 19, 19, 0},
	{494, 32, 19, 19, 0},
	{557, 32, 19, 19, 0},
	{578, 32, 19, 19, 0},
	{599, 32, 19, 19, 0},
	{620, 32, 19, 19, 0},// t ren

	{360, 52, 57, 26, 0},
	{430, 52, 57, 26, 0},
	{500, 52, 57, 26, 0},

	{360, 77, 57, 26, 0},
	{430, 77, 57, 26, 0},
	{500, 77, 57, 26, 0},
	{570, 52, 57, 26, 0},

	{563,78, 57, 22, 0},
};

#define G_LOCATION(idx) ALL_LOCATIONS[idx]

#define COLOR_BTN_ENABLE RGB(154, 85, 14)
#define COLOR_BTN_DISABLE RGB(106, 77, 51)

#define COLOR_FIT_BTN_ENABLE RGB(147, 187, 108)
#define COLOR_FIT_BTN_DISABLE RGB(212, 173, 106)

#define COLOR_MONSTER_FONT RGB(255, 255, 255)
#define DEVIATION_MONSTER_FONT 5
#define DEVIATION_BTN 5
#define DEVIATION_FIT_BTN 5