#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "mqtt.h"
#include "sdkconfig.h"

#include "gpio.h"
#include "mqtt.h"

#define INPUT 0
#define OUTPUT 2

State _state;

int _started = 0;

xQueueHandle interruptionQueue;

static void IRAM_ATTR gpio_isr_handler(void *args)
{
  int pin = (int) args;
  xQueueSendFromISR(interruptionQueue, &pin, NULL);
}

void handle_interruption(void *params)
{
  int pin;

  while (true)
  {
    if (xQueueReceive(interruptionQueue, &pin, portMAX_DELAY))
    {
        int state = gpio_get_level(pin);
        State current = gpio_current_state();
        current.input = !current.input;
        gpio_toggle(current);
        if (state == 1)
        {
          gpio_isr_handler_remove(pin);
          while (gpio_get_level(pin) == state)
          {
            vTaskDelay(50 / portTICK_PERIOD_MS);
          }
          State current = gpio_current_state();
          current.input = !current.input;
          gpio_toggle(current);

          vTaskDelay(50 / portTICK_PERIOD_MS);
          gpio_isr_handler_add(pin, gpio_isr_handler, (void *)pin);
        }
    }
  }
}

void gpio_start()
{
  State state;
  state.input = gpio_get_level(INPUT);
  state.output = gpio_get_level(OUTPUT);
  _state = state;

  gpio_pad_select_gpio(INPUT);
  gpio_set_direction(INPUT, GPIO_MODE_INPUT);
  gpio_pulldown_en(INPUT);
  gpio_pullup_dis(INPUT);

  gpio_set_intr_type(INPUT, GPIO_INTR_ANYEDGE);

  interruptionQueue = xQueueCreate(10, sizeof(int));
  xTaskCreate(handle_interruption, "Handle of Button", 2048, NULL, 1, NULL);

  gpio_install_isr_service(0);
  gpio_isr_handler_add(INPUT, gpio_isr_handler, (void *)INPUT);

  _started = 1;
}

State gpio_current_state()
{
  return _state;
}

void gpio_toggle(State state)
{
  if (!_started) {
    return;
  }
  
  if (_state.output != state.output) {
    gpio_pad_select_gpio(OUTPUT);
    gpio_set_direction(OUTPUT, GPIO_MODE_OUTPUT);
    gpio_set_level(OUTPUT, state.output);
  }

  _state = state;

  mqtt_publish(_state);
}