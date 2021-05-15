#include "gpio.h"

#ifndef MQTT_H
#define MQTT_H

void mqtt_start();
void mqtt_register();
void mqtt_publish(State state);
void mqtt_publish_dht11();

#endif
