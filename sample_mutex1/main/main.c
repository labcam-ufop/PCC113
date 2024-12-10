#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Declare a mutex handle
SemaphoreHandle_t xMutex;

void tarefa1(void *param);
void tarefa2(void *param);

void app_main(void) {
    // Create a mutex
    xMutex = xSemaphoreCreateMutex();

    if (xMutex != NULL) {
        // Create the tasks
        xTaskCreate(&tarefa1, "Tarefa1", 2048, NULL, 1, NULL); // Lower priority
        xTaskCreate(&tarefa2, "Tarefa2", 2048, NULL, 2, NULL); // Higher priority
    } else {
        printf("Failed to create mutex.\n");
    }
}

void tarefa1(void *param) {
    (void)param;

    while (1) {
        printf("Tarefa1 starts - Waiting for mutex...\n");

        // Take the mutex
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdPASS) {
            // Task is unblocked when the mutex is taken
            printf("Tarefa1: Got the mutex! Processing...\n");

            // Simulate some processing
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            printf("Tarefa1: Processing complete. Releasing mutex.\n");

            // Release the mutex to allow the next synchronization
            xSemaphoreGive(xMutex);
        } else {
            printf("Failed to take mutex.\n");
        }

        // Simulate some delay before trying to take the mutex again
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void tarefa2(void *param) {
    (void)param;

    while (1) {
        printf("Tarefa2 starts - Waiting for mutex...\n");

        // Take the mutex
        if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdPASS) {
            // Task is unblocked when the mutex is taken
            printf("Tarefa2: Got the mutex! Processing...\n");

            // Simulate some processing
            vTaskDelay(500 / portTICK_PERIOD_MS);

            printf("Tarefa2: Processing complete. Releasing mutex.\n");

            // Release the mutex to allow the next synchronization
            xSemaphoreGive(xMutex);
        } else {
            printf("Failed to take mutex.\n");
        }

        // Simulate some delay before trying to take the mutex again
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}
