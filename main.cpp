#include <array>
#include <print>
#include <stdexcept>

void printHelp() {
    std::println(R"("utts" - unix timestamp to string,
usage: 'utts <timestamp>'
    )");
}

struct Time {
    int year = 0;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minutes = 0;
    int seconds = 0;
};

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

Time convertTimestampToTime(time_t timestamp) {
    Time tm{};

    auto getYear = [](time_t& timestamp) {
        int year = 1970;
        while(true) {
            int daysInYear = isLeapYear(year) ? 366 : 365;
            time_t secondsInYear = daysInYear * 86400;
            if (timestamp >= secondsInYear) {
                timestamp -= secondsInYear;
                std::println("{}", year);
                year++;
            } else {
                break;
            }
        }
        return year;
    };

    auto getMonth = [](time_t& timestamp, bool isLeap) {
        int month = 0;
        constexpr std::array<int, 12> daysInMonthNonLeap = {31,28,31,30,31,30,31,31,30,31,30,31};
        constexpr std::array<int, 12> daysInMonthLeap    = {31,29,31,30,31,30,31,31,30,31,30,31};

        while(true) {
            auto daysInMonth = isLeap ? daysInMonthLeap[month] : daysInMonthNonLeap[month];
            time_t secondsInMonth = daysInMonth * 86400;
            if (timestamp >= secondsInMonth) {
                timestamp -= secondsInMonth;
                month++;
            } else {
                break;
            }
        }
        month += 1;
        return month;
    };

    auto getDay = [](time_t& timestamp) {
        int days = 0;

        while(true) {
            time_t secondsInDay = 86400;
            if (timestamp >= secondsInDay) {
                timestamp -= secondsInDay;
                days++;
            } else {
                break;
            }
        }
        days += 1;
        return days;
    };

    auto getHour = [](time_t& timestamp) {
        int hour = 0;

        while(true) {
            time_t secondsInHour = 3600;
            if (timestamp >= secondsInHour) {
                timestamp -= secondsInHour;
                hour++;
            } else {
                break;
            }
        }
        return hour;
    };

    auto getMinutes = [](time_t& timestamp) {
        int minutes = 0;

        while(true) {
            time_t secondsInMinute = 60;
            if (timestamp >= secondsInMinute) {
                timestamp -= secondsInMinute;
                minutes++;
            } else {
                break;
            }
        }
        return minutes;
    };

    tm.year = getYear(timestamp);
    tm.month = getMonth(timestamp, isLeapYear(tm.year));
    tm.day = getDay(timestamp);
    tm.hour = getHour(timestamp);
    tm.minutes = getMinutes(timestamp);
    tm.seconds = static_cast<int>(timestamp);
    return tm;
}

void printTime(const Time& t, std::string_view timezone) {
    std::println("{}: {:02}:{:02}:{:02} {:02}/{:02}/{}", timezone, t.hour, t.minutes, t.seconds, t.day, t.month, t.year);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printHelp();
        return 0;
    }
    time_t timestamp;
    try {
        timestamp = static_cast<time_t>(std::stoll(argv[1]));
    } catch(std::invalid_argument e) {
        std::println("Error occured: {}.\n", e.what());
        printHelp();
        return -1;
    } catch(std::out_of_range e) {
        std::println("Error occured: {}.\n", e.what());
        printHelp();
        return -1;
    }

    if (timestamp <= 0) {
        std::println("Error: must be unsigned number\n");
        printHelp();
        return 0;
    }
    Time utc_t = convertTimestampToTime(timestamp);
    time_t timestamp_gmt9 = timestamp + (3600 * 9);
    Time gmt9_t = convertTimestampToTime(timestamp_gmt9);
    printTime(utc_t, "UTC");
    printTime(gmt9_t, "GMT+9");
}
