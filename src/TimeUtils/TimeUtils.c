#include <sys/time.h>
#include <assert.h>
#include "TimeUtils.h"

/**
 * @brief Retourne l'intervalle de temps en millisecondes
 * entre deux temps de type timeval.
 * Si la fin est antérieure au début, une assertion échoue.
 * 
 * @param debut 
 * @param fin 
 * @return time_t 
 */

time_t intervalle_ms(struct timeval debut, struct timeval fin) {
    time_t intervalle = 0;
    
    time_t sec = fin.tv_sec - debut.tv_sec;
    time_t usec = fin.tv_usec - debut.tv_usec;
    sec *= (long int) 1e6;

    intervalle =  (sec + usec) / 1000;

    assert(intervalle >= 0);

    return intervalle;
}

int nb_ms_vers_secondes(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / (1000 * 1)) % 60;
}
int nb_ms_vers_minutes(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / (1000 * 60)) % 60;
}
int nb_ms_vers_heures(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / (1000 * 60 * 60));
}

int nb_ms_vers_centiemes(time_t nb_ms) {
    assert(nb_ms >= 0);

    return (nb_ms / 10) % 100;
}

/**
 * @brief Sépare un temps en millisecondes en plusieurs composantes.
 * 
 * @param ms 
 * @return FormattedTime 
 */
FormattedTime ms_to_FormattedTime(time_t ms) {
    FormattedTime interval;

    interval.cs = nb_ms_vers_centiemes(ms);
    interval.sec = nb_ms_vers_secondes(ms);
    interval.min = nb_ms_vers_minutes(ms);
    interval.hour = nb_ms_vers_heures(ms);

    return interval;
}
