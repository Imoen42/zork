#include <stdio.h>
#include <string.h>
#include "command.h"

enum Direction {
    NORTH, SOUTH, EAST, WEST
};

int get_input()
{
	char input[256];
	char *args[128]; //命令行拆解成的各部分，以空指针结尾

	while(1) {
		printf("\033[1;31m> \033[0m");
		fflush(stdin);
		fgets(input, 256, stdin);
		int i; //清理结尾的换行符
		for (i = 0; input[i] != '\n'; i++)
			;
		input[i] = '\0';
        args[0] = input; //拆解命令行，例如输入命令为"ls -l /tmp"，则args[0]、args[1]、args[2]分别为ls、-l和/tmp
        for (i = 0; *args[i]; i++)
            for (args[i+1] = args[i] + 1; *args[i+1]; args[i+1]++)
                if (*args[i+1] == ' ') {
                    *args[i+1] = '\0';
                    args[i+1]++;
					break;
				}
		args[i] = NULL;

		if (!args[0]) {
			continue;
		}
		else if ( (strcmp(args[0], "use") == 0 || \
					strcmp(args[0], "take") == 0 || \
					strcmp(args[0], "drop") == 0 || \
					strcmp(args[0], "look") == 0 || \
					strcmp(args[0], "talk") == 0) && (args[1] != '\0') ) {
			search_command(args[0], args[1]);
			continue;
		}
		else if (strcmp(args[0], "n") == 0) {
			room_move(NORTH);
			continue;
		}
		else if (strcmp(args[0], "s") == 0) {
			room_move(SOUTH);
			continue;
		}
		else if (strcmp(args[0], "e") == 0) {
			room_move(EAST);
			continue;
		}
		else if (strcmp(args[0], "w") == 0) {
			room_move(WEST);
			continue;
		}
		else if (strcmp(args[0], "i") == 0) {
			inventory();
			continue;
		}
		else if (strcmp(args[0], "q") == 0) {
			return 0;
		}
		else if (strcmp(args[0], "look") == 0) {
			describe();
			continue;
		}
		else if (strcmp(args[0], "save") == 0) {
			save();
			continue;
		}
		else if (strcmp(args[0], "load") == 0) {
			load();
			continue;
		}
		else {
			printf("请重新输入\n");
		}
	}
}

int main(int argc, char *argv[])
{
	reset_location();
	get_input();
    return 0;
}
