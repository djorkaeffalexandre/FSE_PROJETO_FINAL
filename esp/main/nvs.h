#include "gpio.h"

#ifndef NVS_H
#define NVS_H

void nvs_start();
void nvs_erase();
char* nvs_read_string(char *key);
void nvs_save_string(char *key, char *value);

#endif
