/* Copyright 2017 Wirepas Ltd. All Rights Reserved.
 *
 * See file LICENSE.txt for full license details.
 *
 */
#ifndef WPC_PLATFORM_H_
#define WPC_PLATFORM_H_

bool Platform_init(void);

void Platform_usleep(unsigned int time_us);

void Platform_msleep(unsigned int time_ms);

/**
 * \brief  Call at the beginning of a locked section to send a request
 * \Note   It is up to the platform implementation to see if
 *         this lock must be implemented or not. If all the requests
 *         to the stack are done in the same context, it is useless.
 *         But if poll requests (and indication handling) are not done
 *         from same thread as other API requests, it has to be implemented
 *         accordingly to the architecture chosen.
 */
bool Platform_lock_request(void);

/**
 *
 * \brief  Called at the end of a locked section to send a request
 */
void Platform_unlock_request(void);

void Platform_close(void);

void Platform_start_polling();

void Platform_stop_polling();

void Platform_change_polling_interval(unsigned int interval);

#endif /* WPC_PLATFORM_H_ */
