#include <stdio.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/semphr.h"

#include "wifi.h"
#include "mqtt.h"

xSemaphoreHandle wifiConnect_semaphore;
xSemaphoreHandle mqttConnect_semaphore;

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
  if (xSemaphoreTake(mqttConnect_semaphore, portMAX_DELAY))
  {
    mqtt_register();
  }
}

void app_main(void)
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
    
  wifiConnect_semaphore = xSemaphoreCreateBinary();
  mqttConnect_semaphore = xSemaphoreCreateBinary();

  wifi_start();
  xTaskCreate(&wifiConnect, "Handler of WiFi", 4096, NULL, 1, NULL);

  registerSystem();
}
