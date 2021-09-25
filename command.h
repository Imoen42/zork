#ifndef _command_h
#define _command_h

void init();
void free_mem();
void command_map(char *args0);
void command(char *args0, char *args1);

void set_location(char *args1);

int search_item_from_inventory(char *args1);

void set_item(char *args1);
int item_on(char *args1);

#endif
