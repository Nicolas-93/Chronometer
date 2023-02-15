#include <sys/time.h>
#include <stdbool.h>
#include "TimeUtils/TimeUtils.h"
#define MAX_TURNS 7

#define MIN(a, b) ((a < b) ? (a) : (b))
#define MAX(a, b) ((a > b) ? (a) : (b))

typedef struct {
    bool active;
    time_t total_ms;
    time_t duration_alert;
    int turn_index;
    int turn_saved;
    int turns[MAX_TURNS];
} Chronometer;

Chronometer initialiser_chronometre();
