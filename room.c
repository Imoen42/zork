#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"
#include "room.h"

#define MAX_ROWS 4

static void get_mem();
static void create_room(int id, void (*special_move)(enum Direction direction), char *name, char *description);
static void init_room_item(int id, int argc, char *args, ...);
static void init_room_character(int id, int argc, char *args, ...);
static void init_map();

void init_room();
void free_room();
void reset_location();
void set_location(char *args1);
void describe();
void room_move(enum Direction direction);
int search_item_from_room(char *args1);
int search_character_from_room(char *args1);
void add_item_to_room(char *update);
void add_character_to_room(char *update);
void delete_item_from_room(char *args1);
void save_room();
void load_room();

static void deadforest_move(enum Direction direction);
static void lakesidegraveyard_move(enum Direction direction);

struct Room
{
    struct Room *north;
    struct Room *south;
    struct Room *east;
    struct Room *west;
	void (*special_move)(enum Direction direction);//函数指针，满足某些条件才能进入下一地图
	int id;
    char name[20];
    char description[200];
	char item[5][20];
	char character[5][20];
};

static char save_location[20];
static struct Room *location;

static struct room_list
{
	struct Room rows[MAX_ROWS];
};
static struct room_list *p;

static void get_mem()
{
	p = (struct room_list*)malloc(sizeof(struct room_list));
}

static void create_room(int id, void (*special_move)(enum Direction direction), char *name, char *description)
{
	struct Room *room = &p->rows[id];
	room->id = id;
	room->special_move = special_move;
	memcpy(room->name, name, strlen(name) +1);
	memcpy(room->description, description, strlen(description) +1);
}

static void init_room_item(int id, int argc, char *args, ...)//每个地图的初始道具不同，所以这里用可变参函数
{
	va_list list;
	va_start(list, args);
	for(int i = 0; i < argc; i++) {
		snprintf(p->rows[id].item[i], 20, "%s", va_arg(list, char *));
	}
	va_end(list);
}

static void init_room_character(int id, int argc, char *args, ...)
{
	va_list list;
	va_start(list, args);
	for(int i = 0; i < argc; i++) {
		snprintf(p->rows[id].character[i], 20, "%s", va_arg(list, char *));
	}
	va_end(list);
}

static void init_map()
{
	p->rows[0].north = &p->rows[3];
	p->rows[0].south = NULL;
	p->rows[0].east = NULL;
	p->rows[0].west = &p->rows[1];

	p->rows[1].north = &p->rows[2];
	p->rows[1].south = NULL;
	p->rows[1].east = &p->rows[0];
	p->rows[1].west = NULL;

	p->rows[2].north = &p->rows[1];
	p->rows[2].south = NULL;
	p->rows[2].east = NULL;
	p->rows[2].west = NULL;

	p->rows[3].north = NULL;
	p->rows[3].south = &p->rows[0];
	p->rows[3].east = NULL;
	p->rows[3].west = NULL;
}

void init_room()
{
	get_mem();
	create_room(0, &deadforest_move, "亡者森林", "这里是一片橡树林，住在这儿的松鼠似乎并不怕人，如果你冲它们伸手，没准还能得到礼物。（北边是奥克镇，西边是湖边墓地。");
	init_room_item(0, 2, "list", "蜡烛(candle)", "二胡(erhu)");
	init_room_character(0, 1, "list", "喵(miao)");

	create_room(1, &lakesidegraveyard_move, "湖边墓地", "小心点儿！别磕着墓碑！（东边是一片橡树林，北边有间小木屋");
	init_room_item(1, 2, "list", "小提琴(violin)", "蜡烛(candle)");

	create_room(2, NULL, "小木屋", "昏暗的烛光中，足以看清蹲在墙角的人，此外空无一物。（出口在南边）");
	init_room_character(2, 1, "list", "守墓人(gk)");
	
	create_room(3, NULL, "奥克镇南门", "两边长满了橡树，枝叶被刻意修剪过，形成一条长长的通道，直通往奥克镇广场。（南边是亡者森林，北边是奥克镇广场。）");
	
	init_map();
}

void free_room()
{
	free(p);
	p = NULL;
}

void reset_location()
{
	location = &p->rows[0];
	describe();
}

