#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ROWS 3

static void get_mem();
static void create_item(int id, int player_score, int turn_on, char *zh_name, char *name, char *description, char *turn_on_desc, char *turn_off_desc, char *failed);
static struct Items *get_item(char *args1);

int search_item(char *args1);
char *get_item_string(char *update, char *args1);
void look_item(char *args1);
void use_item(int player_score, char *args1);
int item_on(char *args1);
void set_item(char *args1);
void init_mem();
void free_item();

struct Items
{
	int id;
	int player_score;
    int turn_on;
    char zh_name[10];
    char name[10];
    char description[200];
	char turn_on_desc[30];
	char turn_off_desc[30];
	char failed[50];
};

static struct item_list
{
	struct Items rows[MAX_ROWS];
};
static struct item_list *p;

static void get_mem()
{
	p = (struct item_list*)malloc(sizeof(struct item_list));
}

static void create_item(int id, int player_score, int turn_on, char *zh_name, char *name, char *description, char *turn_on_desc, char *turn_off_desc, char *failed)
{
	struct Items *item = &p->rows[id];
	item->id = id;
	item->player_score = player_score;
	item->turn_on = turn_on;
	memcpy(item->zh_name, zh_name, strlen(zh_name) +1);
	memcpy(item->name, name, strlen(name) +1);
	memcpy(item->description, description, strlen(description) +1);
	memcpy(item->turn_on_desc, turn_on_desc, strlen(turn_on_desc) +1);
	memcpy(item->turn_off_desc, turn_off_desc, strlen(turn_off_desc) +1);
	memcpy(item->failed, failed, strlen(failed) +1);
}

static struct Items *get_item(char *args1)
{
	for(int i = 0; i < MAX_ROWS; i++) {
		struct Items *item = &p->rows[i];
		if(strcmp(item->name, args1) == 0) return item;
	}
	return 0;
}

int search_item(char *args1)
{
	for(int i = 0; i < MAX_ROWS; i++) {
		struct Items *item = &p->rows[i];
		if(strcmp(item->name, args1) == 0) return 1;
	}
	return 0;
}

char *get_item_string(char *update, char *args1)
{
	struct Items *item = get_item(args1);
	char temp[20];
	if(update != NULL)
		snprintf(temp, sizeof temp, "%s(%s)", item->zh_name, item->name);
	memcpy(update, temp, strlen(temp) +1);
	return update;
}

void look_item(char *args1)
{
	struct Items *item = get_item(args1);
	printf("%s\n", item->description);
}

void use_item(int player_score, char *args1)
{
	struct Items *item = get_item(args1);
	if(player_score >= item->player_score && item->turn_on) {
		item->turn_on = 0;
		printf("\033[40;37m%s\033[0m\n", item->turn_off_desc);
	} else if(player_score >= item->player_score && !item->turn_on) {
		item->turn_on = 1;
		printf("\033[40;37m%s\033[0m\n", item->turn_on_desc);
	} else {
		printf("\033[40;31m%s\033[0m\n", item->failed);
	}
}

int item_on(char *args1)
{
	struct Items *item = get_item(args1);
	if(item->turn_on) return 1;
	return 0;
}

void set_item(char *args1)
{
	struct Items *item = get_item(args1);
	if(item->turn_on) {
		item->turn_on = 0;
		printf("\033[40;37m%s\033[0m\n", item->turn_off_desc);
	} else {
		item->turn_on = 1;
		printf("\033[40;37m%s\033[0m\n", item->turn_on_desc);
	}
}

void init_item()
{
	get_mem();
	create_item(0, 0, 0, "蜡烛", "candle", "一支含有微弱魔力的蜡烛。", "蜡烛被点燃", "蜡烛熄灭了", "error");
	create_item(1, 100, 0, "小提琴", "violin", "一把橘红色的小提琴，拥有与其身形不相匹配的重量，根本不适合用来演奏。", "拉响了小提琴。", "小提琴不再发出任何声音。", "这把小提琴对你来说过于沉重，无法演奏。");
	create_item(2, 0, 0, "二胡", "erhu", "来自神秘国度的乐器。", "拉响了二胡。", "停止了演奏。", "");
}

void free_item()
{
	free(p);
	p = NULL;
}
