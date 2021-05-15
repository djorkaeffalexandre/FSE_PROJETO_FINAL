#ifndef GPIO_H
#define GPIO_H

typedef struct {
  int input;
  int output;
} State;

void gpio_start();
State gpio_current_state();
void gpio_toggle(State state);

#endif
