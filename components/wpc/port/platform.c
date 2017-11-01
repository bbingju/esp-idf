/* Copyright 2017 Flyingloft. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */
#include <stdbool.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define LOG_MODULE_NAME "freertos_plat"
#define MAX_LOG_LEVEL INFO_LOG_LEVEL
#include "logger.h"
#include "wpc_internal.h"

#define MAX_NUMBER_INDICATION 3

// Polling interval to check for indication
#define DEFAULT_POLLING_INTERVAL_MS 500

#define POLLING_THREAD_PRIO         12 /* ( tskIDLE_PRIORITY + 2UL ) */
#define POLLING_THREAD_STACKSIZE    4096

// Mutex for sending, ie serial access
static SemaphoreHandle_t sending_mutex;

// This thread is used to poll for indication
static TaskHandle_t thread_polling;

static TickType_t polling_interval = pdMS_TO_TICKS(DEFAULT_POLLING_INTERVAL_MS);
static bool polling_stop = true;

/*****************************************************************************/
/*                Thread main loop implementation                            */
/*****************************************************************************/

/**
 * \brief   Polling thread.
 *          This thread polls for indication every POLLING_INTERVAL_MS ms
 */
static void poll_for_indication(void * unused)
{
    int res;

    while (true)
    {
        vTaskDelay(polling_interval);

        if (polling_stop == false)
            res = WPC_Int_get_indication(MAX_NUMBER_INDICATION);
    }
    // Cannot be reached
    LOGI("Exiting polling thread");
}

void Platform_usleep(unsigned int time_us)
{
    usleep(time_us);
}

bool Platform_lock_request(void)
{
    while (xSemaphoreTake(sending_mutex, portMAX_DELAY) != pdPASS);
    return true;
}

void Platform_unlock_request(void)
{
    xSemaphoreGive(sending_mutex);
}

bool Platform_init(void)
{
    sending_mutex = xSemaphoreCreateMutex();
    if (!sending_mutex) {
        return false;
    }

    xTaskCreate(poll_for_indication,
                "wpc_polling",
                POLLING_THREAD_STACKSIZE,
                NULL,
                POLLING_THREAD_PRIO,
                &thread_polling);
    if (!thread_polling) {
        LOGE("xTaskCreate error");
        return false;
    }

    return true;
}

void Platform_close(void)
{
    vTaskDelete(thread_polling);
}

void Platform_start_polling()
{
    polling_stop = false;
}

void Platform_stop_polling()
{
    polling_stop = true;
}

void Platform_change_polling_interval(unsigned int interval)
{
    if (pdMS_TO_TICKS(interval) < pdMS_TO_TICKS(100))
        interval = pdMS_TO_TICKS(100);
    else if (pdMS_TO_TICKS(interval) > pdMS_TO_TICKS(1000))
        interval = pdMS_TO_TICKS(1000);
    else
        polling_interval = pdMS_TO_TICKS(interval);
}
