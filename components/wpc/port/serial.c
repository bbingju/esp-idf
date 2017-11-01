/* Copyright 2017 Flyingloft Inc. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */
#include <string.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"

#define LOG_MODULE_NAME "wpc_serial"
#define MAX_LOG_LEVEL DEBUG_LOG_LEVEL
#include "logger.h"


#define WPC_UART_NUM           (UART_NUM_2) /* CONFIG_WPC_UART_NUM */

#define WPC_SERIAL_TXD         (GPIO_NUM_17)
#define WPC_SERIAL_RXD         (GPIO_NUM_16)
#define WPC_SERIAL_RTS         (UART_PIN_NO_CHANGE)
#define WPC_SERIAL_CTS         (UART_PIN_NO_CHANGE)

#define BUF_SIZE               CONFIG_WIREPAS_UART_BUFSIZE
#define UART_BAUD_RATE         CONFIG_WIREPAS_UART_BAUD_RATE

static QueueHandle_t uart2_queue;

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    size_t buffered_size;
    uint8_t *dtmp = (uint8_t *) malloc(BUF_SIZE);
    while (1) {
        /* Waiting for UART event.
           If it happens then print out information what is it */
        if (xQueueReceive(uart2_queue, (void * )&event, (portTickType)portMAX_DELAY)) {
            LOGI("uart[%d] event:", WPC_UART_NUM);
            switch (event.type) {
            case UART_DATA:
                /* Event of UART receiving data
                 * We'd better handler data event fast, there would be much more data events
                 * than other types of events.
                 * If we take too much time on data event, the queue might be full.
                 * In this example, we don't process data in event, but read data outside.
                 */
                /* uart_get_buffered_data_len(WPC_UART_NUM, &buffered_size); */
                /* LOGI("data, len: %d; buffered len: %d", event.size, buffered_size); */
                LOGI("data, len: %d", event.size);
                /* uart_read_bytes(WPC_UART_NUM, dtmp, event.size, 100 / portTICK_RATE_MS); */
                break;
            case UART_FIFO_OVF:
                LOGE("hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // We can read data out out the buffer, or directly flush the Rx buffer.
                uart_flush(WPC_UART_NUM);
                break;
            case UART_BUFFER_FULL:
                LOGE("ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // We can read data out out the buffer, or directly flush the Rx buffer.
                uart_flush(WPC_UART_NUM);
                break;
            case UART_BREAK:
                LOGI("uart rx break detected");
                break;
            case UART_PARITY_ERR:
                LOGE("uart parity error");
                break;
            case UART_FRAME_ERR:
                LOGE("uart frame error");
                break;
            case UART_PATTERN_DET:
                LOGI("uart pattern detected");
                break;
            default:
                LOGE("not serviced uart event type: %d\n", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

/****************************************************************************/
/*                Public method implementation                              */
/****************************************************************************/
int Serial_open(char* port_name)
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUD_RATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    uart_param_config(WPC_UART_NUM, &uart_config);
    uart_set_pin(WPC_UART_NUM,
                 WPC_SERIAL_TXD, WPC_SERIAL_RXD,
                 WPC_SERIAL_RTS, WPC_SERIAL_CTS);
    /* uart_driver_install(WPC_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 24, &uart2_queue, 0); */

    /* xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL); */

    uart_driver_install(WPC_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 0, NULL, 0);

    LOGD("Serial opened");
    return 0;
}

int Serial_close(void)
{
    uart_driver_delete(WPC_UART_NUM);
    LOGD("Serial closed");
    return 0;
}

int Serial_read(unsigned char * buffer, unsigned int buffer_size)
{
    return uart_read_bytes(WPC_UART_NUM, buffer, buffer_size, 100 / portTICK_RATE_MS);
}

int Serial_write(unsigned char * buffer, unsigned int buffer_size)
{
    return uart_write_bytes(WPC_UART_NUM, (const char *) buffer, buffer_size);
}
