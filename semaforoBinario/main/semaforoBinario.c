#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

// Declare a binary semaphore handle
SemaphoreHandle_t xBinarySemaphore;

void tarefa1(void *param);
void tarefa2(void *param);

void app_main(void) {
    // Create a binary semaphore
    xBinarySemaphore = xSemaphoreCreateBinary();

    if (xBinarySemaphore != NULL) {
        // Give the semaphore initially to simulate the resource being available
        xSemaphoreGive(xBinarySemaphore);

        // Create the tasks
        xTaskCreate(&tarefa1, "Tarefa1", 2048, NULL, 1, NULL); // Lower priority
        xTaskCreate(&tarefa2, "Tarefa2", 2048, NULL, 2, NULL); // Higher priority
    } else {
        printf("Failed to create binary semaphore.\n");
    }
}

void tarefa1(void *param) {
    (void)param;

    while (1) {
        printf("Tarefa1 starts - Waiting for binary semaphore...\n");

        // Take the binary semaphore
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdPASS) {
            // Task is unblocked when the semaphore is taken
            printf("Tarefa1: Got the binary semaphore! Processing...\n");

            // Simulate some processing
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            printf("Tarefa1: Processing complete. Releasing binary semaphore.\n");

            // Release the binary semaphore to allow the next synchronization
            xSemaphoreGive(xBinarySemaphore);
        } else {
            printf("Failed to take binary semaphore.\n");
        }

        // Simulate some delay before trying to take the semaphore again
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void tarefa2(void *param) {
    (void)param;

    while (1) {
        printf("Tarefa2 starts - Waiting for binary semaphore...\n");

        // Take the binary semaphore
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdPASS) {
            // Task is unblocked when the semaphore is taken
            printf("Tarefa2: Got the binary semaphore! Processing...\n");

            // Simulate some processing
            vTaskDelay(500 / portTICK_PERIOD_MS);

            printf("Tarefa2: Processing complete. Releasing binary semaphore.\n");

            // Release the binary semaphore to allow the next synchronization
            xSemaphoreGive(xBinarySemaphore);
        } else {
            printf("Failed to take binary semaphore.\n");
        }

        // Simulate some delay before trying to take the semaphore again
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}