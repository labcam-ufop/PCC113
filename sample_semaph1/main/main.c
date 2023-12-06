#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

#define SEMAPHORE_SYNC (1)

// Variável compartilhada
volatile int sharedVariable = 0;

// Handler do semáforo
SemaphoreHandle_t xSemaphore;

// Task 1
void task1(void *pvParameters) {
while(1) {
if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
sharedVariable++; // Acessando a variável compartilhada
printf("Task 1 incrementa = %d\n", sharedVariable);
xSemaphoreGive(xSemaphore); // Liberando o semáforo
}
vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay de 1 segundo
}
}

// Task 2
void task2(void *pvParameters) {
while(1) {
if(xSemaphoreTake(xSemaphore, portMAX_DELAY) == pdTRUE) {
sharedVariable--; // Acessando a variável compartilhada
printf("Task 2 decrementa = %d\n", sharedVariable);
xSemaphoreGive(xSemaphore); // Liberando o semáforo
}
vTaskDelay(1500 / portTICK_PERIOD_MS); // Delay de 1.5 segundos
}
}

void app_main() {
// Criação do semáforo binário
xSemaphore = xSemaphoreCreateBinary();
if(xSemaphore != NULL) {
// Inicialização das tasks
xTaskCreate(&task1, "Task 1", 2048, NULL, 5, NULL);
xTaskCreate(&task2, "Task 2", 2048, NULL, 5, NULL);
}

// Inicia o semáforo (liberando para a Task 1)
xSemaphoreGive(xSemaphore);
}
