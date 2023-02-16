#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdbool.h>
#include "Chronometre.h"

#define REFRESH 10 // ms
#define CENTER_X COLS / 2
#define CENTER_Y LINES / 2

#define STR_TITRE "== Chronometre =="
// #define STR_TIME "%02d : %02d : %02d : %02d"
#define STR_TIME "%2d : %2d : %2d : %2d"
#define STR_ALERTE "Alerte : "
#define STR_CHRONO "Chrono : "
#define STR_TOUR "Tour %2d : "
#define NB_PRINT_TURNS 5
#define FLASH_REPEAT 5


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
        .alerte_flag = true,
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
    chrono->alerte_flag = true;
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
 * @brief Calcule la taille d'affichage d'une durée.
 * Calcule une seule fois pour toute l'exécution la taille
 * du texte formaté. Il serait possible de la modifier
 * pour gérer plus de 99 heures.
 * 
 * @param nb_ms Nombre de millisecondes.
 * 
 */
int len_affichage_duree(int nb_ms) {
    static int len_strtime = 0;
    if (!len_strtime) {
        FormattedTime d = ms_to_FormattedTime(nb_ms);
        len_strtime = snprintf(NULL, 0, STR_TIME"\n", d.hour, d.min, d.sec, d.cs) - 1;
    }
    return len_strtime;
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
    int nb_lignes = NB_PRINT_TURNS - 1;
    int debut = MAX(1, chrono.turn_saved - nb_lignes) + chrono.scroll;
    int fin = chrono.turn_saved + chrono.scroll;

    // mvprintw(LINES - 1, COLS - 40, "debut=%3d, fin=%3d, scroll=%3d", debut, fin, chrono.scroll);

    if (chrono.turn_saved == 0)
        return;

    // Affiche les tours de début à fin en numérotant les tours à partir de 1
    for (int i_tour = debut, i_print = 0; i_tour <= fin; ++i_tour, ++i_print) {
        int turn_ms = chrono.turns[chrono.turn_saved - i_tour];
        int len_strtime = len_affichage_duree(turn_ms);
        mvprintw(
            y + i_print,
            CENTER_X - (len_strtime + sizeof(STR_TOUR)) / 2,
            STR_TOUR, i_tour
        );
        afficher_duree(
            getcury(stdscr), getcurx(stdscr),
            turn_ms
        );
    }
}

void afficher_interface(Chronometer chrono) {
    int len_strtime = len_affichage_duree(chrono.duration_alert);
    
    mvprintw(0, CENTER_X - sizeof(STR_TITRE) / 2, STR_TITRE);

    afficher_tours(1, 0, chrono);

    mvprintw(
        8, CENTER_X - (len_strtime + sizeof(STR_ALERTE)) / 2,
        STR_ALERTE
    );
    afficher_duree(getcury(stdscr), getcurx(stdscr), chrono.duration_alert);
    mvprintw(
        9, CENTER_X - (len_strtime + sizeof(STR_CHRONO)) / 2,
        STR_CHRONO
    );
    afficher_duree(getcury(stdscr), getcurx(stdscr), chrono.total_ms);

    static const char* options[] = {
        "Espace : lancer / mettre en pause",
        "r : reinitialiser",
        "t : marquer tour",
        "F1/F2 : incrementer / decrementer heure avertissement",
        "F3/F4 : incrementer / decrementer minute avertissement",
        "F5/F6 : incrementer / decrementer seconde avertissement",
        "q : quitter"
    };
    for (int i = 6; i >= 0; --i) {
        mvprintw(LINES - i - 1, 0, "%s", options[6 - i]);
    }
}

void afficher_flash(Chronometer chrono) {
    if(chrono.alerte_flag == false)
        return;

    init_pair(1, COLOR_GREEN, COLOR_GREEN);
    init_pair(2, COLOR_RED, COLOR_RED);
    int mode = 1;
    int inner_mode = 1;

    for (int r = 0; r < FLASH_REPEAT; ++r) {    
        mode = r % 2 + 1;
        for(int i = 0; i < LINES - 1; ++i) {
            for(int j = 0; j < COLS - 1; ++j) {
                inner_mode = ((i + j) + mode) % 2 + 1;
                attron(COLOR_PAIR(inner_mode));
                mvprintw(i, j, "*");
                attroff(COLOR_PAIR(inner_mode));
            }
        }
        refresh();
        usleep(500000);
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
    chrono->alerte_flag = true;
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
    start_color();
    
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
            clear();
            break;
        case 't':
            ajouter_tour(&chrono);
            chrono.scroll = 0;
            break;
        case 'u':
            if (chrono.scroll < 0)
                chrono.scroll += 1;
            break;
        case 'd':
            if (ABS(chrono.scroll) < chrono.turn_index - NB_PRINT_TURNS)
                chrono.scroll -= 1;
            break;
        case 'q':
            quit = true;
            break;
        case KEY_RESIZE:
            clear();
            break;
        default:
            alert_keymap(&chrono, touche);
        }

        if(chrono.total_ms >= chrono.duration_alert && chrono.alerte_flag) {
            afficher_flash(chrono);
            chrono.alerte_flag = false;
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
