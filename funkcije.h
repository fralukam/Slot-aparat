#pragma once
#ifndef FUNKCIJE_H
#define FUNKCIJE_H

#include <stdio.h>

#define MAX_IGARA 100
#define KORISNIK_DUZINA 50

typedef struct {
    char ime[KORISNIK_DUZINA];
    char simboli[3][3];
    float dobitak;
} SlotGame;

extern SlotGame* sveIgre[MAX_IGARA];
extern int brojIgara;

SlotGame* initGame();
void playGame(SlotGame* game, const char* korisnik);
void saveGame(SlotGame* game);
void deleteGames();
void printGames();
void sortGames();
void searchGameByDobitak();
void quickSort(SlotGame* games, int low, int high);
int partition(SlotGame* games, int low, int high);
SlotGame* binarySearch(SlotGame* games, int low, int high, float target);
void freeGame(SlotGame* game);

#endif
