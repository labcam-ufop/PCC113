#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_timer.h"

#define BUTTON_GPIO GPIO_NUM_0 // Defina o pino do botão
#define DEBOUNCE_TIME_MS 50    // Tempo de debounce em milissegundos
#define MAX_COUNT 10           // Número máximo de apertos para imprimir a mensagem

SemaphoreHandle_t xCountingSemaphore; // Declare um semáforo contador
esp_timer_handle_t debounce_timer;    // Handle para o temporizador de debounce

void button_isr_handler(void *arg);
void debounce_timer_callback(void *arg);
void task_button_press(void *param);

void app_main(void)
{
    // Crie um semáforo contador com valor máximo de 10 e valor inicial de 0
    xCountingSemaphore = xSemaphoreCreateCounting(MAX_COUNT, 0);
    if (xCountingSemaphore == NULL)
    {
        printf("Failed to create counting semaphore.\n");
        return;
    }

    // Configure o pino do botão como entrada com interrupção na borda de subida
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_POSEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << BUTTON_GPIO),
        .pull_down_en = 0,
        .pull_up_en = 1,
    };
    gpio_config(&io_conf);

    // Instale o serviço de interrupção do GPIO
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

    // Configure o temporizador de debounce
    const esp_timer_create_args_t debounce_timer_args = {
        .callback = &debounce_timer_callback,
        .name = "debounce_timer"
    };
    esp_timer_create(&debounce_timer_args, &debounce_timer);

    // Crie a tarefa para processar os apertos do botão
    xTaskCreate(task_button_press, "TaskButtonPress", 2048, NULL, 1, NULL);
}

void button_isr_handler(void *arg)
{
    // Inicie o temporizador de debounce
    esp_timer_start_once(debounce_timer, DEBOUNCE_TIME_MS * 1000);
}

void debounce_timer_callback(void *arg)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // Libere o semáforo contador a partir da ISR
    xSemaphoreGiveFromISR(xCountingSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void task_button_press(void *param)
{
    (void)param;
    while (1)
    {
        // Aguarde até que o semáforo contador seja liberado
        for (int i = 0; i < MAX_COUNT; i++)
        {
            xSemaphoreTake(xCountingSemaphore, portMAX_DELAY);
        }
        printf("Button pressed 10 times!\n");
    }
}
