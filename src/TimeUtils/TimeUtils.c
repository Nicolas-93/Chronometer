#include <sys/time.h>
#include "TimeUtils.h"

time_t intervalle_ms(struct timeval debut, struct timeval fin) {
    time_t sec = fin.tv_sec - debut.tv_sec;
    time_t usec = fin.tv_usec - debut.tv_usec;
    sec *= (long int) 1e6;
    return (sec + usec) / 1000;
}
time_t nb_ms_vers_secondes(time_t nb_ms) {
    return (nb_ms / (1000 * 1)) % 60;
}
time_t nb_ms_vers_minutes(time_t nb_ms) {
    return (nb_ms / (1000 * 60)) % 60;
}
time_t nb_ms_vers_heures(time_t nb_ms) {
    return (nb_ms / (1000 * 60 * 60)) % 24;
}

time_t nb_ms_vers_centiemes(time_t nb_ms) {
    return (nb_ms / 10) % 100;
}

FormattedTime ms_to_FormattedTime(time_t ms) {
    FormattedTime interval;
    interval.cs = nb_ms_vers_centiemes(ms);
    interval.sec = nb_ms_vers_secondes(ms);
    interval.min = nb_ms_vers_minutes(ms);
    interval.hour = nb_ms_vers_heures(ms);
    return interval;
}
