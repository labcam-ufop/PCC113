#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig_arch.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "Headers/real_time_stats.h"


//#include "randomForestModel.h"


#define INTERRUPT_PIN GPIO_NUM_4
#define STATS_BUFFER_SIZE 512




SemaphoreHandle_t xBinarySemaphore = NULL;

void IRAM_ATTR gpio_isr_handler(void* arg) {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    gpio_intr_disable(INTERRUPT_PIN); // Desativa a interrupção
    if (xBinarySemaphore != NULL) {
        xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
        if (xHigherPriorityTaskWoken) {
            portYIELD_FROM_ISR();
        }
    } else {
        printf("Failed to give semaphore from ISR.\n");
    }
}

void task1(void* arg) {	    
    while (1) {
        printf("Task1 starts - Waiting for binary semaphore...\n");
        if (xSemaphoreTake(xBinarySemaphore, portMAX_DELAY) == pdPASS) {
            printf("Task1: Got the binary semaphore! Processing...\n");
            // Simulate some processing
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            printf("Task1: Processing complete. \n");            
            
            //habilita interrupção
            gpio_intr_enable(INTERRUPT_PIN);
            
        } else {
            printf("Failed to take binary semaphore.\n");
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


void app_main() {	
	// Inicializa a coleta de estatísticas
    start_stats_collection();	
    // Cria o semáforo binário
    xBinarySemaphore = xSemaphoreCreateBinary();
    if (xBinarySemaphore != NULL) {
        // Inicialmente libera o semáforo para simular o recurso disponível
        //xSemaphoreGive(xBinarySemaphore);

        // Configura o pino de interrupção
        gpio_config_t io_conf = {
            .intr_type = GPIO_INTR_POSEDGE,
            .mode = GPIO_MODE_INPUT,
            .pin_bit_mask = (1ULL << INTERRUPT_PIN),
            .pull_down_en = 0,
            .pull_up_en = 1,
        };
        gpio_config(&io_conf);

        // Instala o serviço de interrupção de GPIO
        gpio_install_isr_service(0);
        gpio_isr_handler_add(INTERRUPT_PIN, gpio_isr_handler, NULL);

        // Cria as tarefas
        xTaskCreate(task1, "Task1", 4096, NULL, 1, NULL);
        
    } else {
        printf("Failed to create binary semaphore.\n");
    }
}