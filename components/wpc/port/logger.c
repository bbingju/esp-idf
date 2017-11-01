/* Copyright 2017 Flyingloft Inc. All Rights Reserved.
 */
#include <stdint.h>
#include <stdio.h>

#include <time.h>
#include "platform_logger.h"


static inline void get_timestamp(char *timestamp)
{
    time_t ltime;
    struct tm result;

    ltime = time(NULL);
    localtime_r(&ltime, &result);
    sprintf(timestamp, "%d:%d:%d", result.tm_hour, result.tm_min, result.tm_sec);
}

/**
 * \Print a buffer
 * \param   buffer
 *          the buffer to print
 * \param   size
 *          the size of the buffer to print
 */
inline void Logger_print_buffer(uint8_t * buffer, int size)
{
    int i;
    for (i = 0; i < size; i++) {
        if ((i & 0xF) == 0xF)
            printf("\n");
        printf("%x ", buffer[i]);
    }
    printf("\n");
}

static inline void print_prefix(char level, char * module)
{
    char timestamp[26] = {0,};
    get_timestamp(timestamp);
    printf("[%s][%s] %c:" , module, timestamp, level);
}

void Platform_LOG(char level, char * module, char * format, va_list args)
{
    print_prefix(level, module);
    vprintf(format, args);
}
