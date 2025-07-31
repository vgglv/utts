#include <locale.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void printHelp() {
    printf("utts - unix timestamp to string\nusage: 'utts <timestamp>'\n");
}

typedef struct Time {
    int year;
    int month;
    int day;
    int hour;
    int minutes;
    int seconds;
    const char* zone;
} Time;

bool isLeapYear(int year) {
    // 1. If the year is evenly divisible by 4, go to step 2. Otherwise, go to step 5.
    // 2. If the year is evenly divisible by 100, go to step 3. Otherwise, go to step 4.
    // 3. If the year is evenly divisible by 400, go to step 4. Otherwise, go to step 5.
    // 4. The year is a leap year (it has 366 days).
    // 5. The year is not a leap year (it has 365 days). 
    if (year % 4 != 0) {
        return false;
    } 
    if (year % 100 != 0) {
        return true;
    } 
    if (year % 400 != 0) {
        return false;
    }
    return true;
}

void printTime(const Time* t) {
    printf("Zone%s: %02d:%02d:%02d %02d/%02d/%d\n", t->zone,
           t->hour, t->minutes, t->seconds,
           t->day, t->month, t->year);
}

Time convert(time_t timestamp) {
    struct tm* gm = localtime(&timestamp);

    Time t = {
        .year = gm->tm_year + 1900,
        .month = gm->tm_mon + 1,
        .day = gm->tm_mday,
        .hour = gm->tm_hour,
        .minutes = gm->tm_min,
        .seconds = gm->tm_sec,
        .zone = gm->tm_zone
    };
    return t;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }
    time_t timestamp = strtoll(argv[1], NULL, 10);
    if (timestamp <= 0) {
        printHelp();
        return 0;
    }
    Time utc_t = convert(timestamp);
    printTime(&utc_t);
}
