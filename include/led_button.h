#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <Arduino.h>
#include "m_typedef.h"

#define NUMBER_CHANNEL   3
#define CHANNEL1         0
#define CHANNEL2         1
#define CHANNEL3         2

#define NUMBER_BUTTON   NUMBER_CHANNEL

#define BUTTON1_PIN     2
#define BUTTON2_PIN     3
#define BUTTON3_PIN     4

#define BUTTON1         0
#define BUTTON2         1
#define BUTTON3         2

#define NUMBER_LED      NUMBER_CHANNEL

#define LED1_PIN        5
#define LED2_PIN        6
#define LED3_PIN        7

#define LED_STATUS      13

#define TIME_SLICE_TO_READ_BUTTON   10

#define OS_BTN_IS_PRESS_TIME_MAX    5000
#define OS_BTN_IS_PRESS_TIME_MIN    20

void led_button_init(void);
void button_handler(void);

void gpio_on(uint8_t pin);
void gpio_off(uint8_t pin);
void gpio_toggle(uint8_t pin);
void control_device(uint8_t cmd);

hc06_ctrl_t uno_get_cmd_for_pin(uint8_t index, uint8_t is_turn_on);

#endif