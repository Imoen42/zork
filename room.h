#ifndef _room_h
#define _room_h

enum Direction {
    NORTH, SOUTH, EAST, WEST
};

extern enum Direction direction;
void init_room();
void free_room();
void reset_location();
void set_location(char *args1);
void describe();
void room_move(enum Direction direction);
int search_item_from_room(char *args1);
int search_character_from_room(char *args1);
void add_item_to_room(char *update);
void delete_item_from_room(char *args1);
void add_character_to_room(char *update);
void save_room();
void load_room();

#endif
