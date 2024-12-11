#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Declare task handles
TaskHandle_t xTask1Handle = NULL;
TaskHandle_t xTask2Handle = NULL;

void tarefa1(void *param);
void tarefa2(void *param);

void app_main(void) {
    // Create the tasks
    xTaskCreate(&tarefa1, "Tarefa1", 2048, NULL, 1, &xTask1Handle); // Lower priority
    xTaskCreate(&tarefa2, "Tarefa2", 2048, NULL, 2, &xTask2Handle); // Higher priority
}

void tarefa1(void *param) {
    (void)param;

    while (1) {
        printf("Tarefa1 starts - Waiting for task notification...\n");

        // Wait for a notification from tarefa2
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Task is unblocked when the notification is received
        printf("Tarefa1: Got the task notification! Processing...\n");

        // Simulate some processing
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        printf("Tarefa1: Processing complete. Sending notification to Tarefa2.\n");

        // Notify tarefa2 to continue
        xTaskNotifyGive(xTask2Handle);

        // Simulate some delay before trying to take the notification again
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void tarefa2(void *param) {
    (void)param;

    while (1) {
        printf("Tarefa2 starts - Sending notification to Tarefa1...\n");

        // Notify tarefa1 to continue
        xTaskNotifyGive(xTask1Handle);

        // Wait for a notification from tarefa1
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Task is unblocked when the notification is received
        printf("Tarefa2: Got the task notification! Processing...\n");

        // Simulate some processing
        vTaskDelay(500 / portTICK_PERIOD_MS);

        printf("Tarefa2: Processing complete. Waiting for notification from Tarefa1.\n");

        // Simulate some delay before trying to take the notification again
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}