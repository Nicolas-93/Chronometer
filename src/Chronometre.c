#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include "Chronometre.h"
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>

#define REFRESH 50


Chronometer initialiser_chronometre() {
    Chronometer chrono = {
        .active = false,
        .duration_alert = 25,
        .total_ms = 0,
        .turn_index = 0,
        .turn_saved = 0,
    };
    memset(&(chrono.turns), 0, sizeof(int));
    return chrono;
}
void afficher_duree(int y, int x, int nb_ms) {
    FormattedTime d = ms_to_FormattedTime(nb_ms);
    mvprintw(y, x, "%d : %2d : %2d : %2d\n", d.hour, d.min, d.sec, d.cs);
}

void afficher_interface(Chronometer chrono) {
    mvprintw(0, COLS / 2, "== Chronometre ==\n");

    for (int i = 0; i < 3; ++i) {
        printw("Tour %d : ", i);
        afficher_duree(1 + i, getcurx(stdscr), chrono.turns[i]);
    }
    printw("Alerte : ");
    afficher_duree(getcury(stdscr), getcurx(stdscr), chrono.duration_alert);
    printw("Chrono : ");
    afficher_duree(getcury(stdscr), getcurx(stdscr), chrono.total_ms);

    static char* options[] = {
        "Espace : lancer / mettre en pause",
        "r : reinitialiser",
        "t : marquer tour",
        "F1/F2 : incrementer / decrementer heure avertissement",
        "F3/F4 : incrementer / decrementer minute avertissement",
        "F5/F6 : incrementer / decrementer seconde avertissement",
        "q : quitter"
    };
    for (int i = 6; i >= 0; --i) {
        mvprintw(LINES - i - 2, 0, options[6 - i]);
    }
}

void afficher_flash() {
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    int mode = 1;
    for(int i = 0; i < LINES - 1; ++i) {
        for(int j = 0; j < COLS - 1; ++j) {
            mode = (i + j) % 2 + 1;
            attron(COLOR_PAIR(mode));
            mvprintw(i, j, "*");
            attroff(COLOR_PAIR(mode));
        }
    }
}

void ajouter_tour(Chronometer* chrono) {
    chrono->turn_index++;
    chrono->turn_saved++;

    memmove(&(chrono->turns[1]), &(chrono->turns[0]), chrono->turn_saved * sizeof(int));

    chrono->turns[0] = chrono->total_ms;
    
    return;
}


int main(int argc, char* argv[]) {
    struct timeval debut, fin;
    long int duree_totale = 0;
    bool pause = true;
    int touche;
    
    initscr();
    //cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
    
    Chronometer chrono = initialiser_chronometre();

    while (1) {
        touche = getch();

        if (touche == ' ') {
            if (pause) {
                gettimeofday(&debut, NULL);
            }
            pause ^= 1;
        }
        else if (touche == 'r') {
            pause = true;
            duree_totale = 0;
            chrono = initialiser_chronometre();;
        }
        else if (touche == 't') {
            ajouter_tour(&chrono);
        }
        else if (touche == 'q') {
            break;
        }
        else if (touche == '5') {
            chrono.duration_alert += 1 * 1000;
        }
        else if (touche == '6') {
            chrono.duration_alert -= 1 * 1000;
        }
        else if (touche == '3') {
            chrono.duration_alert += 60 * 1000;
        }
        else if (touche == '4') {
            chrono.duration_alert -= 60 * 1000;
        }
        else if (touche == '1') {
            chrono.duration_alert += 60 * 60 * 1000;
        }
        else if (touche == '2') {
            chrono.duration_alert -= 60 * 60 * 1000;
        }
        usleep(REFRESH * 1000);
        if(!pause) {
            gettimeofday(&fin, NULL);
            duree_totale += intervalle_ms(debut, fin);
            debut = fin;
        }
        afficher_interface(chrono);
    }
    getch();
    endwin();
    return 0;
}
