#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "room.h"
#include "item.h"
#include "character.h"
#include "command.h"

#define ARRAYCOUNT 2

struct Map dict[ARRAYCOUNT] = {//简单的键值对, 简化人名输入
	{"miao", "miao", &miao_talk},
    {"gk", "gravekeeper", &GraveKeeper_talk}
}, *m;

static char save_location[20];

struct Room *room_list[] = {
	&DeadForest,
	&LakesideGraveyard,
	&LogCabin,
	&OakTownGate
};

struct Characters *character_list[] = {
	&miao,
	&GraveKeeper
};

struct Items *item_list[] = {
	&candle,
	&violin,
	&erhu
};

void reset_location()
{
	location = &DeadForest;
	describe();
}

void describe()
{
    printf("\033[5m\033[47;30m%s\033[0m\n%s\n", location->name, location->description);
    printf("\033[47;30m道具:\033[0m\n");
    for (int i = 0; i < 5 && strcmp(location->item[i], "\0") != 0; i++) {
		printf("\033[1m\033[40;37m%s\033[0m\n", location->item[i]);
    }
    printf("\033[47;30m角色:\033[0m\n");
    for (int i = 0; i < 5 && strcmp(location->character[i], "\0") != 0; i++) {
		printf("\033[1m\033[40;37m%s\033[0m\n", location->character[i]);
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

void inventory()
{
    printf("\033[47;30m背包:\033[0m\n");
    for (int i = 0; i < 5 && strcmp(player.item[i], "\0") != 0; i++) {
		printf("\033[1m\033[40;37m%s\033[0m\n", player.item[i]);
    }
}

static void look_item(struct Items *item)
{
	printf("%s\n", item->description);
}

static void look_character(char *args1)
{
    m = &dict[0];
    for (int i = 0; i < ARRAYCOUNT; i++) {
		if (strcmp(m[i].key, args1) == 0) {
			for (int j = 0; j < 3; j++) {
				if (strcmp(character_list[j]->name, m[i].value) == 0) {
					printf("%s\n", character_list[j]->description);
				}
			}
		}
    }
}

static void talk_to_character(char *args1)
{
	m = &dict[0];
	for (int i = 0; i < ARRAYCOUNT; i++) {
		if (strcmp(m[i].key, args1) == 0) {
			m[i].talk();//函数指针调用函数
		}
	}
}

static void use_item(struct Items *item)
{
	if (player.player_score >= item->player_score) {
		printf("\033[40;37m%s\033[0m\n", item->turn_on_description);
		item->turn_on = 1;
	}
	else {
		printf("\033[40;31m%s\033[0m\n", item->fail);
	}
}

static void delete_item_from_inventory(struct Items *item)
{
	int i, d, u;
    for (i = 0; i < 10 && strcmp(player.item[i], "\0") != 0; i++) {
		if (strstr(player.item[i], item->name)) {
			d = i;
		}
	}
	i--;
	if (strstr(player.item[d], item->name) && i > 0) {
		for (u = 0; u < i; ++u) {
    	    if (u < d) { //循环变量u小于插入值位置i时, 每一个元素所存放的位置不变
    	        memcpy(player.item[u], player.item[u], strlen(player.item[u]) +1);
				memcpy(player.item[d], "\0", strlen("\0") +1);
    	    }
			else {
    	        memcpy(player.item[u], player.item[u+1], strlen(player.item[u+1]) +1);
				memcpy(player.item[u+1], "\0", strlen("\0") +1); //删除值后面的元素都往前移一位, 要删除的值直接被覆盖
    	    }
    	}
	}
	else if ( i == 0) {
		memcpy(player.item[i], "\0", strlen("\0") +1);
	}
	else {
		printf("输入有误请重新输入\n");
	}
}

static void drop_item(struct Items *item)
{
	char update[20];
	int x = 0;
	for (int i = 0; strcmp(location->item[i], "\0") != 0; i++) {
		x++;
	}
	snprintf(update, sizeof update, "%s(%s)", item->zh_name, item->name);
	memcpy(location->item[x], update, strlen(update) +1);
	delete_item_from_inventory(item);
	printf("放下%s\n", item->zh_name);
}

static void add_item_to_inventory(struct Items *item)
{
    char update[20];
	int x = 0;
    for (int i = 0; strcmp(player.item[i], "\0") != 0; i++) {
		x++;
	}
	sprintf(update, "%s(%s)", item->zh_name, item->name);
	memcpy(player.item[x], update, strlen(update) +1);
}

static void take_item(struct Items *item)
{
	int i, d, u;
    for (i = 0; i < 5 && strcmp(location->item[i], "\0") != 0; i++) {
		if (strstr(location->item[i], item->name)) {
			d = i;
		}
	}
	i--;
	if (strstr(location->item[d], item->name) && i > 0) {
		for (u = 0; u<i; ++u) {
    	    if (u < d) {
    	        memcpy(location->item[u], location->item[u], strlen(location->item[u]) +1);
				memcpy(location->item[d], "\0", strlen("\0") +1);
    	    }
			else {
    	        memcpy(location->item[u], location->item[u+1], strlen(location->item[u+1]) +1);
				memcpy(location->item[u+1], "\0", strlen("\0") +1);
    	    }
    	}
	}
	else if ( i == 0) {
		memcpy(location->item[i], "\0", strlen("\0") +1);
	}
	else {
		printf("输入有误请重新输入\n");
	}
	add_item_to_inventory(item);
	printf("获得%s\n", item->zh_name);
}

static int get_item(char *args1)
{
	char temp[20];
	sprintf(temp, "(%s)", args1);
	for (int i = 0; i < 5; i++) {
		if (strstr(location->item[i], temp)) {
			for (int j = 0; j < 3; j++) {
				if (strcmp(item_list[j]->name, args1) == 0)
					return j;
			}
		}
	}
	return -1;
}

static int get_item_from_inventory(char *args1)
{
	char temp[20];
	sprintf(temp, "(%s)", args1);
	for (int i = 0; i < 10; i++) {
		if (strstr(player.item[i], temp)) {
			for (int j = 0; j < 3; j++) {
				if (strcmp(item_list[j]->name, args1) == 0)
					return j;
			}
		}
	}
	return -1;
}

static int get_character(char *args1)
{
	char temp[20];
	sprintf(temp, "(%s)", args1);
	for (int i = 0; i < 5; i++) {
		if (strstr(location->character[i], temp)) {
			return i;
		}
	}
	return -1;
}

void search_command(char *args0, char *args1)
{
	if (strcmp(args0, "take") == 0 && get_item(args1) >= 0) {
		take_item(item_list[get_item(args1)]);
	}
	else if (strcmp(args0, "drop") == 0 && get_item_from_inventory(args1) >= 0) {
		drop_item(item_list[get_item_from_inventory(args1)]);
	}
	else if (strcmp(args0, "look") == 0) {
		if (get_item(args1) >= 0) {
			look_item(item_list[get_item(args1)]);
		}
		else if (get_item_from_inventory(args1) >= 0) {
			look_item(item_list[get_item_from_inventory(args1)]);
		}
		else if (get_character(args1) >= 0) {
			look_character(args1);
		}
		else {
			printf("没看见%s\n", args1);
		}
	}
	else if (strcmp(args0, "use") == 0) {
		if (get_item(args1) >= 0) {
			use_item(item_list[get_item(args1)]);
		}
		else if (get_item_from_inventory(args1) >= 0) {
			use_item(item_list[get_item_from_inventory(args1)]);
		}
		else {
			printf("没看见%s\n", args1);
		}
	}
	else if (strcmp(args0, "talk") == 0) {
		if (get_character(args1) >= 0) {
			talk_to_character(args1);
		}
	}
	else {
		printf("输入有误请重新输入\n");
	}
}

void save()
{
	FILE *fp;
	if ((fp = fopen("save", "wb")) == NULL) {
		printf("404\n");
		exit(0);
	}

	for (int i = 0; i < 4; i++) {
		if (fwrite(room_list[i], sizeof(struct Room), 1, fp) != 1)
			printf("404\n");
		fwrite(room_list[i]->item, sizeof(room_list[i]->item), 1, fp);
		fwrite(room_list[i]->character, sizeof(room_list[i]->character), 1, fp);
	}

	for (int i = 0; i < 2; i++) {
		if (fwrite(character_list[i], sizeof(struct Characters), 1, fp) != 1)
			printf("404\n");
		fwrite(character_list[i], sizeof(character_list[i]), 1, fp);
	}

	fwrite(&player, sizeof(struct Player), 1, fp);
	memcpy(save_location, location->name, strlen(location->name) +1);
	fwrite(save_location, sizeof(save_location), 1, fp);
	printf("存档成功\n");
	fclose(fp);
}

void load()
{
	FILE * fp;
	if ((fp = fopen("save", "rb")) == NULL) {
		printf("404\n");
		exit(0);
	}

	for (int i = 0; i < 4; i++) {
		if (fread(room_list[i], sizeof(struct Room), 1, fp) != 1)
			printf("404\n");
		fread(room_list[i]->item, sizeof(room_list[i]->item), 1, fp);
		fread(room_list[i]->character, sizeof(room_list[i]->character), 1, fp);
	}

	for (int i = 0; i < 2; i++) {
		if (fread(character_list[i], sizeof(struct Characters), 1, fp) != 1)
			printf("404\n");
		fread(character_list[i], sizeof(character_list[i]), 1, fp);
	}

	fread(&player, sizeof(struct Player), 1, fp);
	fread(save_location, sizeof(save_location), 1, fp);
	for (int i = 0; i < 4; i++) {
		if (strstr(room_list[i]->name, save_location)) {
			location = room_list[i];
		}
	}
	printf("读档成功\n");
	fclose(fp);
	describe();
}

int search_inventory(char *args)
{
	for (int i = 0; i < 10; i++) {
		if (strstr(player.item[i], args)) {
			return 1;
		}
	}
	return 0;
}

int search_item(char *args)
{
	for (int i = 0; i < 3; i++) {
		if (item_list[i]->turn_on) {
			return 1;
		}
	}
	return 0;
}

void turn_off_item(char *args)
{
	for (int i = 0; i < 3; i++) {
		if (strcmp(item_list[i]->name, args) == 0)
			item_list[i]->turn_on = 0;
	}
}
