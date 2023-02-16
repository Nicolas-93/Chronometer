#define _DEFAULT_SOURCE

#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>
#include <unistd.h>
#include "TimeUtils/TimeUtils.h"

#define REFRESH 500

void print_time(FormattedTime d) {
    printf("\r%02d : %02d : %02d : %02d", d.hour, d.min, d.sec, d.cs);
    fflush(stdout); // Actualise l'affichage car stdout est bufferisé
}

int main(int argc, char* argv[]) {
    FormattedTime duration;
    struct timeval debut, fin;
    gettimeofday(&debut, NULL);

    while (true) {
        gettimeofday(&fin, NULL);
        duration = ms_to_FormattedTime(intervalle_ms(debut, fin));
        print_time(duration);
        usleep(REFRESH * 1000);
    }

    return 0;
}
