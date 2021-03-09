#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include <Arduino.h>

#define HC06    Serial1


typedef enum {
    CH1_ON  = 1,
    CH1_OFF = 2,
    CH2_ON  = 3,
    CH2_OFF = 4,
    CH3_ON  = 5,
    CH3_OFF = 6,
} hc06_ctrl_t;

typedef enum {
    CONTROL_IO = 0,
    SET_ALARM,
    RESET_ALARM,
    QUERY_INFOM,
    QUERY_TIME_DAY,
    QUERY_TIME_WEEK,
    QUERY_TIME_MONTH,
    GET_TIME_ALARM
} hc06_cmd_type_t;

typedef struct {
    struct tm m_time;
    uint8_t m_cmd;
} m_alarm_t;


#define uno_respond_app(respond) \
            do { \
                HC06.print(respond); \
            } while(0)

#endif