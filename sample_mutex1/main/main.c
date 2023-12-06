#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" // Include the semaphore header

SemaphoreHandle_t xRecursiveMutex; // Declare a recursive mutex handle

void tarefa1(void *param);

void app_main(void)
{
    // Create a recursive mutex
    xRecursiveMutex = xSemaphoreCreateRecursiveMutex();

    if (xRecursiveMutex != NULL)
    {
        // Create the task
        xTaskCreate(&tarefa1, "Tarefa1", 2048, NULL, 1, NULL);

        // Create the interrupt (simulated in this example)
        const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);
        for (;;)
        {
            vTaskDelay(xDelay500ms);
            printf("Interrupt simulation - About to give the recursive mutex.\n");

            // Give the recursive mutex from the interrupt
            xSemaphoreGiveRecursive(xRecursiveMutex);

            printf("Interrupt simulation - Recursive mutex given.\n\n\n");
        }
    }
    else
    {
        printf("Failed to create recursive mutex.\n");
    }
}

void tarefa1(void *param)
{
    (void)param;

    while (1)
    {
        printf("tarefa1 starts - Waiting for recursive mutex...\n");

        // Take the recursive mutex
        if (xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY) == pdPASS)
        {
            // Task is unblocked when the recursive mutex is taken
            printf("tarefa1: Got the recursive mutex! Processing...\n");

            // Simulate some processing
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            printf("tarefa1: Processing complete. Releasing recursive mutex.\n");

            // Release the recursive mutex to allow the next synchronization
            xSemaphoreGiveRecursive(xRecursiveMutex);
        }
        else
        {
            printf("Failed to take recursive mutex.\n");
        }
    }
}