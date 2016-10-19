#pragma once  
#include <pebble.h>
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_TRAFFIC_TITLE 2
#define KEY_TRAFFIC_WARNING_ONE 3
#define KEY_TRAFFIC_WARNING_TWO 4
#define KEY_TRAFFIC_DIRECTION 5

void inbox_dropped_callback(AppMessageResult reason, void *context);
void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context);
void outbox_sent_callback(DictionaryIterator *iterator, void *context);
void inbox_received_callback(DictionaryIterator *iterator, void *context);
void trigger_js();