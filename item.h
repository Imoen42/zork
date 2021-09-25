#ifndef _item_h
#define _item_h

void init_item();
void free_item();
int search_item(char *args1);
char *get_item_string(char *update, char *args1);
void look_item(char *args1);
void use_item(int player_score, char *args1);
void set_item(char *args1);
int item_on(char *args1);

#endif
