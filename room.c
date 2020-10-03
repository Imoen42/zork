#include <stdio.h>
#include "room.h"
#include "command.h"

void deadforest_move(enum Direction direction)
{
    if(direction == NORTH && location->north) {
		if (search_inventory("erhu") || search_inventory("violin")) {
			location = location->north;
			describe();
		}
		else {
			printf("需要乐器。\n");
		}
    }
	else if(direction == WEST && location->west) {
        location = location->west;
		describe();
    }
	else {
        printf("此路不通\n");
    }
}

void lakesidegraveyard_move(enum Direction direction)
{
    if(direction == NORTH && location->north) {
		if (search_inventory("candle") && search_item("candle")) {
			location = location->north;
			describe();
		}
		else {
			printf("里面太黑了。\n");
		}
    }
	else if(direction == EAST && location->east) {
        location = location->east;
		describe();
    }
	else {
        printf("此路不通\n");
    }
}

struct Room DeadForest = {
	"亡者森林",
	"这里是一片橡树林，住在这儿的松鼠似乎并不怕人，如果你冲它们伸手，没准还能得到礼物。（北边是奥克镇，西边是湖边墓地。",
	{"蜡烛(candle)", "二胡(erhu)"},
	{"喵(miao)"},
	&OakTownGate,
	NULL,
	NULL,
	&LakesideGraveyard,
	&deadforest_move
};
struct Room LakesideGraveyard = {
	"湖边墓地",
	"小心点儿！别磕着墓碑！（东边是一片橡树林，北边有间小木屋）",
	{"小提琴(violin)"},
	{},
	&LogCabin,
	NULL,
	&DeadForest,
	NULL,
	&lakesidegraveyard_move
};
struct Room LogCabin = {
	"小木屋",
	"昏暗的烛光中，足以看清蹲在墙角的人，此外空无一物。（出口在南边）",
	{},
	{"守墓人(gk)"},
	NULL,
	&LakesideGraveyard,
	NULL,
	NULL,
	NULL
};
struct Room OakTownGate = {
	"奥克镇南门",
	"两边长满了橡树，枝叶被刻意修剪过，形成一条长长的通道，直通往奥克镇广场。（南边是亡者森林，北边是奥克镇广场。）",
	{},
	{},
	NULL,
	&DeadForest,
	NULL,
	NULL,
	NULL,
};

struct Room *location;
