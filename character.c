#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define MAX_ROWS 2

static void get_mem();
static void create_character(void (*talk)(), int id, int talk_count, int state, char *zh_name, char *name, char *description);
static struct Characters *get_character(char *args1);

void init_character();
void free_character();
char *get_character_string(char *update, char *args1);
int search_character(char *args1);
int get_player_score();
void inventory();
int search_item_from_inventory(char *args1);
void add_item_to_inventory(char *update);
void delete_item_from_inventory(char *args1);
void look_character(char *args1);
void talk_character(char *args1);

static void set_state(struct Characters *character, int state);
static void add_state(struct Characters *character, int state);
static void delete_state(struct Characters *character, int state);
static int is_state(struct Characters *character, int state);
static int is_only_state(struct Characters *character, int state);
static int is_not_state(struct Characters *character, int state);

void save_character();
void load_character();

static void miao_talk();
static void GraveKeeper_talk();

struct Player
{
	int player_score;
	char item[10][20];
	char team[10][20];
};
static struct Player player = {0, {}, {}};

struct Characters
{
	void (*talk)();
	int id;
	int talk_count;
	int state;
	char zh_name[20];
	char name[20];
	char description[100];
};

static struct character_list
{
	struct Characters rows[MAX_ROWS];
};
static struct character_list *p;

struct State
{
	int team;//00001
	int alive;//00010
	int sad;//00100
	int angry;//01000
	int happy;//10000
};
static const struct State state = {
	1 << 0,
	1 << 1,
	1 << 2,
	1 << 3,
	1 << 4
};

static void get_mem()
{
	p = (struct character_list*)malloc(sizeof(struct character_list));
}

static void create_character(void (*talk)(), int id, int talk_count, int state, char *zh_name, char *name, char *description)
{
	struct Characters *character = &p->rows[id];
	character->talk = talk;
	character->id = id;
	character->talk_count = talk_count;
	character->state = state;
	memcpy(character->zh_name, zh_name, strlen(zh_name) +1);
	memcpy(character->name, name, strlen(name) +1);
	memcpy(character->description, description, strlen(description) +1);
}

void init_character()
{
	get_mem();
	create_character(&GraveKeeper_talk, 0, 0, 3, "守墓人", "gravekeeper", "一位白发苍苍的老者，总是低着头，无法看清容貌。");
	create_character(&miao_talk, 1, 0, 3, "喵", "miao", "喵喵喵~");
}

void free_character()
{
	free(p);
	p = NULL;
}

static struct Characters *get_character(char *args1)
{
	for(int i = 0; i < MAX_ROWS; i++) {
		struct Characters *character = &p->rows[i];
		if(strcmp(character->name, args1) == 0) return character;
	}
	return 0;
}

char *get_character_string(char *update, char *args1)
{
	struct Characters *character = get_character(args1);
	char temp[20];
	if(update != NULL) {
		snprintf(temp, 20, "%s(%s)", character->zh_name, character->name);
	}
	memcpy(update, temp, strlen(temp) +1);
	return update;
}

int search_character(char *args1)
{
	for(int i = 0; i < MAX_ROWS; i++) {
		struct Characters *character = &p->rows[i];
		if(strcmp(character->name, args1) == 0) return 1;
	}
	return 0;
}

int get_player_score()
{
	return player.player_score;
}

void inventory()
{
	printf("\033[47;30m背包:\033[0m\n");
	for(int i = 0; i < 10; i++) {
		if(strcmp(player.item[i], "\0") != 0) {
			printf("\033[1m\033[40;37m%s\033[0m\n", player.item[i]);
		}
	}
}

int search_item_from_inventory(char *args1)
{
	for(int i = 0; i < 10; i++) {
		if(strstr(player.item[i], args1)) return 1;
	}
	return 0;
}

void add_item_to_inventory(char *update)
{
	int j = 0;
	for(int i = 0; strcmp(player.item[i], "\0") != 0; i++) j++;
	if(j < 10) {
		memcpy(player.item[j], update, strlen(update) +1);
	} else {
		printf("full\n");
	}
}

void delete_item_from_inventory(char *args1)
{
	int j = 0;
	for(int i = 0; i < 10; i++) {
		if(strstr(player.item[i], args1)) j = i;
	}
	memcpy(player.item[j], "\0", strlen("\0") +1);
}

void look_character(char *args1)
{
	struct Characters *character = get_character(args1);
	printf("%s\n", character->description);
}

void talk_character(char *args1)
{
	struct Characters *character = get_character(args1);
	character->talk();
}

static void set_state(struct Characters *character, int state)//设置状态
{
	character->state = state;
}

static void add_state(struct Characters *character, int state)//添加一种或多种状态
{
	character->state |= state;
}

static void delete_state(struct Characters *character, int state)//移除一种或多种状态
{
	character->state &= ~state;
}

static int is_state(struct Characters *character, int state)//是否拥有某种状态
{
	return (character->state & state) == state;
}

static int is_only_state(struct Characters *character, int state)//是否仅仅拥有某种状态
{
	return character->state == state;
}

static int is_not_state(struct Characters *character, int state)//是否没有某种状态
{
	return (character->state & state) == 0;
}

void save_character()
{
	FILE *fp;
	if((fp = fopen("save", "ab+")) == NULL) {
		printf("404\n");
		exit(0);
	}
	for(int i = 0; i < MAX_ROWS; i++) {
		fwrite(&p->rows[i].talk_count, sizeof(p->rows[i].talk_count), 1, fp);
		fwrite(&p->rows[i].state, sizeof(p->rows[i].state), 1, fp);
	}
	fwrite(&player, sizeof(struct Player), 1, fp);
	fclose(fp);
}

void load_character()
{
	FILE *fp;
	if((fp = fopen("save", "ab+")) == NULL) {
		printf("404\n");
		exit(0);
	}
	fseek(fp, -1L*(MAX_ROWS*8 +sizeof(struct Player)), SEEK_END);
	for(int i = 0; i < MAX_ROWS; i++) {
		fread(&p->rows[i].talk_count, sizeof(p->rows[i].talk_count), 1, fp);
		fread(&p->rows[i].state, sizeof(p->rows[i].state), 1, fp);
	}
	fread(&player, sizeof(struct Player), 1, fp);
	fclose(fp);
}

static void miao_talk()
{
	struct Characters *character = get_character("miao");
	if (character->talk_count == 0) {
		printf("喵喵喵？\n");
		while(1) {
			printf("1：？？？\n2：喵喵喵~\n3：……\n");
			printf("请选择：\n");
			char ch = getchar();
			getchar();

			switch(ch) {
				case '1':
					printf("它跑开了。\n");
					break;
				case '2':
					printf("喵~\n");
					character->talk_count++;
					break;
				case '3':
					player.player_score--;
					break;
				default:
					printf("未做出选择。");
			}
			return;
		}
	}
	else if (character->talk_count == 1) {
		printf("喵~喵~喵~\n");
	}
	else {
		printf("404\n");
	}
}

static void GraveKeeper_talk()
{
	struct Characters *character = get_character("gravekeeper");
	if (character->talk_count == 0 && search_item_from_inventory("candle") && item_on("candle")) {
		printf("“别打扰我！”老头说完便挥手熄灭了你手中的蜡烛，并打算将你赶出来。\n");
		set_item("candle");
		character->talk_count++;
		player.player_score++;
		set_location("湖边墓地");
	}
	else if (character->talk_count == 1) {
		printf("……\n");
	}
	else {
		printf("404\n");
	}
}
