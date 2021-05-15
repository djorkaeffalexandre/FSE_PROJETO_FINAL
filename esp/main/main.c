#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"
#include "dht11.h"
#include "nvs.h"

xSemaphoreHandle wifiConnect_semaphore;
xSemaphoreHandle mqttConnect_semaphore;
xSemaphoreHandle registerHandler_semaphore;

void wifiConnect(void *params)
{
  while(true)
  {
    if(xSemaphoreTake(wifiConnect_semaphore, portMAX_DELAY))
    {
      mqtt_start();
    }
  }
}

void registerSystem()
{
  char *name = nvs_read_string("name");
  if (xSemaphoreTake(mqttConnect_semaphore, portMAX_DELAY))
  {
    if (name == NULL || strlen(name) == 0) {
      mqtt_register();
    } else {
      mqtt_register_with(name);
    }

    if (xSemaphoreTake(registerHandler_semaphore, portMAX_DELAY))
    {
      gpio_start();
    }

    xSemaphoreGive(mqttConnect_semaphore);
  }
  free(name);
}

void app_main(void)
{
  #ifdef CONFIG_ESP_MODEL_TYPE_ENERGY
  DHT11_init(GPIO_NUM_4);
  #endif

  nvs_start();
    
  wifiConnect_semaphore = xSemaphoreCreateBinary();
  mqttConnect_semaphore = xSemaphoreCreateBinary();
  registerHandler_semaphore = xSemaphoreCreateBinary();

  wifi_start();
  xTaskCreate(&wifiConnect, "Handler of WiFi", 4096, NULL, 1, NULL);

  registerSystem();

  #ifdef CONFIG_ESP_MODEL_TYPE_ENERGY
  xTaskCreate(&mqtt_publish_dht11, "Handler of DHT11", 2048, NULL, 1, NULL);
  #endif
}
