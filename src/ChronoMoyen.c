#include <sys/time.h>
#include <ncurses.h>
#include <stdbool.h>
#include "TimeUtils/TimeUtils.h"
#include <unistd.h>
#define REFRESH 1


void print_time(Interval d) {
    mvprintw(0, 0, "%d : %2d : %2d : %2d\n", d.hour, d.min, d.sec, d.cs);
}

/**
 * @brief 
 * 
 */
void add_to_chrono(Chrono* chrono, time_t tps_ms) {
    chrono->cs += nb_ms_vers_centiemes(tps_ms);
    chrono->sec += nb_ms_vers_secondes(tps_ms);
    chrono->min += nb_ms_vers_minutes(tps_ms);
    chrono->hour += nb_ms_vers_heures(tps_ms);
}

int main(int argc, char* argv[]) {
    struct timeval debut, fin;
    long int duree_totale = 0;
    bool pause = true;
    int touche;
    Chrono chrono = {0, 0, 0, 0};

    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(FALSE);

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
        }
        usleep(REFRESH * 1000);
        if(!pause) {
            gettimeofday(&fin, NULL);
            duree_totale += intervalle_ms(debut, fin);
            debut = fin;
        }
        //add_to_chrono(&chrono, duree_totale);
        chrono = ms_to_Interval(duree_totale);
        print_time(chrono);            
        if (touche == 'q')
            break;
    }
    getch();
    endwin();

    return 0;
}