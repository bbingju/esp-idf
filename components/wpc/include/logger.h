#ifndef WPC_LOGGER_H
#define WPC_LOGGER_H

#include "esp_log.h"
#include "platform_logger.h"

#define LOGE(format, ...)      ESP_LOGE(LOG_MODULE_NAME, format, ##__VA_ARGS__)
#define LOGW(format, ...)      ESP_LOGW(LOG_MODULE_NAME, format, ##__VA_ARGS__)
#define LOGI(format, ...)      ESP_LOGI(LOG_MODULE_NAME, format, ##__VA_ARGS__)
#define LOGD(format, ...)      ESP_LOGD(LOG_MODULE_NAME, format, ##__VA_ARGS__)

#endif /* WPC_LOGGER_H */
