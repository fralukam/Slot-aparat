#pragma once
#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DATA_FILE "igre.bin"
#define INITIAL_CAPACITY 16
#define KORISNIK_DUZINA 50

typedef enum { OPC_EXIT = 0, OPC_NEW_GAME = 1, OPC_PRINT = 2, OPC_SORT = 3, OPC_SEARCH = 4, OPC_DELETE = 5, OPC_UPDATE = 6, OPC_COPY_FILE = 7 } menu_option_t;

typedef struct {
    char ime[KORISNIK_DUZINA];
    char simboli[3][3];
    double dobitak;
} SlotGame;

extern SlotGame** games_list;
extern size_t games_count;
extern size_t games_capacity;

SlotGame* init_game(void);
void play_game(SlotGame* game, const char* korisnik);
int save_game(SlotGame* game);
int load_games_from_file(void);
void insert_game(SlotGame* game);
void print_games(void);
void delete_game_by_name(void);
void update_game(void);
void sort_games(void);
void search_game_by_dobitak(void);
void free_game(SlotGame* game);
void free_all_games(void);
int copy_file(const char* src, const char* dst);

void trim(char* str);

#endif
