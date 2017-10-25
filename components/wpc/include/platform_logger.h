/* Copyright 2017 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */
#ifndef WPC_PLATFORM_LOGGER_H
#define WPC_PLATFORM_LOGGER_H

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>


void Platform_LOG(char level, char * module, char * format, va_list args);

/**
 * \Print a buffer
 * \param   buffer
 *          the buffer to print
 * \param   size
 *          the size of the buffer to print
 */
void Logger_print_buffer(uint8_t * buffer, int size);


#endif /* WPC_PLATFORM_LOGGER_H */



