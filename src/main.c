#include <pebble.h>
#include "task.h"
#include "time.h"
#include "weather.h"
#include "javascripthandler.h"
#include "system.h"

static Window *s_main_window;

static void main_window_unload(Window *window)
{
  // Shoot the custom fonts
  fonts_unload_custom_font(s_time_font);
  // Kill the images
  layer_destroy(s_calendar_background_layer);
  // Murder the icons
  gdraw_command_image_destroy(Weather_currentWeatherIcon);
  gdraw_command_image_destroy(calendar_background);
  // Destroy TextLayers
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_task_layer);
  text_layer_destroy(s_row_one_layer);
  text_layer_destroy(s_row_two_layer);
  text_layer_destroy(s_row_three_layer);
  text_layer_destroy(s_weather_layer);
  text_layer_destroy(s_day_of_month_layer);
  text_layer_destroy(s_travel_row_one_layer);
  text_layer_destroy(s_travel_row_two_layer);
  text_layer_destroy(s_travel_row_three_layer);
  text_layer_destroy(s_travel_row_four_layer);
  // Waterboard the layers
  layer_destroy(s_task_color_layer);
  gpath_destroy(s_task_color_path);
  layer_destroy(s_weather_icon_layer);
}

static void main_window_load(Window *window)
{
    declare_drawing_layer(window);
    set_text_layer_bounds();
    set_fonts();
    apply_fonts_set_alignment();
    add_text_layers_to_window(window);
}

static void init()
{
    // Create main Window element and assign to pointer
    s_main_window = window_create();
    // Specify custom Window color using byte
    window_set_background_color(s_main_window, GColorBlack);
    // Set handlers to manage the elements inside the Window
    window_set_window_handlers(s_main_window, (WindowHandlers)
    {
        .load = main_window_load,
        .unload = main_window_unload
    });
    // Register callbacks
    app_message_register_inbox_received(inbox_received_callback);
    // Open AppMessage
    app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    app_message_register_inbox_dropped(inbox_dropped_callback);
    app_message_register_outbox_failed(outbox_failed_callback);
    app_message_register_outbox_sent(outbox_sent_callback);
    // Show the Window on the watch, with animated=true
    window_stack_push(s_main_window, true);
    // Make sure the time is displayed from the start
    update_time();
    // Register with TickTimerService
    tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit()
{
    // Destroy Window
    window_destroy(s_main_window);
}

int main(void)
{
    init();
    app_event_loop();
    deinit();
}