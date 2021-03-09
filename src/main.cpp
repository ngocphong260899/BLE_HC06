#include <Arduino.h>
#include <Wire.h> 
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <PZEM004Tv30.h>

#include "RTClib.h"
#include "led_button.h"
#include "hc06.h"
#include "rtc.h"
#include "pzem004t.h"
#include "uno_database.h"

#define TIME_HANDLER_BUTTON         20
#define TIME_HANDLER_LCD            10
#define TIME_HANDLER_HC06           40
#define TIME_HANDLER_RTC            1000
#define TIME_HANDLER_ALARM          (1000 * 30)
#define TIME_HANDLER_PZEM004T       5000
#define TIME_COUNT_TIME_ACTIVE      10000 /* 10s */
#define TIME_SYNC_DATABASE          20000 /* 10s */

extern uint8_t uno_sync_database_request;

static uint32_t time_slice = 0;
static uint32_t time_handler_button_before = 0;
static uint32_t time_handler_hc06_before = 0;
static uint32_t time_handler_rtc_before = 0;
static uint32_t time_handler_alarm_before = 0;
static uint32_t time_handler_pzem004t_before = 0;
static uint32_t time_count_time_active_before = 0;
static uint32_t time_handler_sync_db_before = 0;

void setup()
{
    hc06_init();
    eeprom_database_loader();
    rtc_init();
    pzem_init();
    led_button_init();
    Serial.println("system init success");
}

void button_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_button_before > TIME_HANDLER_BUTTON) {
        button_handler();
        time_handler_button_before = time_slice;
    }
}

static void hc06_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_hc06_before > TIME_HANDLER_HC06) {
        hc06_hander();
        time_handler_hc06_before = time_slice;
    }
}

static void rtc_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_rtc_before > TIME_HANDLER_RTC) {
        rtc_hander();
        time_handler_rtc_before = time_slice;
    }
}

static void alarm_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_alarm_before > TIME_HANDLER_ALARM) {
        alarm_handler();
        time_handler_alarm_before = time_slice;
    }
}

static void pzem004t_loop()
{
    time_slice = millis();

    if (time_slice - time_handler_pzem004t_before > TIME_HANDLER_PZEM004T) {
        pzem_hander();
        time_handler_pzem004t_before = time_slice;
    }
}

static void uno_count_time_device_active()
{
    time_slice = millis();

    if (time_slice - time_count_time_active_before > TIME_COUNT_TIME_ACTIVE) {
        uno_caculate_time_device_active_loop();
        time_count_time_active_before = time_slice;
    }
}

static void uno_sync_database()
{
    time_slice = millis();

    if (time_slice - time_handler_sync_db_before > TIME_SYNC_DATABASE) {
        if (uno_sync_database_request != 0) {
            eeprom_sync_database();
            time_handler_sync_db_before = time_slice;
        }
    }
}

void loop()
{
    button_loop();
    hc06_loop();
    rtc_loop();
    alarm_loop();
    pzem004t_loop();
    uno_count_time_device_active();
    uno_sync_database();
}