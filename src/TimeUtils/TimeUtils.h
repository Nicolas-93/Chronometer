#include <sys/time.h>

/**
 * @brief Structure séparant les composantes d'un temps.
 * 
 */
typedef struct {
    int cs;
    int sec;
    int min;
    int hour;
} FormattedTime;

time_t intervalle_ms(struct timeval debut, struct timeval fin);
int nb_ms_vers_centiemes(time_t nb_ms);
int nb_ms_vers_secondes(time_t nb_ms);
int nb_ms_vers_minutes(time_t nb_ms);
int nb_ms_vers_heures(time_t nb_ms);
FormattedTime ms_to_FormattedTime(time_t ms) ;
