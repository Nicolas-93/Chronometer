#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>
#include <unistd.h>
#include "TimeUtils/TimeUtils.h"

#define REFRESH 500

void print_time(Interval d) {
    printf("%d : %d : %d : %d\n", d.hour, d.min, d.sec, d.cs);
}

int main(int argc, char* argv[]) {
    Interval duration;
    struct timeval debut, fin;
    gettimeofday(&debut, NULL);

    while (true) {
        gettimeofday(&fin, NULL);
        duration = ms_to_Interval(intervalle_ms(debut, fin));
        print_time(duration);
        usleep(REFRESH * 1000);
    }
    
    
    return 0;
}

