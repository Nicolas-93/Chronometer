#include <sys/time.h>
#include "TimeUtils.h"

int intervalle_ms(struct timeval debut, struct timeval fin) {
    time_t sec = fin.tv_sec - debut.tv_sec;
    long int usec = fin.tv_usec - debut.tv_usec;
    sec *= 1e6;
    return (sec + usec) / 1000;
}
int nb_ms_vers_secondes(int nb_ms) {
    return (nb_ms / 1000) % 60;
}
int nb_ms_vers_minutes(int nb_ms) {
    return (nb_ms / 60000) % 60;
}
int nb_ms_vers_heures(int nb_ms) {
    return (nb_ms / 3600000) % 24;
}

int nb_ms_vers_centiemes(int nb_ms) {
    return (nb_ms / 10) % 100;
}

Interval ms_to_Interval(int ms) {
    Interval interval;
    interval.cs = nb_ms_vers_centiemes(ms);
    interval.sec = nb_ms_vers_secondes(ms);
    interval.min = nb_ms_vers_minutes(ms);
    interval.hour = nb_ms_vers_heures(ms);
    return interval;
}
