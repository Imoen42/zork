#include <stdio.h>
#include <string.h>
#include "command.h"

//https://github.com/adoxa/ansicon并执行chcp 65001

int get_input()
{
	char input[256];
	char *args[128];//命令行拆解成的各部分,以空指针结尾

	for(;;) {
		printf("> ");
		fflush(stdin);
		fgets(input, 256, stdin);
		int i;//清理结尾的换行符
		for(i = 0; input[i] != '\n'; i++)
			;
		input[i] = '\0';
        args[0] = input;//拆解命令行，例如输入命令为"ls -l /tmp"，则args[0]、args[1]、args[2]分别为ls、-l和/tmp
        for(i = 0; *args[i]; i++)
            for(args[i+1] = args[i] + 1; *args[i+1]; args[i+1]++)
                if(*args[i+1] == ' ') {
                    *args[i+1] = '\0';
                    args[i+1]++;
					break;
				}
		args[i] = NULL;

		if(!args[0]) {
			continue;
		}
		else if(args[0] != '\0' && args[1] != '\0') {
			command(args[0], args[1]);
			continue;
		}
		else if(strcmp(args[0], "q") == 0) {
			return 0;
		}
		else if(args[0] != '\0') {
			command_map(args[0]);
			continue;
		}
		else {
			printf("请重新输入\n");
		}
	}
}

int main(int argc, char *argv[])
{
	init();
	get_input();
	free_mem();
    return 0;
}
