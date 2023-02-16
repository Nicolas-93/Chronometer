#include <sys/time.h>
#include <stdbool.h>
#include "TimeUtils/TimeUtils.h"
#define MAX_TURNS 10

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define ABS(a) (MAX((-a), (a)))

typedef struct {
    bool active;
    bool alerte_flag;
    time_t total_ms;
    time_t duration_alert;
    int turn_index;
    int turn_saved;
    int turns[MAX_TURNS];
    int scroll;
} Chronometer;

Chronometer initialiser_chronometre();
