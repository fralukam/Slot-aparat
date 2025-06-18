#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcije.h"

enum Opcije {
    IZLAZ, NOVA_IGRA, PRIKAZI_IGRE, SORTIRAJ, PRETRAZI, OBRISI
};

int main() {
    srand((unsigned int)time(NULL));
    SlotGame* trenutnaIgra = NULL;
    int izbor;

    do {
        printf("\n=== GLAVNI IZBORNIK ===\n");
        printf("1. Nova igra\n");
        printf("2. Prikazi igre\n");
        printf("3. Sortiraj igre\n");
        printf("4. Pretrazi igru po dobitku\n");
        printf("5. Obrisi sve igre\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");
        scanf("%d", &izbor);
        getchar();

        switch (izbor) {
        case NOVA_IGRA: {
            char korisnikIme[KORISNIK_DUZINA];
            printf("Unesite ime igraca: ");
            fgets(korisnikIme, KORISNIK_DUZINA, stdin);
            korisnikIme[strcspn(korisnikIme, "\n")] = '\0';

            trenutnaIgra = initGame();
            playGame(trenutnaIgra, korisnikIme);
            saveGame(trenutnaIgra);
            freeGame(trenutnaIgra);
            break;
        }
        case PRIKAZI_IGRE:
            printGames();
            break;
        case SORTIRAJ:
            sortGames();
            break;
        case PRETRAZI:
            searchGameByDobitak();
            break;
        case OBRISI:
            deleteGames();
            break;
        case IZLAZ:
            printf("Izlaz iz programa.\n");
            break;
        default:
            printf("Nepoznata opcija.\n");
        }

    } while (izbor != IZLAZ);

    return 0;
}
