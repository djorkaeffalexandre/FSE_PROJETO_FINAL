#include <stdio.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED 2

void app_main(void)
{
  gpio_pad_select_gpio(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
  
  int state = 0;

  while (1)
  {
    gpio_set_level(LED, state);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    state = !state;
  }
}
