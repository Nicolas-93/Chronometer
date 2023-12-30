#define _DEFAULT_SOURCE

#include "Chronometer.h"
#include <ncurses.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#define REFRESH 10 // ms
#define CENTER_X (COLS / 2)
#define CENTER_Y (LINES / 2)

#define STR_TITLE "== Chronometer =="
// #define STR_TIME "%02d : %02d : %02d : %02d"
#define STR_TIME "%2d : %2d : %2d : %2d"
#define STR_ALERTE "Alert : "
#define STR_CHRONO "Chrono : "
#define STR_TOUR "Tour %2d : "
#define STR_ERR_TOO_SMALL_WINDOW                                               \
    "Window is too small to display the chronometer"
#define NB_MAX_PRINT_TURNS 6
#define FLASH_REPEAT 5

const int OPTS_LEN = 8;
const char* OPTS_STRINGS[] = {
    "Espace : Pause / Resume",
    "r : Reset",
    "t : New turn",
    "Up/Down: Scroll over laps",
    "F1/F2 : alarm hour",
    "F3/F4 : alarm minute",
    "F5/F6 : alarm second",
    "q : quit",
};

typedef enum Error {
    ERR_NONE,
    ERR_EXIT,
    ERR_TOO_SMALL_WINDOW,
} Error;

typedef enum ColorPair {
    PAIR_TITLE = 1,
    PAIR_ALERT = 1,
    PAIR_TURN,
    PAIR_DURATION1,
    PAIR_DURATION2,
    PAIR_RT_CHRONO,
    PAIR_OPTIONS,
    PAIR_FLASH1,
    PAIR_FLASH2,
    PAIR_FLASH3,
} ColorPair;

/**
 * @brief Initialize chronometer
 *
 * @return Chronometer
 */
Chronometer init_chronometer(void) {
    Chronometer chrono = {
        .active = false,
        .alert = 25 * 1000,
        .total_ms = 0,
        .turns = {0},
        .turn_index = 0,
        .turn_saved = 0,
        .alerte_flag = true,
    };
    return chrono;
}

/**
 * @brief Reset chronometer while keeping alert setting
 *
 * @param chrono
 */
void reset_chronometer(Chronometer* chrono) {
    time_t alerte = chrono->alert;
    *chrono = (Chronometer) {
        .alert = alerte,
        .alerte_flag = true,
    };
}

void print_time(int y, int x, int nb_ms) {
    FormattedTime d = ms_to_FormattedTime(nb_ms);
    mvprintw(y, x, STR_TIME "\n", d.hour, d.min, d.sec, d.cs);
}

/**
 * @brief Returns required length to print a time
 *
 */
int get_time_formatted_len(int nb_ms) {
    static int len_strtime = 0;
    if (!len_strtime) {
        FormattedTime d = ms_to_FormattedTime(nb_ms);
        len_strtime =
            snprintf(NULL, 0, STR_TIME "\n", d.hour, d.min, d.sec, d.cs) - 1;
    }
    return len_strtime;
}

void print_laps(int y, int x, Chronometer chrono) {
    int nb_lines =
        MIN(NB_MAX_PRINT_TURNS - 1, LINES - OPTS_LEN - NB_MAX_PRINT_TURNS);
    int start = MAX(1, chrono.turn_saved - nb_lines) + chrono.scroll;
    int end = chrono.turn_saved + chrono.scroll;
    enum ColorPair color_tour = PAIR_DURATION1;

    if (chrono.turn_saved == 0)
        return;

    // Print laps, numbered from 1
    for (int i_tour = start, i_print = 0; i_tour <= end; ++i_tour, ++i_print) {
        int turn_ms = chrono.turns[chrono.turn_saved - i_tour];
        int len_strtime = get_time_formatted_len(turn_ms);
        color_tour = i_print % 2 + PAIR_DURATION1;

        attron(COLOR_PAIR(PAIR_TURN));
        mvprintw(
            y + i_print,
            CENTER_X - (len_strtime + sizeof(STR_TOUR)) / 2,
            STR_TOUR,
            i_tour);
        attroff(COLOR_PAIR(PAIR_TURN));

        attron(COLOR_PAIR(color_tour) ^ A_BOLD);
        print_time(getcury(stdscr), getcurx(stdscr), turn_ms);
        attroff(COLOR_PAIR(color_tour) ^ A_BOLD);
    }
}

/**
 * @brief Print 'c' character n times
 */
void print_characters(int y, int x, int n, char c) {
    for (int i = 0; i < n; ++i)
        mvaddch(y, x + i, c);
}

