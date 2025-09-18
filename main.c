#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "funkcije.h"

int main(void) {
    srand((unsigned int)time(NULL));

    if (load_games_from_file() != 0) {
        printf("Nema spremljenih igara ili datoteka ne postoji. Nastavljam s praznom listom.\n");
    }

    int izbor = -1;
    do {
        printf("\n=== LUCKY LUKE'S CHARM ===\n");
        printf("1. Nova igra\n");
        printf("2. Prikazi igre\n");
        printf("3. Sortiraj igre (po dobitku)\n");
        printf("4. Pretrazi igru po dobitku\n");
        printf("5. Obrisi igru po imenu\n");
        printf("6. Azuriraj igru po imenu\n");
        printf("7. Kopiraj datoteku s igrama (backup)\n");
        printf("0. Izlaz\n");
        printf("Odabir: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos. Pokusajte ponovno.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        switch (izbor) {
        case OPC_EXIT:
            printf("Izlaz iz programa.\n");
            break;
        case OPC_NEW_GAME: {
            char ime[KORISNIK_DUZINA];
            printf("Unesite ime igraca: ");
            if (!fgets(ime, sizeof(ime), stdin)) { printf("Neispravan unos imena.\n"); break; }
            ime[strcspn(ime, "\n")] = '\0';
            trim(ime);

            SlotGame* g = init_game();
            if (!g) { perror("Alokacija igre"); break; }
            play_game(g, ime);
            if (save_game(g) != 0) {
                printf("Upozorenje: nije uspjelo spremanje igre u datoteku.\n");
            }
            insert_game(g);
            break;
        }
        case OPC_PRINT:
            print_games();
            break;
        case OPC_SORT:
            sort_games();
            break;
        case OPC_SEARCH:
            search_game_by_dobitak();
            break;
        case OPC_DELETE:
            delete_game_by_name();
            break;
        case OPC_UPDATE:
            update_game();
            break;
        case OPC_COPY_FILE: {
            if (copy_file(DATA_FILE, "igre_backup.bin") == 0)
                printf("Datoteka uspjesno kopirana u 'igre_backup.bin'.\n");
            else
                printf("Kopiranje datoteke nije uspjelo.\n");
            break;
        }
        default:
            printf("Nepoznata opcija.\n");
        }

    } while (izbor != OPC_EXIT);

    free_all_games();
    return 0;
}
