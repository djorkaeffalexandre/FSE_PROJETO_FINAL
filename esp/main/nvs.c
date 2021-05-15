#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#define NAME_LEN 30

#define PARTITION "NVS_FSE_UNB_2020_2"

void nvs_start()
{
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

void nvs_erase()
{
  nvs_save_string("name", "");
}

char* nvs_read_string(char *key)
{
  ESP_ERROR_CHECK(nvs_flash_init());

  size_t size;
  char *name = malloc(NAME_LEN);
  nvs_handle default_partition;
  
  esp_err_t res_nvs = nvs_open(PARTITION, NVS_READONLY, &default_partition);

  if(res_nvs == ESP_ERR_NVS_NOT_FOUND)
  {
    ESP_LOGE("NVS", "Namespace: default, não encontrado");
    return NULL;
  }
  else
  {
    nvs_get_str(default_partition, key, NULL, &size);
    esp_err_t res = nvs_get_str(default_partition, key, name, &size);

    switch (res)
    {
      case ESP_OK:
        ESP_LOGI("NVS", "Valor encontrado");
        break;
      case ESP_ERR_NOT_FOUND:
        ESP_LOGI("NVS", "Valor não encontrado");
        return NULL;
      default:
        ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
        return NULL;
    }

    nvs_close(default_partition);
  }
  return name;
}

void nvs_save_string(char *key, char *value)
{
  ESP_ERROR_CHECK(nvs_flash_init());

  nvs_handle default_partition;

  esp_err_t res_nvs = nvs_open(PARTITION, NVS_READWRITE, &default_partition);

  if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
  {
    ESP_LOGE("NVS", "Namespace: default, não encontrado");
  }

  esp_err_t res = nvs_set_str(default_partition, key, value);
  if (res != ESP_OK)
  {
    ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
  }

  nvs_commit(default_partition);
  nvs_close(default_partition);
}
