#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h" // Include the semaphore header

SemaphoreHandle_t xBinarySemaphore; // Declare a binary semaphore handle

void tarefa1(void *param);

void app_main(void)
{
    // Create a binary semaphore
    xBinarySemaphore = xSemaphoreCreateBinary();

    if (xBinarySemaphore != NULL)
    {
        // Create the task
        xTaskCreate(&tarefa1, "Tarefa1", 2048, NULL, 1, NULL);

        // Create the interrupt (simulated in this example)
        // For a real interrupt, you would use interrupt service routine (ISR)
        // and xSemaphoreGiveFromISR() to give the semaphore from ISR.
        // For simplicity, this example uses vPortGenerateSimulatedInterrupt().
        const TickType_t xDelay500ms = pdMS_TO_TICKS(500UL);
        for (;;)
        {
            vTaskDelay(xDelay500ms);
            printf("Interrupt simulation - About to give the semaphore.\n");
            xSemaphoreGive(xBinarySemaphore); // Give the semaphore from the interrupt
            printf("Interrupt simulation - Semaphore given.\n\n\n");
        }
    }
    else
    {
        printf("Failed to create binary semaphore.\n");
    }
}

void tarefa1(void *param)
{
    (void)param;

    while (1)
    {
        printf("tarefa1 starts - Waiting for semaphore...\n");

        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY))
        {
            // Task is unblocked when the semaphore is taken
            printf("tarefa1: Got the semaphore! Processing...\n");

            // Simulate some processing
            vTaskDelay(1000 / portTICK_PERIOD_MS);

            printf("tarefa1: Processing complete. Releasing semaphore.\n");

            // Release the semaphore to allow the next synchronization
            xSemaphoreGive(xBinarySemaphore);
        }
        else
        {
            printf("Failed to take semaphore.\n");
        }
    }
}
