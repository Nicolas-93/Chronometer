#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include "Chronometre.h"

#define REFRESH 50
#define CENTER_X COLS / 2
#define CENTER_Y LINES / 2

#define STR_TITRE "== Chronometre =="
#define STR_TIME "%02d : %02d : %02d : %02d"


/**
 * @brief Initialise le chronomètre avec un avertissement
 * au bout de 25 secondes.
 * 
 * @return Chronometer 
 */
Chronometer initialiser_chronometre() {
    Chronometer chrono = {
        .active = false,
        .duration_alert = 25 * 1000,
        .total_ms = 0,
        .turn_index = 0,
        .turn_saved = 0,
    };
    memset(&(chrono.turns), 0, sizeof(int));
    return chrono;
}

/**
 * @brief Réinitialise le chronomètre en conservant
 * le réglage de l'avertissement.
 * 
 * @param chrono 
 */
void reset_chronometre(Chronometer* chrono) {
    time_t alerte = chrono->duration_alert;
    memset(chrono, 0, sizeof(Chronometer));
    chrono->duration_alert = alerte;
}

/**
 * @brief Affiche le temps en millisecondes passé en paramètre.
 * 
 * @param y 
 * @param x 
 * @param nb_ms 
 */
void afficher_duree(int y, int x, int nb_ms) {
    FormattedTime d = ms_to_FormattedTime(nb_ms);
    mvprintw(
        y, x,
        STR_TIME"\n", d.hour, d.min, d.sec, d.cs
    );
}

/**
 * @brief Affiche chacun des tours.
 * 
 * @param y 
 * @param x 
 * @param chrono 
 */
void afficher_tours(int y, int x, Chronometer chrono) {
    //int nb_lines = MIN(chrono.turn_saved , MIN((LINES - 10 - 3), 0));
    int nb_lignes = 5;
    int debut = MAX(1, chrono.turn_saved - nb_lignes);
    int fin = chrono.turn_saved;

    if (chrono.turn_saved == 0)
        return;

    // Affiche les tours de début à fin en numérotant les tours à partir de 1
    for (int i_tour = debut, i_print = 0; i_tour <= fin; ++i_tour, ++i_print) {
        mvprintw(y + i_print, CENTER_X - sizeof(STR_TIME), "Tour %1d : ", i_tour);
        afficher_duree(
            getcury(stdscr), CENTER_X - sizeof(STR_TIME) / 2,
            chrono.turns[chrono.turn_saved - i_tour]
        );
    }
}

void afficher_interface(Chronometer chrono) {
    mvprintw(0, CENTER_X - sizeof(STR_TITRE) / 2, STR_TITRE);

    afficher_tours(1, 0, chrono);

    mvprintw(15, CENTER_X - sizeof(STR_TIME) / 2, "Alerte : ");
    afficher_duree(getcury(stdscr), getcurx(stdscr), chrono.duration_alert);
    mvprintw(16, CENTER_X - sizeof(STR_TIME) / 2, "Chrono : ");
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
/**
 * @brief Ajoute un tour au chronometre et décale les autres tours.
 * 
 * @param chrono 
 */
void ajouter_tour(Chronometer* chrono) {
    chrono->turn_saved++;
    if (chrono->turn_index < MAX_TURNS - 1) {
        chrono->turn_index++;
    }

    for (int i = chrono->turn_index; i > 0; --i) {
        chrono->turns[i] = chrono->turns[i - 1];
    }
    chrono->turns[0] = chrono->total_ms;

    return;
}

/**
 * @brief Gère les touches de paramétrage de l'avertissement.
 * 
 * @param chrono 
 * @param touche 
 * @return int 
 */
int alert_keymap(Chronometer* chrono, int touche) {
    switch (touche) {
    case '1':
    case KEY_F(1):
        chrono->duration_alert += 60 * 60 * 1000;
        break;
    case '2':
    case KEY_F(2):
        chrono->duration_alert -= 60 * 60 * 1000;
        break;
    case '3':
    case KEY_F(3):
        chrono->duration_alert += 60 * 1000;
        break;
    case '4':
    case KEY_F(4):
        chrono->duration_alert -= 60 * 1000;
        break;
    case '5':
    case KEY_F(5):
        chrono->duration_alert += 1 * 1000;
        break;
    case '6':
    case KEY_F(6):
        chrono->duration_alert -= 1 * 1000;
        break;
    default:
        return 0;
    }
    return 1;
}

int main(int argc, char* argv[]) {
    struct timeval debut, fin;
    bool pause = true;
    bool quit = false;
    int touche;
    
    initscr();
    //cbreak();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
    
    Chronometer chrono = initialiser_chronometre();

    while (!quit) {
        touche = getch();

        switch (touche) {
        case ' ':            
            if (pause) {
                gettimeofday(&debut, NULL);
            }
            pause ^= 1;
            break;
        case 'r':
            pause = true;
            reset_chronometre(&chrono);
            break;
        case 't':
            ajouter_tour(&chrono);
            break;
        case 'q':
            quit = true;
            break;
        default:
            alert_keymap(&chrono, touche);
        case KEY_RESIZE:
            clear();
        }

        usleep(REFRESH * 1000);
        if(!pause) {
            gettimeofday(&fin, NULL);
            chrono.total_ms += intervalle_ms(debut, fin);
            debut = fin;
        }
        afficher_interface(chrono);
    }
    getch();
    endwin();
    return 0;
}
