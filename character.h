#ifndef _character_h
#define _character_h

struct Player
{
	int player_score;
	char item[10][20];
	char team[10][20];
};

struct Characters
{
	char zh_name[10];
	char name[30];
	char description[100];
	int talk_count;
	int team;
	int alive;
	int sad;
	int angry;
	int happy;
};

struct Map
{
	char key[8];
	char value[20];
	void (*talk)();
};

extern struct Player player;

extern struct Characters miao;
extern struct Characters GraveKeeper;

void miao_talk();
void GraveKeeper_talk();

#endif
