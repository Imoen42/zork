#ifndef _room_h
#define _room_h

enum Direction {
    NORTH, SOUTH, EAST, WEST
};

struct Room
{
    char name[20];
    char description[200];
	char item[5][20];
	char character[5][20];
    struct Room *north;
    struct Room *south;
    struct Room *east;
    struct Room *west;
	void (*special_move)(enum Direction direction);//函数指针，满足某些条件才能进入下一地图
};

extern struct Room DeadForest;
extern struct Room LakesideGraveyard;
extern struct Room LogCabin;
extern struct Room OakTownGate;
extern struct Room *location;

#endif
