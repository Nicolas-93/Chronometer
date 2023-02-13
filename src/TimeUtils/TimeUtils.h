#include <sys/time.h>

typedef struct {
    time_t cs;
    time_t sec;
    time_t min;
    time_t hour;
} FormattedTime;

time_t intervalle_ms(struct timeval debut, struct timeval fin);
time_t nb_ms_vers_centiemes(time_t nb_ms);
time_t nb_ms_vers_secondes(time_t nb_ms);
time_t nb_ms_vers_minutes(time_t nb_ms);
time_t nb_ms_vers_heures(time_t nb_ms);
FormattedTime ms_to_FormattedTime(time_t ms) ;

