#ifndef _UNO_DATABASE_H_
#define _UNO_DATABASE_H_

#include "m_typedef.h"
#include "led_button.h"
#include "rtc.h"

#define EEPROM_DB_ADDR              0

#define USER_HABIT_TRACE_DAYS       5 /* Tracking 5 days to get user's habit */
#define USER_HABIT_HABIT_CMD        6

#define time_count_t    uint8_t

typedef struct {
    struct tm time_start_previous[USER_HABIT_TRACE_DAYS];
    uint16_t total_time_active_previous[USER_HABIT_TRACE_DAYS]; /* minutes */
    uint8_t is_tracking_time[NUMBER_CHANNEL]; /* 1 or 0 to identify that device is whether tracking process */
} m_user_habit_trace_t;

typedef struct {
    hc06_ctrl_t m_state_out[NUMBER_CHANNEL];

    /* Time active */
    struct tm time_active_on_day[NUMBER_CHANNEL];
    uint8_t time_active_in_week_tm_hour[NUMBER_CHANNEL];
    uint8_t time_active_in_week_tm_min[NUMBER_CHANNEL];
    uint8_t time_active_in_month_tm_hour[NUMBER_CHANNEL];
    uint8_t time_active_in_month_tm_min[NUMBER_CHANNEL];

    m_alarm_t m_time_alarm[MAX_CMD_ALARM];
    uint8_t alarm_is_set;

    uint8_t number_days_dev_active; /* count number of day device was active start from first day power up */

    /* User's habit */
    // struct tm users_habit[NUMBER_CHANNEL];
    uint8_t users_habit_is_set[NUMBER_CHANNEL]; /* true or flase */
    m_alarm_t users_habit_alarm[USER_HABIT_HABIT_CMD];
} device_info_t;

void eeprom_database_loader(void);
void eeprom_sync_database(void);

void uno_get_device_infor_jsonform(char *buff, uint16_t len);
void uno_update_current_state_switch(void);
void uno_caculate_time_device_active_loop(void);

void report_current_state(uint8_t cmd);
void uno_get_time_alarm_was_set(void);

void uno_get_time_active_in_day(void);
void uno_get_time_active_in_week(void);
void uno_get_time_active_in_month(void);

void uno_get_time_active_on_day(void);
void uno_get_time_active_in_week(void);
void uno_get_time_active_in_month(void);

#endif