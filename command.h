#ifndef _command_h
#define _command_h

extern enum Direction direction;

void reset_location();
void describe();
void room_move(enum Direction direction);
void inventory();
void search_command(char *args1, char *args2);

void save();
void load();

int search_inventory();
int search_item();
void turn_off_item(char *args);

#endif
