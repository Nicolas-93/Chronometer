#include <sys/time.h>
#include <assert.h>
#include "TimeUtils.h"

time_t time_interval(struct timeval debut, struct timeval fin) {
    time_t intervalle = 0;
    
    time_t sec = fin.tv_sec - debut.tv_sec;
    time_t usec = fin.tv_usec - debut.tv_usec;
    sec *= (long int) 1e6;

    intervalle = (sec + usec) / 1000;

    assert(intervalle >= 0);

    return intervalle;
}

int ms_to_sec(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / (1000 * 1)) % 60;
}
int ms_to_min(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / (1000 * 60)) % 60;
}
int ms_to_hour(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / (1000 * 60 * 60));
}

int ms_to_tenths(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / 10) % 100;
}

FormattedTime ms_to_FormattedTime(time_t ms) {
    FormattedTime ft;

    ft.cs = ms_to_tenths(ms);
    ft.sec = ms_to_sec(ms);
    ft.min = ms_to_min(ms);
    ft.hour = ms_to_hour(ms);

    return ft;
}
