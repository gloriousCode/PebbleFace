#include <pebble.h>
#include "javascripthandler.h"
#include "system.h"
#include "time.h"  
#include "task.h"
#include "weather.h"
  
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_TRAFFIC_TITLE 2
#define KEY_TRAFFIC_WARNING_ONE 3
#define KEY_TRAFFIC_WARNING_TWO 4
#define KEY_TRAFFIC_DIRECTION 5

void trigger_js() {
  // Begin dictionary
  DictionaryIterator *iter;
  app_message_outbox_begin(&iter);
  // Add a key-value pair
  dict_write_uint8(iter, 0, 0);
  // Send the message!
  app_message_outbox_send();
}
  
void inbox_dropped_callback(AppMessageResult reason, void *context)
{
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
    APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
    APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

void inbox_received_callback(DictionaryIterator *iterator, void *context)
{
  // Store incoming information
  static char temperature_buffer[3];
  static char weather_layer_buffer[3];
  static char title_buffer[100];
  static char warning_buffer[100];
  static char warningb_buffer[100];
  static char suburb_buffer[100];
  
    Tuple *t = dict_read_first(iterator);
    while(t != NULL)
    {
      if(t->key == KEY_TEMPERATURE) {
        snprintf(temperature_buffer, sizeof(temperature_buffer), "%d", (int)t->value->int32);
        snprintf(weather_layer_buffer, sizeof(weather_layer_buffer), temperature_buffer);
        text_layer_set_text(s_weather_layer, weather_layer_buffer);
      }
      else if (t->key == KEY_CONDITIONS) {
        conditionId = (int *)t->value->int32;
        layer_set_update_proc(s_weather_icon_layer, set_weather_icon);
      }
      else if(currentlyPrepTime || currentlyTravelTime) {
        if (t->key == KEY_TRAFFIC_TITLE ) {
          snprintf(title_buffer, sizeof(title_buffer), "%s", t->value->cstring);
          text_layer_set_text(s_travel_row_one_layer, title_buffer);
          light_enable_interaction();
        }
        else if (t->key == KEY_TRAFFIC_WARNING_ONE && (currentlyPrepTime || currentlyTravelTime)) {
          snprintf(warning_buffer, sizeof(warning_buffer), "%s", t->value->cstring);
          text_layer_set_text(s_travel_row_two_layer, warning_buffer);
        }
        else if (t->key == KEY_TRAFFIC_WARNING_TWO && (currentlyPrepTime || currentlyTravelTime)) {
          snprintf(warningb_buffer, sizeof(warningb_buffer), "%s", t->value->cstring);
          text_layer_set_text(s_travel_row_three_layer, warningb_buffer);
        }
        else if (t->key == KEY_TRAFFIC_DIRECTION && (currentlyPrepTime || currentlyTravelTime)) {
          snprintf(suburb_buffer, sizeof(suburb_buffer), "%s", t->value->cstring);
          text_layer_set_text(s_travel_row_four_layer, suburb_buffer);
        }
      }
      t = dict_read_next(iterator);
    }
}