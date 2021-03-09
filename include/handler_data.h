#ifndef _HANDLER_DATA_H_
#define _HANDLER_DATA_H_

#include <ArduinoJson.h>

void handler_data(char* command);

void uno_handler_query_info(void);
void uno_handler_remove_alarm(JsonDocument &_doc);

#endif