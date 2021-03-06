#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "cJSON.h"
#include "esp_log.h"
#include "mqtt_client.h"

#include "mqtt.h"
#include "gpio.h"
#include "dht11.h"
#include "nvs.h"

#define TAG "MQTT"

#define NAME_LEN 30

#define REGISTER "REGISTER"
#define UNREGISTER "UNREGISTER"
#define SET_OUTPUT "SET_OUTPUT"

#define PUSH_TICK 2000

#ifdef CONFIG_ESP_MODEL_TYPE_BATTERY
#define TYPE "BATTERY"
#else
#define TYPE "ENERGY"
#endif

#define STUDENT_ID CONFIG_ESP_STUDENT_ID

extern xSemaphoreHandle mqttConnect_semaphore;
extern xSemaphoreHandle registerHandler_semaphore;

esp_mqtt_client_handle_t client;

char _name[NAME_LEN];

char* mac_address()
{
    u_int8_t base_mac_address[6] = {0};
    int size = 20;
    char *mac = malloc(size);
    esp_efuse_mac_get_default(base_mac_address);
    snprintf(
        mac,
        size,
        "%x%x%x%x%x%x",
        base_mac_address[0],
        base_mac_address[1],
        base_mac_address[2],
        base_mac_address[3],
        base_mac_address[4],
        base_mac_address[5]
    );
    return mac;
}

char *get_topic_for(char *option)
{
    int topic_size = 64;
    char *topic = malloc(topic_size);
    snprintf(topic, topic_size, "fse2020/%d/%s/%s", STUDENT_ID, _name, option);
    return topic;
}

void mqtt_handle_data(int length, char *data)
{
    /*
    {
        "type": "REGISTER" | "SET_OUTPUT" | "UNREGISTER",
        "name": "name"
    }
    */
    cJSON *body = cJSON_Parse(data);

    char *type = cJSON_GetObjectItem(body, "type")->valuestring;

    if (strcmp(type, REGISTER) == 0) {
        strcpy(_name, cJSON_GetObjectItem(body, "name")->valuestring);
        nvs_save_string("name", _name);
        xSemaphoreGive(registerHandler_semaphore);
    }

    if (strcmp(type, UNREGISTER) == 0) {
        nvs_erase();
        strcpy(_name, "");
        mqtt_register();
    }
    
    #ifdef CONFIG_ESP_MODEL_TYPE_ENERGY
    if (strcmp(type, SET_OUTPUT) == 0) {
        State state = gpio_current_state();
        state.output = !state.output;
        gpio_toggle(state);
    }
    #endif
}

static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{   
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            xSemaphoreGive(mqttConnect_semaphore);
            break;
        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            mqtt_handle_data(event->data_len, event->data);
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            break;
        default:
            ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}

void mqtt_start()
{
    esp_mqtt_client_config_t mqtt_config = {
        .uri = "mqtt://test.mosquitto.org",
    };
    client = esp_mqtt_client_init(&mqtt_config);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_receive_message(char *topic)
{
    esp_mqtt_client_subscribe(client, topic, 0);
}

void mqtt_send_message(char *topic, char *message)
{
    int message_id = esp_mqtt_client_publish(client, topic, message, 0, 1, 0);
    ESP_LOGI(TAG, "Mensagem enviada, ID: %d", message_id);
}

void mqtt_register_with(char *name)
{
    strcpy(_name, name);
    nvs_save_string("name", _name);
    xSemaphoreGive(registerHandler_semaphore);

    char topic[64];
    snprintf(topic, 64, "fse2020/%d/dispositivos/%s", STUDENT_ID, mac_address());
    mqtt_receive_message(topic);
}

void mqtt_register()
{
    cJSON *data = cJSON_CreateObject();

    /*
    {
        "type": "ENERGY" | "BATTERY",
        "mac": mac_address
    }
    */

    cJSON_AddStringToObject(data, "type", TYPE);
    cJSON_AddStringToObject(data, "mac", mac_address());

    char *json = cJSON_Print(data);

    char topic[64];
    snprintf(topic, 64, "fse2020/%d/dispositivos/%s", STUDENT_ID, mac_address());

    mqtt_send_message(topic, json);
    mqtt_receive_message(topic);
}

void mqtt_publish_state(State state)
{
    /*
    {
        "mac": "mac_address"
        "output": 0 | 1
        "input": 0 | 1
    }
    */
    cJSON *data = cJSON_CreateObject();

    cJSON_AddStringToObject(data, "mac", mac_address());
    cJSON_AddNumberToObject(data, "output", state.output);
    cJSON_AddNumberToObject(data, "input", !state.input);

    char *json = cJSON_Print(data);

    char *topic = get_topic_for("estado");
    mqtt_send_message(topic, json);
    free(topic);
}

void mqtt_publish_dht11(void *params)
{
    char *temperature_topic = get_topic_for("temperatura");
    char *humidity_topic = get_topic_for("umidade");

    /*
    {
        "mac": "mac_address",
        "temperature": int
    }
    */
    cJSON *temp_data = cJSON_CreateObject();
    cJSON_AddStringToObject(temp_data, "mac", mac_address());
    /*
    {
        "mac": "mac_address",
        "humidity": int
    }
    */
    cJSON *hum_data = cJSON_CreateObject();
    cJSON_AddStringToObject(hum_data, "mac", mac_address());

    if (xSemaphoreTake(mqttConnect_semaphore, portMAX_DELAY))
    {
        while (1)
        {
            struct dht11_reading dht11 = DHT11_read();

            if (strlen(_name) != 0 && dht11.status == 0)
            {
                cJSON_AddNumberToObject(temp_data, "temperature", dht11.temperature);
                cJSON_AddNumberToObject(hum_data, "humidity", dht11.humidity);

                char *temp_json = cJSON_Print(temp_data);
                char *hum_json = cJSON_Print(hum_data);

                mqtt_send_message(temperature_topic, temp_json);
                mqtt_send_message(humidity_topic, hum_json);

                cJSON_DeleteItemFromObject(temp_data, "temperature");
                cJSON_DeleteItemFromObject(hum_data, "humidity");
            }

            vTaskDelay(PUSH_TICK / portTICK_PERIOD_MS);
        }
    }

    free(temperature_topic);
    free(humidity_topic);
}