void set_location(char *args1)
{
	for(int i = 0; i < MAX_ROWS; i++) {
		if(strstr(p->rows[i].name, args1)) {
			location = &p->rows[i];
		}
	}
	describe();
}

void describe()
{
    printf("\033[5m\033[47;30m%s\033[0m\n%s\n", location->name, location->description);
    printf("\033[47;30m道具:\033[0m\n");
    for (int i = 0; i < 5; i++) {
		if(strcmp(location->item[i], "\0") != 0) {
			printf("\033[1m\033[40;37m%s\033[0m\n", location->item[i]);
		}
    }
    printf("\033[47;30m角色:\033[0m\n");
    for (int i = 0; i < 5; i++) {
		if(strcmp(location->character[i], "\0") != 0) {
			printf("\033[1m\033[40;37m%s\033[0m\n", location->character[i]);
		}
    }
}

void room_move(enum Direction direction)
{
	if (!location->special_move) {
	    if(direction == NORTH && location->north) {
    	    location = location->north;
			describe();
    	}
		else if(direction == SOUTH && location->south) {
       		location = location->south;
			describe();
  		}
		else if(direction == EAST && location->east) {
        	location = location->east;
			describe();
    	}
		else if(direction == WEST && location->west) {
        	location = location->west;
			describe();
    	}
		else {
        	printf("此路不通\n");
    	}
	}
	else {
		location->special_move(direction);
	}
}

int search_item_from_room(char *args1)
{
	for(int i = 0; i < 5; i++) {
		if(strstr(location->item[i], args1)) return 1;
	}
	return 0;
}

int search_character_from_room(char *args1)
{
	for(int i = 0; i < 5; i++) {
		if(strstr(location->character[i], args1)) return 1;
	}
	return 0;
}

void add_item_to_room(char *update)
{
	int j = 0;
	for(int i = 0; strcmp(location->item[i], "\0") != 0; i++) j++;
	if(j < 5) {
		memcpy(location->item[j], update, strlen(update) +1);
	} else {
		printf("full\n");
	}
}

void add_character_to_room(char *update)
{
	int j = 0;
	for(int i = 0; strcmp(location->character[i], "\0") != 0; i++) j++;
	if(j < 5) {
		memcpy(location->character[j], update, strlen(update) +1);
	} else {
		printf("full\n");
	}
}

void delete_item_from_room(char *args1)
{
	int j = 0;
	for(int i = 0; i < 5; i++) {
		if(strstr(location->item[i], args1)) j = i;
	}
	memcpy(location->item[j], "\0", strlen("\0") +1);
}

void save_room()
{
	FILE *fp;
	if((fp = fopen("save", "wb")) == NULL) {
		printf("404\n");
		exit(0);
	}
	for(int i = 0; i < MAX_ROWS; i++) {
		fwrite(p->rows[i].item, sizeof(p->rows[i].item), 1, fp);
		fwrite(p->rows[i].character, sizeof(p->rows[i].character), 1, fp);
	}
	memcpy(save_location, location->name, strlen(location->name) +1);
	fwrite(save_location, sizeof(save_location), 1, fp);
	printf("save:%s\n", save_location);
	fclose(fp);
}

void load_room()
{
	FILE *fp;
	if((fp = fopen("save", "ab+")) == NULL) {
		printf("404\n");
		exit(0);
	}
	for(int i = 0; i < MAX_ROWS; i++) {
		fread(p->rows[i].item, sizeof(p->rows[i].item), 1, fp);
		fread(p->rows[i].character, sizeof(p->rows[i].character), 1, fp);
	}
	fread(save_location, sizeof(save_location), 1, fp);
	for(int i = 0; i < MAX_ROWS; i++) {
		if(strstr(p->rows[i].name, save_location)) {
			printf("load:%s\n", save_location);
			printf("location:%s\n", p->rows[i].name);
			location = &p->rows[i];
		}
	}
	printf("location:%s\n", location->name);
	fclose(fp);
	describe();
}

static void deadforest_move(enum Direction direction)
{
    if(direction == NORTH && location->north) {
		if (search_item_from_inventory("erhu") || search_item_from_inventory("violin")) {
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

static void lakesidegraveyard_move(enum Direction direction)
{
    if(direction == NORTH && location->north) {
		if (search_item_from_inventory("candle")) {
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
