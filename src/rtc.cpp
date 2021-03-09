#include "rtc.h"
#include <time.h>
#include "m_typedef.h"
#include "led_button.h"
#include "uno_database.h"

extern device_info_t m_device;
extern m_user_habit_trace_t user_habit[NUMBER_CHANNEL];

RTC_DS1307 rtc;
DateTime now;
struct tm m_time_local;
uint8_t alarm_is_set = 0;

struct tm temp_time_start[NUMBER_CHANNEL];
uint16_t temp_time_start_total_time_active[NUMBER_CHANNEL]; /* minutes */

void rtc_init(void)
{
    if (Serial) {
        if (! rtc.begin()) {
            Serial.print("Couldn't find RTC");
            while (1);
        }

        if (!rtc.isrunning()) {
            Serial.print("RTC is NOT running!");
            Serial.println();
        }

        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
}

static void time_alarm() {
    uint8_t i;

    for (i = 0; i < m_device.alarm_is_set; i++) {
        if (now.hour() == m_device.m_time_alarm[i].m_time.tm_hour
            && now.minute() == m_device.m_time_alarm[i].m_time.tm_min)
        {
            Serial.print("Do Alarm Hanlder with cmd: ");
            Serial.println(m_device.m_time_alarm[i].m_cmd);
            control_device(m_device.m_time_alarm[i].m_cmd);
        }
    }
}

static void control_with_users_habit() {

    /* If device active time was not pass time to get user's habit */
    if  (m_device.number_days_dev_active < USER_HABIT_TRACE_DAYS) {
        return;
    }

    uint8_t i;

    for (i = 0; i < NUMBER_CHANNEL; i++) {
        if (m_device.users_habit_is_set[i]) {
            if (now.hour() == m_device.users_habit_alarm[i].m_time.tm_hour
                && now.minute() == m_device.users_habit_alarm[i].m_time.tm_min)
            {
                control_device(m_device.users_habit_alarm[i].m_cmd); /* turn on channel */
            }
        }
    }
}

extern uint8_t uno_sync_database_request;

/* run each 10 second */
void uno_caculate_time_device_active_loop(void)
{
    uint8_t dev_index;

    /* CHANNEL1 */
    dev_index = CHANNEL1;
    if (digitalRead(LED1_PIN)) {
        /* Count total time active on day */
        m_device.time_active_on_day[dev_index].tm_sec += 10; /* 10s */
        if (m_device.time_active_on_day[dev_index].tm_sec >= 60) { /* minutes */
            m_device.time_active_on_day[dev_index].tm_min ++;
            m_device.time_active_in_week_tm_min[dev_index] ++;
            m_device.time_active_in_month_tm_min[dev_index] ++;
            
            m_device.time_active_on_day[dev_index].tm_sec = 0;
            
            if (m_device.time_active_on_day[dev_index].tm_min >= 60) { /* hours */
                m_device.time_active_on_day[dev_index].tm_min = 0;

                /* Count time active on day */
                m_device.time_active_on_day[dev_index].tm_hour ++;
                /* Count time active on week */
                m_device.time_active_in_week_tm_hour[dev_index] ++;
                /* Count time active on month */
                m_device.time_active_in_month_tm_hour[dev_index] ++;
            }

            /* Count time for tracking user's habit */
            if (user_habit[dev_index].is_tracking_time[dev_index]) {
                temp_time_start_total_time_active[dev_index] ++; /* 1 minutes */
            }
        }

        /* Tracking user's habit */
        if (0 == user_habit[dev_index].is_tracking_time[dev_index]) {
            user_habit[dev_index].is_tracking_time[dev_index] = 1; /* start tracking */

            /* Get time device turn on */
            temp_time_start[dev_index].tm_hour = now.hour();
            temp_time_start[dev_index].tm_min = now.minute(); 
        }

        /* sync database */
        uno_sync_database_request = 1;
    }
    else {
        user_habit[dev_index].is_tracking_time[dev_index] = 0; /* stop tracking */

        /* Check time device was used to get user's habit */
        if (temp_time_start_total_time_active[dev_index] > user_habit[dev_index].total_time_active_previous[m_device.number_days_dev_active]) {
            /* Store user's using device time */
            user_habit[dev_index].total_time_active_previous[m_device.number_days_dev_active] = temp_time_start_total_time_active[dev_index];
            user_habit[dev_index].time_start_previous[m_device.number_days_dev_active].tm_hour = temp_time_start[dev_index].tm_hour;
            user_habit[dev_index].time_start_previous[m_device.number_days_dev_active].tm_min = temp_time_start[dev_index].tm_min;
        }
    }

    /* CHANNEL2 */
    dev_index = CHANNEL2;
    if (digitalRead(LED2_PIN)) {
        /* Count total time active on day */
        m_device.time_active_on_day[dev_index].tm_sec += 10; /* 10s */
        if (m_device.time_active_on_day[dev_index].tm_sec >= 60) { /* minutes */
            m_device.time_active_on_day[dev_index].tm_min ++;
            m_device.time_active_in_week_tm_min[dev_index] ++;
            m_device.time_active_in_month_tm_min[dev_index] ++;
            
            m_device.time_active_on_day[dev_index].tm_sec = 0;
            
            if (m_device.time_active_on_day[dev_index].tm_min >= 60) { /* hours */
                m_device.time_active_on_day[dev_index].tm_min = 0;

                /* Count time active on day */
                m_device.time_active_on_day[dev_index].tm_hour ++;
                /* Count time active on week */
                m_device.time_active_in_week_tm_hour[dev_index] ++;
                /* Count time active on month */
                m_device.time_active_in_month_tm_hour[dev_index] ++;
            }

            /* Count time for tracking user's habit */
            if (user_habit[dev_index].is_tracking_time[dev_index]) {
                temp_time_start_total_time_active[dev_index] ++; /* 1 minutes */
            }
        }

        /* Tracking user's habit */
        if (0 == user_habit[dev_index].is_tracking_time[dev_index]) {
            user_habit[dev_index].is_tracking_time[dev_index] = 1; /* start tracking */

            /* Get time device turn on */
            temp_time_start[dev_index].tm_hour = now.hour();
            temp_time_start[dev_index].tm_min = now.minute(); 
        }

        /* sync database */
        uno_sync_database_request = 1;
    }
    else {
        user_habit[dev_index].is_tracking_time[dev_index] = 0; /* stop tracking */

        /* Check time device was used to get user's habit */
        if (temp_time_start_total_time_active[dev_index] > user_habit[dev_index].total_time_active_previous[m_device.number_days_dev_active]) {
            /* Store user's using device time */
            user_habit[dev_index].total_time_active_previous[m_device.number_days_dev_active] = temp_time_start_total_time_active[dev_index];
            user_habit[dev_index].time_start_previous[m_device.number_days_dev_active].tm_hour = temp_time_start[dev_index].tm_hour;
            user_habit[dev_index].time_start_previous[m_device.number_days_dev_active].tm_min = temp_time_start[dev_index].tm_min;
        }
    }

    /* CHANNEL3 */
    dev_index = CHANNEL3;
    if (digitalRead(LED3_PIN)) {
        /* Count total time active on day */
        m_device.time_active_on_day[dev_index].tm_sec += 10; /* 10s */
        if (m_device.time_active_on_day[dev_index].tm_sec >= 60) { /* minutes */
            m_device.time_active_on_day[dev_index].tm_min ++;
            m_device.time_active_in_week_tm_min[dev_index] ++;
            m_device.time_active_in_month_tm_min[dev_index] ++;
            
            m_device.time_active_on_day[dev_index].tm_sec = 0;
            
            if (m_device.time_active_on_day[dev_index].tm_min >= 60) { /* hours */
                m_device.time_active_on_day[dev_index].tm_min = 0;

                /* Count time active on day */
                m_device.time_active_on_day[dev_index].tm_hour ++;
                /* Count time active on week */
                m_device.time_active_in_week_tm_hour[dev_index] ++;
                /* Count time active on month */
                m_device.time_active_in_month_tm_hour[dev_index] ++;
            }

            /* Count time for tracking user's habit */
            if (user_habit[dev_index].is_tracking_time[dev_index]) {
                temp_time_start_total_time_active[dev_index] ++; /* 1 minutes */
            }
        }

        /* Tracking user's habit */
        if (0 == user_habit[dev_index].is_tracking_time[dev_index]) {
            user_habit[dev_index].is_tracking_time[dev_index] = 1; /* start tracking */

            /* Get time device turn on */
            temp_time_start[dev_index].tm_hour = now.hour();
            temp_time_start[dev_index].tm_min = now.minute(); 
        }

        /* sync database */
        uno_sync_database_request = 1;
    }
    else {
        user_habit[dev_index].is_tracking_time[dev_index] = 0; /* stop tracking */

        /* Check time device was used to get user's habit */
        if (temp_time_start_total_time_active[dev_index] > user_habit[dev_index].total_time_active_previous[m_device.number_days_dev_active]) {
            /* Store user's using device time */
            user_habit[dev_index].total_time_active_previous[m_device.number_days_dev_active] = temp_time_start_total_time_active[dev_index];
            user_habit[dev_index].time_start_previous[m_device.number_days_dev_active].tm_hour = temp_time_start[dev_index].tm_hour;
            user_habit[dev_index].time_start_previous[m_device.number_days_dev_active].tm_min = temp_time_start[dev_index].tm_min;
        }
    }
}

static void caculate_user_habit(uint8_t dev_index)
{
    /* If device was set time active depend on user's habit before */
    if (m_device.users_habit_is_set[dev_index] != 0) {
        return;
    }

    uint8_t i = 0;
    uint8_t habit_day_was_select = 0;
    uint16_t time_active_longest = 0;

    time_active_longest = user_habit[dev_index].total_time_active_previous[habit_day_was_select];
    /* Caculating longest time active in 5 days */
    for (i = 1; i < USER_HABIT_TRACE_DAYS; i++) {
        if (user_habit[dev_index].total_time_active_previous[i] > time_active_longest)
        {
            time_active_longest = user_habit[dev_index].total_time_active_previous[i];
            habit_day_was_select = i;
        }
    }

    /* Set this device had alarm by user's habit */
    m_device.users_habit_is_set[dev_index] = 1;

    /* Set time ALARM for User's Habit */
    m_device.users_habit_alarm[dev_index].m_cmd = uno_get_cmd_for_pin(dev_index, 1); /* turn on */
    m_device.users_habit_alarm[dev_index].m_time.tm_hour = user_habit[dev_index].time_start_previous[habit_day_was_select].tm_hour;
    m_device.users_habit_alarm[dev_index].m_time.tm_min = user_habit[dev_index].time_start_previous[habit_day_was_select].tm_min;
    m_device.users_habit_alarm[dev_index+1].m_cmd = uno_get_cmd_for_pin(dev_index, 0); /* turn off */
    m_device.users_habit_alarm[dev_index+1].m_time.tm_hour = m_device.users_habit_alarm[dev_index].m_time.tm_hour + (time_active_longest / 60);
    m_device.users_habit_alarm[dev_index+1].m_time.tm_min = m_device.users_habit_alarm[dev_index].m_time.tm_min + (time_active_longest % 60);
}

void uno_set_user_habit(void)
{
    caculate_user_habit(CHANNEL1);
    caculate_user_habit(CHANNEL2);
    caculate_user_habit(CHANNEL3);
}

void alarm_handler()
{
    if (m_device.alarm_is_set) {
        time_alarm();
    }
}

void rtc_hander(void)
{
    now = rtc.now();

    /* Check End of day - 0h, update time active every day */
    if ((now.hour() == 0) && (now.minute() == 0) && (now.second() == 0))
    {
        /* Count number of day device was active, just count first five days */
        if (m_device.number_days_dev_active < USER_HABIT_TRACE_DAYS) {
            m_device.number_days_dev_active ++;

            if (m_device.number_days_dev_active == USER_HABIT_TRACE_DAYS) {
                uno_set_user_habit();
            }
        }

        if (now.dayOfTheWeek() == 0) { /* End of the week */
            /* Reset data of week to use for new week */
            memset(m_device.time_active_in_week_tm_hour, 0, sizeof(m_device.time_active_in_week_tm_hour));
            memset(m_device.time_active_in_week_tm_min, 0, sizeof(m_device.time_active_in_week_tm_min));

            /* Reset data to use for new month */
            if (now.day() == 1) {
                memset(m_device.time_active_in_month_tm_hour, 0, sizeof(m_device.time_active_in_week_tm_hour));
                memset(m_device.time_active_in_month_tm_min, 0, sizeof(m_device.time_active_in_week_tm_min));
            }
        }

        /* clear daily data */
        memset(&m_device.time_active_on_day, 0, sizeof(m_device.time_active_on_day));
    }

    /* Caculate user's habit */
    control_with_users_habit();
}


