#ifndef _character_h
#define _character_h

void init_character();
void free_character();
void look_character(char *args1);
void talk_character(char *args1);
int search_character(char *args1);
char *get_character_string(char *update, char *args1);
int get_player_score();
void inventory();
int search_item_from_inventory(char *args1);
void add_item_to_inventory(char *update);
void delete_item_from_inventory(char *update);
void save_character();
void load_character();

#endif
