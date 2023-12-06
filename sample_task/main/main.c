#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"

// protótipos das funções
void tarefa1(void *param);

// Manipuladores de tarefas
TaskHandle_t th1;

void app_main(void)
{
    //printf("FreeRTOS Starts\n");

    xTaskCreate(&tarefa1, "Tarefa1", 2048, NULL, 1, &th1);
}
void tarefa1(void *param)
{
	// variáveis

	// parâmetros
	(void)param;

	// laço da tarefa
	while(1)
	{
		printf("tarefa1 starts\n");
		vTaskDelay(1000 / portTICK_PERIOD_MS);

	}
}
