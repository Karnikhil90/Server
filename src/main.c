#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Define the GPIO pin connected to the LED
#define BLINK_GPIO GPIO_NUM_2

void app_main(void) {
    // Configure the GPIO pin as output
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
        // Turn the LED on
        gpio_set_level(BLINK_GPIO, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500 ms

        // Turn the LED off
        gpio_set_level(BLINK_GPIO, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);  // Delay for 500 ms
    }
}
