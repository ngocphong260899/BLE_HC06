#include "handler_data.h"
#include <stdlib.h>
#include "m_typedef.h"
#include "led_button.h"
#include "uno_database.h"
#include "rtc.h"

extern device_info_t m_device;
extern uint8_t uno_sync_database_request;

/* {"cmd_type":0; "cmd":y} */
static void uno_handler_control_io(JsonDocument &_doc)
{
    uint8_t m_cmd = _doc["cmd"];
    
    control_device(m_cmd);
}

/* {cmd_type:1, "cmd":1, "hour":10, "minutes":10, "second":0} */
static void uno_handler_set_alarm(JsonDocument &_doc)
{
    char respond[256];
    uint8_t cmd = _doc["cmd"];

    if (m_device.alarm_is_set >= MAX_CMD_ALARM) {
        memset(respond, 0, sizeof(respond));
        sprintf(respond, "{\"cmd_type\":%d, \"res\":Fail}\n", SET_ALARM);
        uno_respond_app(respond);

        return;
    }

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "cmd: %d\n", cmd);
    Serial.println(respond);

    /* Do set time alarm here */
    m_device.m_time_alarm[m_device.alarm_is_set].m_time.tm_hour = _doc["hour"];
    m_device.m_time_alarm[m_device.alarm_is_set].m_time.tm_min = _doc["minutes"];
    m_device.m_time_alarm[m_device.alarm_is_set].m_time.tm_sec = _doc["second"];
    m_device.m_time_alarm[m_device.alarm_is_set].m_cmd = cmd;
    
    uno_sync_database_request = 1;
    Serial.print("alarm_is_set!");
    Serial.print("cmd: ");
    Serial.println(m_device.m_time_alarm[m_device.alarm_is_set].m_cmd);

    m_device.alarm_is_set ++;

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":%d, \"res\":OK}\n", SET_ALARM);
    uno_respond_app(respond);
}

/* {cmd_type:2, "cmd":x, "day":11, "month":11, "year":2020, "hour":10, "minutes":10, "second":0} */
void uno_handler_remove_alarm(JsonDocument &_doc)
{
    char respond[256];

    /* clear alarm */
    m_device.alarm_is_set = 0;
    memset(m_device.m_time_alarm, 0, sizeof(m_device.m_time_alarm));

    uno_sync_database_request = 1;

    memset(respond, 0, sizeof(respond));
    sprintf(respond, "{\"cmd_type\":%d, \"res\":OK}\n", RESET_ALARM);
    uno_respond_app(respond);
}

/* {"cmd_type":3} */
void uno_handler_query_info(void)
{
    report_current_state(3);
}

/* {"cmd_type":4} */
static void uno_handler_query_time_active_one_day()
{
    uno_get_time_active_on_day();
}

/* {"cmd_type":5} */
static void uno_handler_query_time_active_one_week()
{
    uno_get_time_active_in_week();
}

/* {"cmd_type":6} */
static void uno_handler_query_time_active_one_month()
{
    uno_get_time_active_in_month();
}

/* {"cmd_type":7} */
static void uno_handler_get_time_alarm()
{
    uno_get_time_alarm_was_set();
}

/* {"cmd_type":x; "cmd":y} */
void handler_data(char* command)
{
    if (command == NULL) {
        return;
    }

    DynamicJsonDocument doc(180);
    DeserializationError error = deserializeJson(doc, command);

    if (error) {
        Serial.print(F("Dungnt98 deserializeJson() failed\n"));
        Serial.println(error.c_str());
        return;
    }

    Serial.println(command);

    uint8_t cmd_type = (uint8_t)doc["cmd_type"];
    Serial.print("cmd_type: ");
    Serial.println(cmd_type);

    switch (cmd_type) {
    case CONTROL_IO: {
        uno_handler_control_io(doc);
    } break;

    case SET_ALARM: {
        uno_handler_set_alarm(doc);
    } break;

    case RESET_ALARM: {
        uno_handler_remove_alarm(doc);
    } break;

    case QUERY_INFOM: {
        uno_handler_query_info();

    } break;

    case QUERY_TIME_DAY: {
        uno_handler_query_time_active_one_day();

    } break;

    case QUERY_TIME_WEEK: {
        uno_handler_query_time_active_one_week();

    } break;

    case QUERY_TIME_MONTH: {
        uno_handler_query_time_active_one_month();

    } break;

    case GET_TIME_ALARM: {
        uno_handler_get_time_alarm();
    } break;

    default:
        Serial.println("Cmd_type was not match!");
        break;
    }
}