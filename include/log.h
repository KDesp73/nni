#ifndef LOG_H
#define LOG_H

#include <io/ansi.h>

#define logtemp(stream, tag, fmt, ...) \
    fprintf(stream, tag " " fmt "\n" ANSI_RESET, ##__VA_ARGS__)

#define info(fmt, ...) \
    logtemp(stdout, ANSI_BLUE "info", fmt, ##__VA_ARGS__)

#define warn(fmt, ...) \
    logtemp(stdout, ANSI_YELLOW "warn", fmt, ##__VA_ARGS__)

#define erro(fmt, ...) \
    logtemp(stdout, ANSI_RED "erro", fmt, ##__VA_ARGS__)

#define debu(fmt, ...) \
    do { \
        if(state.debug) \
            logtemp(stdout, ANSI_GREY "debu", fmt, ##__VA_ARGS__) \
    } while(0)

#endif // LOG_H
