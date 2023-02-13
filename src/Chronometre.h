#include <sys/time.h>
#include "TimeUtils/TimeUtils.h"
#include <stdbool.h>
#define MAX_SIZE 20

typedef struct {
    bool active;
    int total_ms;
    int duration_alert;
    int turn_index;
    int turn_saved;
    int turns[MAX_SIZE];
} Chronometer;

Chronometer initialiser_chronometre();