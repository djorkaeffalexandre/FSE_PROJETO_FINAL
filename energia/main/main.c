#include <stdio.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#include "dht11.h"

#define LED 2

void app_main(void)
{
  DHT11_init(GPIO_NUM_4);
  gpio_pad_select_gpio(LED);
  gpio_set_direction(LED, GPIO_MODE_OUTPUT);
  
  int state = 0;

  while (1)
  {
    printf("Temperature is %d \n", DHT11_read().temperature);
    printf("Humidity is %d\n", DHT11_read().humidity);
    printf("Status code is %d\n", DHT11_read().status);
    gpio_set_level(LED, state);
    vTaskDelay(3000 / portTICK_PERIOD_MS);
    state = !state;
  }
}
