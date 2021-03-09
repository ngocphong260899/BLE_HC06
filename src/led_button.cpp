#include "led_button.h"
#include "m_typedef.h"
#include "rtc.h"
#include "uno_database.h"
#include "m_typedef.h"

extern device_info_t m_device;
extern uint8_t uno_sync_database_request;

static uint16_t time_button_press[NUMBER_BUTTON] = {0, 0, 0};
static uint8_t m_buttons[NUMBER_BUTTON] = {BUTTON1_PIN, BUTTON2_PIN, BUTTON3_PIN};
static uint8_t m_leds[NUMBER_LED] = {LED1_PIN, LED2_PIN, LED3_PIN};


void gpio_on(uint8_t pin)
{
    digitalWrite(pin, HIGH);
}

void gpio_off(uint8_t pin)
{
    digitalWrite(pin, LOW);
}

void gpio_toggle(uint8_t pin)
{
    /* toggle value pin */
    digitalWrite(pin, !digitalRead(pin));

    /* Update state and report */
    report_current_state(0);

    uno_sync_database_request = 1;
}

static void blink_led_status()
{
    uint8_t time_delay = 100;
    uint8_t pin = LED_STATUS;

    digitalWrite(LED_STATUS, HIGH);
    delay(time_delay);
    digitalWrite(pin, LOW);
    delay(time_delay);

    digitalWrite(pin, HIGH);
    delay(time_delay);
    digitalWrite(pin, LOW);
    delay(time_delay);

    digitalWrite(pin, HIGH);
    delay(time_delay);
    digitalWrite(pin, LOW);
    delay(time_delay);

    digitalWrite(pin, HIGH);
    delay(time_delay);
    digitalWrite(pin, LOW);
    delay(time_delay);
}

void control_device(uint8_t cmd)
{
    switch (cmd)
    {
    case CH1_ON: {
        gpio_on(LED1_PIN);
    } break;
    case CH1_OFF: {
        gpio_off(LED1_PIN);
    } break;

    case CH2_ON: {
        gpio_on(LED2_PIN);
    } break;
    case CH2_OFF: {
        gpio_off(LED2_PIN);
    } break;

    case CH3_ON: {
        gpio_on(LED3_PIN);
    } break;
    case CH3_OFF: {
        gpio_off(LED3_PIN);
    } break;
    
    default: {
        Serial.print("Control invalid pin\n");
    } break;
    }

    /* Update and Report information of this device */
    report_current_state(0);

    uno_sync_database_request = 1;
}

hc06_ctrl_t uno_get_cmd_for_pin(uint8_t index, uint8_t is_turn_on)
{
    if (index == 0 && is_turn_on) {
        return CH1_ON;
    } else if (index == 0 && !is_turn_on) {
        return CH1_OFF;
    } else if (index == 1 && is_turn_on) {
        return CH2_ON;
    } else if (index == 1 && !is_turn_on) {
        return CH2_OFF;
    } else if (index == 2 && is_turn_on) {
        return CH3_ON;
    } else if (index == 2 && !is_turn_on) {
        return CH3_OFF;
    }

    /* error */
    return CH3_OFF;
}



extern uint8_t alarm_is_set;
extern m_alarm_t m_time_alarm[MAX_CMD_ALARM];
extern uint8_t uno_sync_database_request;

static void scan_button_handler(uint8_t button_index)
{
   if (!digitalRead(m_buttons[button_index])) /* press */
   {
        time_button_press[button_index] += TIME_SLICE_TO_READ_BUTTON;

        if (time_button_press[button_index] > OS_BTN_IS_PRESS_TIME_MAX) {
            blink_led_status();
        }
   } else { /* realse */
        if (time_button_press[button_index] >= OS_BTN_IS_PRESS_TIME_MIN
            && time_button_press[button_index] <= OS_BTN_IS_PRESS_TIME_MAX)
        {
            if (button_index < 3 ) /* Just have 3 leds */
            {
                Serial.println("Button was press");
                /* Control output */
                /* Send message to bluetooth */
                gpio_toggle(m_leds[button_index]);
            }
        } else if (time_button_press[button_index] > OS_BTN_IS_PRESS_TIME_MAX)
        {
            /* handler button remove alarm */
            Serial.println("Button was hold");
            /* clear alarm */
            m_device.alarm_is_set = 0;
            memset(m_device.m_time_alarm, 0, sizeof(m_device.m_time_alarm));
            Serial.println("clear alarm!");
        }

        time_button_press[button_index] = 0;
   }
}

static void button_init(void)
{
    int i;

    for(i=0; i < NUMBER_BUTTON; i++)
    {
        pinMode(m_buttons[i], INPUT_PULLUP);
    }
}

static void led_init(void)
{
    int i;

    for (i=0; i < NUMBER_LED; i++)
    {
        pinMode(m_leds[i], OUTPUT);
    }
    pinMode(LED_STATUS, OUTPUT);
    
    /* Turn off all */
    for (i=0; i < NUMBER_LED; i++)
    {
        digitalWrite(m_leds[i], LOW);
    }
    digitalWrite(LED_STATUS, LOW);
}

void led_button_init()
{
    button_init();
    led_init();
}

void button_handler(void)
{
    scan_button_handler(BUTTON1);
    scan_button_handler(BUTTON2);
    scan_button_handler(BUTTON3);
}