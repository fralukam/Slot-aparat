#define _CRT_SECURE_NO_WARNINGS
#include "funkcije.h"

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

SlotGame** games_list = NULL;
size_t games_count = 0;
size_t games_capacity = 0;

static int stricmp_fallback(const char* a, const char* b) {
#ifdef _WIN32
    return _stricmp(a, b);
#else
    while (*a && *b) {
        unsigned char ca = (unsigned char)tolower((unsigned char)*a);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b);
        if (ca != cb) return (int)ca - (int)cb;
        a++; b++;
    }
    return (int)(unsigned char)*a - (int)(unsigned char)*b;
#endif
}

static inline double abs_double(double x) { return x < 0 ? -x : x; }

static void ensure_capacity(void) {
    if (games_capacity == 0) {
        games_capacity = INITIAL_CAPACITY;
        games_list = (SlotGame**)calloc(games_capacity, sizeof(SlotGame*));
        if (!games_list) { perror("calloc games_list"); exit(EXIT_FAILURE); }
    }
    else if (games_count >= games_capacity) {
        size_t new_cap = games_capacity * 2;
        SlotGame** tmp = (SlotGame**)realloc(games_list, new_cap * sizeof(SlotGame*));
        if (!tmp) { perror("realloc games_list"); exit(EXIT_FAILURE); }
        for (size_t i = games_capacity; i < new_cap; ++i) tmp[i] = NULL;
        games_list = tmp;
        games_capacity = new_cap;
    }
}

void trim(char* str) {
    if (!str || *str == '\0') return;
    char* start = str;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace((unsigned char)*end)) { *end = '\0'; end--; }
}

SlotGame* init_game(void) {
    SlotGame* g = (SlotGame*)calloc(1, sizeof(SlotGame));
    if (!g) { perror("calloc SlotGame"); return NULL; }
    g->dobitak = 0.0;
    memset(g->ime, 0, sizeof(g->ime));
    memset(g->simboli, 0, sizeof(g->simboli));
    return g;
}

void insert_game(SlotGame* game) {
    if (!game) return;
    ensure_capacity();
    games_list[games_count++] = game;
}

void play_game(SlotGame* game, const char* korisnik) {
    if (!game || !korisnik) return;
    strncpy(game->ime, korisnik, KORISNIK_DUZINA - 1);
    game->ime[KORISNIK_DUZINA - 1] = '\0';

    double kreditiEUR = 0.0;
    int ulogCent = 0;

    printf("Unesite iznos kredita u EUR: ");
    if (scanf("%lf", &kreditiEUR) != 1 || kreditiEUR <= 0.0) {
        printf("Neispravan unos kredita.\n");
        while (getchar() != '\n');
        return;
    }
    printf("Unesite ulog po vrtnji (u centima, min 10, korak 10): ");
    if (scanf("%d", &ulogCent) != 1) {
        printf("Neispravan unos uloga.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    if (ulogCent < 10 || ulogCent % 10 != 0) {
        printf("Neispravan unos uloga.\n");
        return;
    }

    double pocetniDobitak = kreditiEUR;
    double ukupniDobitak = 0.0;
    int maxVrtnji = (int)((kreditiEUR * 100.0) / (double)ulogCent);
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

        double dobitak = 0.0;

        for (int i = 0; i < 3; i++) {
            char s0 = game->simboli[i][0];
            char s1 = game->simboli[i][1];
            char s2 = game->simboli[i][2];
            if (s0 == s1 && s1 == s2) {
                double faktor = 0.0;
                switch (s0) {
                case 'A': case 'B': faktor = 2.0; break;
                case 'C': faktor = 3.0; break;
                case 'D': faktor = 4.0; break;
                case 'E': faktor = 5.0; break;
                case '7': faktor = 10.0; break;
                }
                dobitak += faktor * (double)ulogCent / 100.0;
            }
        }

        if (game->simboli[0][0] == game->simboli[1][1] && game->simboli[1][1] == game->simboli[2][2]) {
            char s = game->simboli[1][1];
            double faktor = 0.0;
            switch (s) {
            case 'A': case 'B': faktor = 2.0; break;
            case 'C': faktor = 3.0; break;
            case 'D': faktor = 4.0; break;
            case 'E': faktor = 5.0; break;
            case '7': faktor = 10.0; break;
            }
            dobitak += faktor * (double)ulogCent / 100.0;
        }

        if (game->simboli[0][2] == game->simboli[1][1] && game->simboli[1][1] == game->simboli[2][0]) {
            char s = game->simboli[1][1];
            double faktor = 0.0;
            switch (s) {
            case 'A': case 'B': faktor = 2.0; break;
            case 'C': faktor = 3.0; break;
            case 'D': faktor = 4.0; break;
            case 'E': faktor = 5.0; break;
            case '7': faktor = 10.0; break;
            }
            dobitak += faktor * (double)ulogCent / 100.0;
        }

        if (dobitak > 0)
            printf("Dobili ste %.2f EUR!\n", dobitak);
        else
            printf("Bez dobitka.\n");

        ukupniDobitak += dobitak;
    }

    game->dobitak = ukupniDobitak;

    printf("\nUkupni dobitak: %.2f EUR\n", ukupniDobitak);
    printf("Pocetni ulog: %.2f EUR\n", pocetniDobitak);
    if (ukupniDobitak > pocetniDobitak)
        printf("Bravo! U DOBITKU ste +%.2f EUR\n", ukupniDobitak - pocetniDobitak);
    else if (ukupniDobitak < pocetniDobitak)
        printf("Nazalost, u MINUSU ste -%.2f EUR\n", pocetniDobitak - ukupniDobitak);
    else
        printf("Na nuli ste.\n");
}

