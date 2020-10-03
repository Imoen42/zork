#include<stdio.h> 
#include "character.h"
#include "command.h"
#include "room.h"

struct Player player = {0, {}, {}};

struct Characters miao = {"喵", "miao", "喵喵喵~", 0, 1, 1, 0, 0, 100};
struct Characters GraveKeeper = {"守墓人", "gravekeeper", "一位白发苍苍的老者，总是低着头，无法看清容貌。", 0, 0, 1, 100, 0, 0};

void miao_talk()
{
	if (miao.talk_count == 0) {
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
					miao.talk_count++;
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
	else if (miao.talk_count == 1) {
		printf("喵~喵~喵~\n");
	}
	else {
		printf("404\n");
	}
}

void GraveKeeper_talk()
{
	if (GraveKeeper.talk_count == 0 && search_inventory("candle") && search_item("candle")) {
		printf("“别打扰我！”老头说完便挥手熄灭了你手中的蜡烛，并将你赶了出来。\n");
		turn_off_item("candle");
		GraveKeeper.talk_count++;
		player.player_score++;
		location = &LakesideGraveyard;
		describe();
	}
	else if (GraveKeeper.talk_count == 1) {
		printf("……\n");
	}
	else {
		printf("404\n");
	}
}
