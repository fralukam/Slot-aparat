#define _CRT_SECURE_NO_WARNINGS
#include "funkcije.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

SlotGame* sveIgre[MAX_IGARA] = { NULL };
int brojIgara = 0;

int compareDobitak(const void* a, const void* b) {
    const SlotGame* ga = *(const SlotGame**)a;
    const SlotGame* gb = *(const SlotGame**)b;
    if (!ga || !gb) return 0;
    return (ga->dobitak > gb->dobitak) - (ga->dobitak < gb->dobitak);
}

SlotGame* initGame() {
    return (SlotGame*)calloc(1, sizeof(SlotGame));
}

void playGame(SlotGame* game, const char* korisnik) {
    if (!game || !korisnik) return;
    strncpy(game->ime, korisnik, KORISNIK_DUZINA);

    float kreditiEUR;
    int ulogCent;

    printf("Unesite iznos kredita u EUR: ");
    scanf("%f", &kreditiEUR);
    printf("Unesite ulog po vrtnji (u centima, min 10, korak 10): ");
    scanf("%d", &ulogCent);
    getchar();

    if (ulogCent < 10 || ulogCent % 10 != 0) {
        printf("Neispravan unos uloga.\n");
        return;
    }

    float pocetniDobitak = kreditiEUR;
    float ukupniDobitak = 0.0f;
    int maxVrtnji = (int)((kreditiEUR * 100) / ulogCent);
    printf("Mozete odigrati %d vrtnji.\n", maxVrtnji);

    char simboli[] = { 'A', 'B', 'C', 'D', 'E', '7' };

    for (int v = 0; v < maxVrtnji; v++) {
        printf("\nPritisnite ENTER za vrtnju %d...\n", v + 1);
        getchar();

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                game->simboli[i][j] = simboli[rand() % 6];

        printf("=== PRIKAZ ===\n");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++)
                printf("[%c]", game->simboli[i][j]);
            printf("\n");
        }

        float dobitak = 0.0f;

        for (int i = 0; i < 3; i++) {
            char s0 = game->simboli[i][0];
            char s1 = game->simboli[i][1];
            char s2 = game->simboli[i][2];
            if (s0 == s1 && s1 == s2) {
                float faktor = 0.0f;
                switch (s0) {
                case 'A': case 'B': faktor = 2.0f; break;
                case 'C': faktor = 3.0f; break;
                case 'D': faktor = 4.0f; break;
                case 'E': faktor = 5.0f; break;
                case '7': faktor = 10.0f; break;
                }
                dobitak += faktor * ulogCent / 100.0f;
            }
        }

        if (game->simboli[0][0] == game->simboli[1][1] && game->simboli[1][1] == game->simboli[2][2]) {
            char s = game->simboli[1][1];
            float faktor = 0.0f;
            switch (s) {
            case 'A': case 'B': faktor = 2.0f; break;
            case 'C': faktor = 3.0f; break;
            case 'D': faktor = 4.0f; break;
            case 'E': faktor = 5.0f; break;
            case '7': faktor = 10.0f; break;
            }
            dobitak += faktor * ulogCent / 100.0f;
        }

        if (game->simboli[0][2] == game->simboli[1][1] && game->simboli[1][1] == game->simboli[2][0]) {
            char s = game->simboli[1][1];
            float faktor = 0.0f;
            switch (s) {
            case 'A': case 'B': faktor = 2.0f; break;
            case 'C': faktor = 3.0f; break;
            case 'D': faktor = 4.0f; break;
            case 'E': faktor = 5.0f; break;
            case '7': faktor = 10.0f; break;
            }
            dobitak += faktor * ulogCent / 100.0f;
        }

        if (dobitak > 0)
            printf("Dobili ste %.2f EUR!\n", dobitak);
        else
            printf("Bez dobitka.\n");

        ukupniDobitak += dobitak;
    }

    game->dobitak = ukupniDobitak;

    if (brojIgara < MAX_IGARA)
        sveIgre[brojIgara++] = game;

    printf("\nUkupni dobitak: %.2f EUR\n", ukupniDobitak);
    printf("Pocetni ulog: %.2f EUR\n", pocetniDobitak);
    if (ukupniDobitak > pocetniDobitak)
        printf("Bravo! U DOBITKU ste +%.2f EUR\n", ukupniDobitak - pocetniDobitak);
    else if (ukupniDobitak < pocetniDobitak)
        printf("Nazalost, u MINUSU ste -%.2f EUR\n", pocetniDobitak - ukupniDobitak);
    else
        printf("Na nuli ste.\n");
}

void saveGame(SlotGame* game) {
    if (!game) return;
    FILE* fp = fopen("igre.txt", "a");
    if (!fp) {
        perror("Greska kod otvaranja datoteke");
        return;
    }
    fprintf(fp, "Ime: %s | Dobitak: %.2f EUR\n", game->ime, game->dobitak);
    fclose(fp);
}

void deleteGames() {
    if (remove("igre.txt") == 0)
        printf("Sve igre su obrisane.\n");
    else
        perror("Greska pri brisanju");
    brojIgara = 0;
    for (int i = 0; i < MAX_IGARA; i++) {
        if (sveIgre[i]) {
            free(sveIgre[i]);
            sveIgre[i] = NULL;
        }
    }
}

void printGames() {
    FILE* fp = fopen("igre.txt", "r");
    if (!fp) {
        perror("Greska kod otvaranja datoteke");
        return;
    }
    char buffer[256];
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
    }
    fclose(fp);
}

void sortGames() {
    qsort(sveIgre, brojIgara, sizeof(SlotGame*), compareDobitak);
    printf("Igre sortirane po dobitku.\n");
}

void quickSort(SlotGame* games, int low, int high) {
    if (low < high) {
        int pi = partition(games, low, high);
        quickSort(games, low, pi - 1);
        quickSort(games, pi + 1, high);
    }
}

int partition(SlotGame* games, int low, int high) {
    float pivot = games[high].dobitak;
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (games[j].dobitak < pivot) {
            i++;
            SlotGame temp = games[i];
            games[i] = games[j];
            games[j] = temp;
        }
    }
    SlotGame temp = games[i + 1];
    games[i + 1] = games[high];
    games[high] = temp;
    return i + 1;
}

SlotGame* binarySearch(SlotGame* games, int low, int high, float target) {
    if (low > high) return NULL;
    int mid = (low + high) / 2;
    if (games[mid].dobitak == target)
        return &games[mid];
    else if (games[mid].dobitak > target)
        return binarySearch(games, low, mid - 1, target);
    else
        return binarySearch(games, mid + 1, high, target);
}

void searchGameByDobitak() {
    float target;
    printf("Unesite trazeni dobitak: ");
    scanf("%f", &target);

    SlotGame temp[MAX_IGARA];
    for (int i = 0; i < brojIgara; i++) {
        if (sveIgre[i]) temp[i] = *sveIgre[i];
    }

    quickSort(temp, 0, brojIgara - 1);
    SlotGame* rezultat = binarySearch(temp, 0, brojIgara - 1, target);

    if (rezultat)
        printf("Pronadena igra: %s s dobitkom %.2f EUR\n", rezultat->ime, rezultat->dobitak);
    else
        printf("Nema igre s trazenim dobitkom.\n");
}

void freeGame(SlotGame* game) {
    if (game) {
        free(game);
        game = NULL;
    }
}