int save_game(SlotGame* game) {
    if (!game) return -1;
    FILE* fp = fopen(DATA_FILE, "ab");
    if (!fp) { perror("fopen save_game"); return -1; }

    if (fwrite(game, sizeof(SlotGame), 1, fp) != 1) {
        perror("fwrite save_game");
        fclose(fp);
        return -1;
    }
    if (fclose(fp) != 0) { perror("fclose save_game"); return -1; }
    return 0;
}

int load_games_from_file(void) {
    FILE* fp = fopen(DATA_FILE, "rb");
    if (!fp) { return -1; }

    free_all_games();

    if (fseek(fp, 0, SEEK_END) != 0) { perror("fseek"); fclose(fp); return -1; }
    long filesize = ftell(fp);
    if (filesize < 0) { perror("ftell"); fclose(fp); return -1; }
    rewind(fp);

    size_t count = (size_t)(filesize / sizeof(SlotGame));
    for (size_t i = 0; i < count; ++i) {
        SlotGame* g = init_game();
        if (!g) { fclose(fp); return -1; }
        if (fread(g, sizeof(SlotGame), 1, fp) != 1) {
            if (ferror(fp)) perror("fread");
            free(g);
            fclose(fp);
            return -1;
        }
        insert_game(g);
    }

    fclose(fp);
    return 0;
}

void print_games(void) {
    if (games_count == 0) { printf("Nema spremljenih igara.\n"); return; }
    printf("=== Spremjene igre ===\n");
    for (size_t i = 0; i < games_count; ++i) {
        SlotGame* g = games_list[i];
        if (g) printf("%zu) Ime: %s | Dobitak: %.2f EUR\n", i + 1, g->ime, g->dobitak);
    }
}

static int compare_by_dobitak(const void* a, const void* b) {
    const SlotGame* A = *(const SlotGame**)a;
    const SlotGame* B = *(const SlotGame**)b;
    if (!A || !B) return 0;
    if (A->dobitak < B->dobitak) return -1;
    if (A->dobitak > B->dobitak) return 1;
    return 0;
}

void sort_games(void) {
    if (games_count <= 1) { printf("Nema dovoljno igara za sortiranje.\n"); return; }
    qsort(games_list, games_count, sizeof(SlotGame*), compare_by_dobitak);
    printf("Igre sortirane po dobitku.\n");
}

static int binary_search_recursive(double target, size_t left, size_t right) {
    if (left > right) return -1;
    size_t mid = left + (right - left) / 2;
    double val = games_list[mid]->dobitak;
    const double EPS = 0.005;
    if (abs_double(val - target) <= EPS) return (int)mid;
    if (val < target) {
        if (mid + 1 > right) return -1;
        return binary_search_recursive(target, mid + 1, right);
    }
    else {
        if (mid == 0) return -1;
        return binary_search_recursive(target, left, mid - 1);
    }
}

void search_game_by_dobitak(void) {
    if (games_count == 0) { printf("Nema spremljenih igara.\n"); return; }
    double target;
    printf("Unesite trazeni dobitak: ");
    if (scanf("%lf", &target) != 1) { printf("Neispravan unos.\n"); while (getchar() != '\n'); return; }
    while (getchar() != '\n');

    qsort(games_list, games_count, sizeof(SlotGame*), compare_by_dobitak);

    int idx = binary_search_recursive(target, 0, games_count - 1);
    if (idx >= 0) {
        printf("Pronadena igra: %s s dobitkom %.2f EUR (index %d)\n", games_list[idx]->ime, games_list[idx]->dobitak, idx);
    }
    else {
        printf("Nema igre s trazenim dobitkom (u toleranciji).\n");
    }
}

