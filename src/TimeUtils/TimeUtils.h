#include <sys/time.h>

typedef struct {
    int cs;
    int sec;
    int min;
    int hour;
} Interval;
typedef Interval Chrono;

int intervalle_ms(struct timeval debut, struct timeval fin);
int nb_ms_vers_centiemes(int nb_ms);
int nb_ms_vers_secondes(int nb_ms);
int nb_ms_vers_minutes(int nb_ms);
int nb_ms_vers_heures(int nb_ms);
Interval ms_to_Interval(int ms) ;

