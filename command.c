#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"
#include "character.h"
#include "room.h"

void init();
void free_mem();

static void _room_move(char *args0);
static void _look_item(char *args1);
static void _use_item(char *args1);
static void _take_item(char *args1);
static void _drop_item(char *args1);
static void _look_character(char *args1);
static void _talk_character(char *args1);
static int search_item_cmd(char *args0, char *args1);
static int search_character_cmd(char *args0, char *args1);
static int search_map_cmd(char *args0);

void command_map(char *args0);
void command(char *args0, char *args1);

static void save();
static void load();

struct Command_Map
{
	char args0[10];
	void (*func)(char *args1);
};

static struct Command_Map map_cmd[] = {
	{
		"n", _room_move
	},
	{
		"s", _room_move
	},
	{
		"e", _room_move
	},
	{
		"w", _room_move
	},
	{
		"i", inventory
	},
	{
		"look", describe
	},
	{
		"save", save
	},
	{
		"load", load
	}
};

static struct Command_Map item_cmd[] = {
	{
		"look", _look_item
	},
	{
		"use", _use_item
	},
	{
		"take", _take_item
	},
	{
		"drop", _drop_item
	}
};

static struct Command_Map character_cmd[] = {
	{
		"look", _look_character
	},
	{
		"talk", _talk_character
	}
};

void init()
{
	init_room();
	init_character();
	init_item();
	reset_location();
}

void free_mem()
{
	free_room();
	free_character();
	free_item();
}

static void _room_move(char *args0)
{
	if(strcmp(args0, "n") == 0) {
		room_move(NORTH);
	}
	else if(strcmp(args0, "s") == 0) {
		room_move(SOUTH);
	}
	else if(strcmp(args0, "e") == 0) {
		room_move(EAST);
	}
	else if(strcmp(args0, "w") == 0) {
		room_move(WEST);
	}
	else {
		printf("404\n");
	}
}

static void _look_item(char *args1)
{
	if(search_item_from_room(args1) || search_item_from_inventory(args1)) {
		look_item(args1);
	} else {
		printf("not found\n");
	}
}

static void _use_item(char *args1)
{
	if(search_item_from_room(args1) || search_item_from_inventory(args1)) {
		use_item(get_player_score(), args1);
	} else {
		printf("not found\n");
	}
}

static void _take_item(char *args1)
{
	if(search_item_from_room(args1)) {
		char update[20];
		delete_item_from_room(args1);
		get_item_string(update, args1);
		add_item_to_inventory(update);
		printf("获得%s\n", update);
	} else {
		printf("not found\n");
	}
}

static void _drop_item(char *args1)
{
	if(search_item_from_inventory(args1)) {
		char update[20];
		delete_item_from_inventory(args1);
		get_item_string(update, args1);
		add_item_to_room(update);
		printf("放下%s\n", update);
	} else {
		printf("not found\n");
	}
}

static void _look_character(char *args1)
{
	if(search_character_from_room(args1)) {
		look_character(args1);
	} else {
		printf("not found\n");
	}
}

static void _talk_character(char *args1)
{
	if(search_character_from_room(args1)) {
		talk_character(args1);
	} else {
		printf("not found\n");
	}
}

static int search_item_cmd(char *args0, char *args1)
{
	int count = sizeof(item_cmd) / sizeof(struct Command_Map);
	for(int i = 0; i < count; i++) {
		if(strcmp(item_cmd[i].args0, args0) == 0 && search_item(args1)) {
			item_cmd[i].func(args1);
			return 1;
		}
	}
	return 0;
}

static int search_character_cmd(char *args0, char *args1)
{
	int count = sizeof(character_cmd) / sizeof(struct Command_Map);
	for(int i = 0; i < count; i++) {
		if(strcmp(character_cmd[i].args0, args0) == 0 && search_character(args1)) {
			character_cmd[i].func(args1);
			return 1;
		}
	}
	return 0;
}

static int search_map_cmd(char *args0)
{
	int count = sizeof(map_cmd) / sizeof(struct Command_Map);
	for(int i = 0; i < count; i++) {
		if(strcmp(map_cmd[i].args0, args0) == 0) {
			map_cmd[i].func(args0);
			return 1;
		}
	}
	return 0;
}

void command_map(char *args0)
{
	if(search_map_cmd(args0)) {
		
	} else {
		printf("map error\n");
	}
}

void command(char *args0, char *args1)
{
	if(search_item_cmd(args0, args1)) {

	} else if (search_character_cmd(args0, args1)) {

	} else {
		printf("error\n");
	}
}

static void save()
{
	save_room();
	save_character();
	printf("存档成功\n");
}

static void load()
{
	load_room();
	load_character();
	printf("读档成功\n");
	describe();
}
