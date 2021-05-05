#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <rtinfo.h>

pid_t notify(char *title, char *message, char *level) {
    pid_t pid;
    char *bin = "/usr/bin/dunstify";
    char *argv[6] = {bin, "-u", level, title, message, NULL};

    if((pid = fork()) != 0) {
        if(pid < 0) {
            perror("fork");
            return 0;
        }

        // wait for child
        waitpid(pid, NULL, 0);
        return pid;
    }

    execv(bin, argv);
    perror("execv");

    exit(EXIT_FAILURE);
    return 0;
}

int battery_check(int status) {
    rtinfo_battery_t battery;
    char message[64];

    if(!rtinfo_get_battery(&battery, NULL))
        return 0;

    printf("%d\n", battery.load);

    if(battery.status != DISCHARGING)
        return 0;

    sprintf(message, "Battery load: %d%%", battery.load);

    if(battery.load < 10) {
        printf("[+] critical low level detected\n");
        notify("Battery", message, "critical");
        return 3;
    }

    if(battery.load < 20) {
        printf("[+] low level detected\n");
        notify("Battery", message, "normal");
        return 2;
    }

    if(battery.load < 50) {
        printf("[+] less than half detected\n");
        if(status != 1)
            notify("Battery", message, "low");

        return 1;
    }

    return 0;
}

int main(void) {
    int status = 0;
    struct timespec timer = {
        .tv_sec = 20,
        .tv_nsec = 0,
    };

    while(1) {
        status = battery_check(status);
        nanosleep(&timer, NULL);
    }
}
