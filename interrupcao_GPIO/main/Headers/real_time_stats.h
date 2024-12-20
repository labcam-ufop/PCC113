#ifndef REAL_TIME_STATS_H
#define REAL_TIME_STATS_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"

// Macros
#define STATS_TASK_PRIO     3
#define STATS_TICKS         pdMS_TO_TICKS(5000)
#define ARRAY_SIZE_OFFSET   5

// Function Prototypes
esp_err_t print_real_time_stats(TickType_t xTicksToWait);
void start_stats_collection(void);

#endif // REAL_TIME_STATS_H