void init_colors_pairs(void) {
    init_pair(PAIR_FLASH1, COLOR_GREEN, COLOR_GREEN);
    init_pair(PAIR_FLASH2, COLOR_RED, COLOR_RED);
    init_pair(PAIR_FLASH3, COLOR_BLUE, COLOR_BLUE);
    init_pair(PAIR_TITLE, COLOR_RED, COLOR_BLACK);
    init_pair(PAIR_TURN, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(PAIR_OPTIONS, COLOR_BLUE, COLOR_BLACK);
    init_pair(PAIR_DURATION1, COLOR_GREEN, COLOR_BLACK);
    init_pair(PAIR_DURATION2, COLOR_CYAN, COLOR_BLACK);
    init_pair(PAIR_RT_CHRONO, COLOR_YELLOW, COLOR_BLACK);
}

void show_tui(Chronometer chrono) {
    int len_strtime = get_time_formatted_len(chrono.alert);

    attron(COLOR_PAIR(PAIR_TITLE) ^ A_BOLD);
    mvprintw(0, CENTER_X - sizeof(STR_TITLE) / 2, STR_TITLE);
    attroff(COLOR_PAIR(PAIR_TITLE) ^ A_BOLD);

    print_laps(1, 0, chrono);

    print_characters(LINES - OPTS_LEN - 4, 0, COLS, '-');

    mvprintw(
        LINES - OPTS_LEN - 3,
        CENTER_X - (len_strtime + sizeof(STR_CHRONO)) / 2,
        STR_CHRONO);

    attron(COLOR_PAIR(PAIR_RT_CHRONO) ^ A_BOLD);
    print_time(getcury(stdscr), getcurx(stdscr), chrono.total_ms);
    attroff(COLOR_PAIR(PAIR_RT_CHRONO) ^ A_BOLD);

    mvprintw(
        getcury(stdscr),
        CENTER_X - (len_strtime + sizeof(STR_ALERTE)) / 2,
        STR_ALERTE);

    attron(COLOR_PAIR(PAIR_ALERT));
    print_time(getcury(stdscr), getcurx(stdscr), chrono.alert);
    attroff(COLOR_PAIR(PAIR_ALERT));

    print_characters(LINES - OPTS_LEN - 1, 0, COLS, '-');
    attron(COLOR_PAIR(PAIR_OPTIONS) ^ A_ITALIC);
    for (int i = OPTS_LEN - 1; i >= 0; --i) {
        mvprintw(LINES - i - 1, 0, "%s", OPTS_STRINGS[OPTS_LEN - 1 - i]);
    }
    attroff(COLOR_PAIR(PAIR_OPTIONS) ^ A_ITALIC);
}

/**
 * @brief Show a checkerboard pattern
 * 
 * @param chrono 
 */
void show_flash(Chronometer chrono) {

    if (chrono.alerte_flag == false)
        return;

    int mode = PAIR_FLASH1;
    int inner_mode = 1;

    for (int r = 0; r < FLASH_REPEAT; ++r) {
        mode = r % 3 + PAIR_FLASH1;
        for (int i = 0; i < LINES; ++i) {
            for (int j = 0; j < COLS; ++j) {
                inner_mode = ((i + j) + mode) % 3 + PAIR_FLASH1;
                attron(COLOR_PAIR(inner_mode));
                mvprintw(i, j, "$");
                attroff(COLOR_PAIR(inner_mode));
            }
        }
        refresh();
        usleep(500 * 1000);
    }
}

/**
 * @brief Add a turn
 * 
 * @param chrono 
 */
void add_lap(Chronometer* chrono) {
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

int alert_keymap(Chronometer* chrono, int touche) {
    switch (touche) {
    case '1':
    case KEY_F(1):
        chrono->alert += 60 * 60 * 1000;
        break;
    case '2':
    case KEY_F(2):
        chrono->alert -= 60 * 60 * 1000;
        break;
    case '3':
    case KEY_F(3):
        chrono->alert += 60 * 1000;
        break;
    case '4':
    case KEY_F(4):
        chrono->alert -= 60 * 1000;
        break;
    case '5':
    case KEY_F(5):
        chrono->alert += 1 * 1000;
        break;
    case '6':
    case KEY_F(6):
        chrono->alert -= 1 * 1000;
        break;
    default:
        return 0;
    }

    chrono->alerte_flag = true;

    return 1;
}

int is_over_99h(int ms) {
    return ms > (99 * 1000 * 3600);
}

int check_chronometer(Chronometer* chrono) {
    if (is_over_99h(chrono->total_ms)) {
        chrono->total_ms = 0;
    }

    if ((chrono->alert < 0) ||
        is_over_99h(chrono->alert))
        chrono->alert = 0;

    return ERR_NONE;
}

int main(void) {
    struct timeval start = {0}, end = {0};
    bool pause = true;
    Error quit = ERR_NONE;
    int touche;

    // Initialisation de ncurses
    initscr();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(FALSE);
    start_color();
    init_colors_pairs();

    Chronometer chrono = init_chronometer();

    while (!quit) {
        touche = getch();

        switch (touche) {
        case ' ':
            if (pause) {
                gettimeofday(&start, NULL);
            }
            pause ^= 1;
            break;
        case 'r':
            pause = true;
            reset_chronometer(&chrono);
            clear();
            break;
        case 't':
            add_lap(&chrono);
            chrono.scroll = 0;
            break;
        case BUTTON5_PRESSED:
        case KEY_DOWN:
            if (chrono.scroll < 0)
                chrono.scroll += 1;
            break;
        case BUTTON4_PRESSED:
        case KEY_UP:
            if (ABS(chrono.scroll) < chrono.turn_index - NB_MAX_PRINT_TURNS + 1)
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
        check_chronometer(&chrono);

        // Si la taille de la console est trop petite, on quitte
        if (LINES < 14 || COLS < 58) {
            quit = ERR_TOO_SMALL_WINDOW;
        }
        if (chrono.total_ms > chrono.alert && chrono.alerte_flag) {
            show_flash(chrono);
            chrono.alerte_flag = false;
            clear();
        }
        usleep(REFRESH * 1000);

        if (!pause) {
            gettimeofday(&end, NULL);
            chrono.total_ms += time_interval(start, end);
            start = end;
        }
        show_tui(chrono);
    }

    clear();
    getch();
    endwin();

    if (quit == ERR_TOO_SMALL_WINDOW)
        fprintf(stderr, STR_ERR_TOO_SMALL_WINDOW);

    return 0;
}
