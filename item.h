#ifndef _item_h
#define _item_h

struct Items
{
    char zh_name[10];
    char name[12];
    char description[500];
	int player_score;
    int turn_on;
	char turn_on_description[50];
	char turn_off_description[50];
	char fail[60];
};

extern struct Items candle;
extern struct Items violin;
extern struct Items erhu;

#endif
