#include <sys/time.h>

typedef struct {
    int cs;
    int sec;
    int min;
    int hour;
} FormattedTime;

time_t time_interval(struct timeval debut, struct timeval fin);
int ms_to_tenths(time_t nb_ms);
int ms_to_sec(time_t nb_ms);
int ms_to_min(time_t nb_ms);
int ms_to_hour(time_t nb_ms);
FormattedTime ms_to_FormattedTime(time_t ms) ;