void delete_game_by_name(void) {
    char ime[KORISNIK_DUZINA];
    printf("Unesite ime igraca za brisanje: ");
    if (!fgets(ime, sizeof(ime), stdin)) { printf("Neispravan unos.\n"); return; }
    ime[strcspn(ime, "\n")] = '\0';
    trim(ime);

    if (games_count == 0) { printf("Nema igara za brisanje.\n"); return; }

    int found = 0;
    for (size_t i = 0; i < games_count; ++i) {
        if (games_list[i] && stricmp_fallback(games_list[i]->ime, ime) == 0) {
            free_game(games_list[i]);
            for (size_t j = i; j + 1 < games_count; ++j) games_list[j] = games_list[j + 1];
            games_list[games_count - 1] = NULL;
            games_count--;
            found = 1;
            break;
        }
    }

    if (!found) { printf("Igra s imenom '%s' nije pronadena.\n", ime); return; }

    FILE* tmp = fopen("temp.bin", "wb");
    if (!tmp) { perror("fopen temp"); return; }
    for (size_t i = 0; i < games_count; ++i) {
        if (fwrite(games_list[i], sizeof(SlotGame), 1, tmp) != 1) { perror("fwrite temp"); fclose(tmp); return; }
    }
    if (fclose(tmp) != 0) perror("fclose temp");

    if (remove(DATA_FILE) != 0 && errno != ENOENT) { perror("remove data file"); }
    if (rename("temp.bin", DATA_FILE) != 0) { perror("rename temp -> data"); }

    printf("Igra igraca '%s' uspjesno obrisana.\n", ime);
}

void update_game(void) {
    char ime[KORISNIK_DUZINA];
    printf("Unesite ime igraca ciju igru zelite azurirati: ");
    if (!fgets(ime, sizeof(ime), stdin)) { printf("Neispravan unos.\n"); return; }
    ime[strcspn(ime, "\n")] = '\0';
    trim(ime);

    if (games_count == 0) { printf("Nema igara za azuriranje.\n"); return; }

    int found = 0;
    for (size_t i = 0; i < games_count; ++i) {
        if (games_list[i] && stricmp_fallback(games_list[i]->ime, ime) == 0) {
            found = 1;
            char novoIme[KORISNIK_DUZINA];
            printf("Unesite novo ime (ENTER za ostaviti isto): ");
            if (!fgets(novoIme, sizeof(novoIme), stdin)) { printf("Neispravan unos.\n"); return; }
            novoIme[strcspn(novoIme, "\n")] = '\0';
            trim(novoIme);
            if (strlen(novoIme) > 0) {
                strncpy(games_list[i]->ime, novoIme, KORISNIK_DUZINA - 1);
                games_list[i]->ime[KORISNIK_DUZINA - 1] = '\0';
            }

            printf("Unesite novi dobitak (npr. 12.50): ");
            double noviDobitak;
            if (scanf("%lf", &noviDobitak) != 1) {
                printf("Neispravan unos dobitka.\n");
                while (getchar() != '\n');
                return;
            }
            while (getchar() != '\n');
            games_list[i]->dobitak = noviDobitak;

            break;
        }
    }

    if (!found) { printf("Igrac s imenom '%s' nije pronaden.\n", ime); return; }

    FILE* tmp = fopen("temp.bin", "wb");
    if (!tmp) { perror("fopen temp for update"); return; }
    for (size_t i = 0; i < games_count; ++i) {
        if (fwrite(games_list[i], sizeof(SlotGame), 1, tmp) != 1) { perror("fwrite temp update"); fclose(tmp); return; }
    }
    if (fclose(tmp) != 0) perror("fclose temp update");

    if (remove(DATA_FILE) != 0 && errno != ENOENT) { perror("remove data file"); }
    if (rename("temp.bin", DATA_FILE) != 0) { perror("rename temp -> data"); }

    printf("Igra uspjesno azurirana.\n");
}

void free_game(SlotGame* game) {
    if (!game) return;
    free(game);
}

void free_all_games(void) {
    if (games_list) {
        for (size_t i = 0; i < games_count; ++i) {
            if (games_list[i]) { free_game(games_list[i]); games_list[i] = NULL; }
        }
        free(games_list);
        games_list = NULL;
    }
    games_count = 0;
    games_capacity = 0;
}

int copy_file(const char* src, const char* dst) {
    FILE* in = fopen(src, "rb");
    if (!in) return -1;
    if (fseek(in, 0, SEEK_END) != 0) { fclose(in); return -1; }
    long size = ftell(in);
    rewind(in);

    FILE* out = fopen(dst, "wb");
    if (!out) { fclose(in); return -1; }

    char buffer[4096];
    size_t n;
    while ((n = fread(buffer, 1, sizeof(buffer), in)) > 0) {
        if (fwrite(buffer, 1, n, out) != n) { fclose(in); fclose(out); return -1; }
    }

    fclose(in);
    fclose(out);
    return 0;
}

