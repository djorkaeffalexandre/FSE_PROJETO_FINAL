#include "gpio.h"

#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_register();
void mqtt_publish_state(State state);
void mqtt_publish_dht11();

#endif
