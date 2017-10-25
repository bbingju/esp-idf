/* Copyright 2017 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */
#ifndef WPC_SERIAL_H
#define WPC_SERIAL_H

/**
 * \file    platform_serial.h
 *          Low level serial interface. Used for transmitting and
 *          receiving data via UART.
 */

/**
 * \brief   Open a serial link to the Pino MCU
 * \param   port_name
 *          Name of the port as enumerated on the platform
 * \return  0 if success, -1 otherwise
 */
int Serial_open(char* port_name);

/**
 * \brief   Close a serial link previously opened with Serial_open
 * \return  0 if success, -1 otherwise
 */
int Serial_close(void);

/**
 * \brief   Read data from the serial link
 * \param   buffer
 *          The buffer to store the read chars
 * \param   buffer_size
 *          The size of the buffer
 * \return  The number of read char or a negative value in case of error
 */
int Serial_read(unsigned char * buffer, unsigned int buffer_size);

/**
 * \brief   Write data from the serial link
 * \param   buffer
 *          The buffer for the chars to write
 * \param   buffer_size
 *          The size of the buffer to write
 * \return  The number of written char or a negative value in case of error
 */ 
int Serial_write(unsigned char * buffer, unsigned int buffer_size);

#endif /* WPC_SERIAL_H */
