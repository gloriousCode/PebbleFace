#include <pebble.h>
#include "task.h"
#include "time.h"
#include "weather.h"
#include "javascripthandler.h"
#include "system.h"

  
Layer *s_task_color_layer;
Layer *s_weather_icon_layer;
Layer *s_calendar_background_layer;

TextLayer *s_time_layer;
TextLayer *s_task_layer;
TextLayer *s_row_one_layer;
TextLayer *s_row_two_layer;
TextLayer *s_row_three_layer;
TextLayer *s_weather_layer;
TextLayer *s_day_of_month_layer;
TextLayer *s_travel_row_one_layer;
TextLayer *s_travel_row_two_layer;
TextLayer *s_travel_row_three_layer;
TextLayer *s_travel_row_four_layer;

GFont s_time_font;
GFont s_task_font;
GFont s_text_time_font;
GFont s_text_time_bold_font;
GFont s_weather_font;
GFont s_day_of_month_font;
GFont s_travel_header_font;
GFont s_travel_row_font;

GDrawCommandImage* Weather_currentWeatherIcon;
GDrawCommandImage* calendar_background;
GPath *s_task_color_path;
GPathInfo s_task_color_path_info =
{
    .num_points = 4,
    .points = (GPoint[])
    {
      {00, 00}, {144, 00}, {144, 34}, {00, 34} 
    }
};


void render_calendar_background(Layer *layer, GContext *ctx)
{
    // Kill the old struct!
    GDrawCommandImage* oldImage = calendar_background;
    calendar_background = gdraw_command_image_create_with_resource(RESOURCE_ID_CALENDAR_BACKGROUND);
    gdraw_command_image_destroy(oldImage);
    gdraw_command_image_draw(ctx, calendar_background, GPoint(114, 4));
}

void declare_drawing_layer(Window *window)
{
    Layer *window_layer = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(window_layer);
    // Create GPath object
    s_task_color_path = gpath_create(&s_task_color_path_info);
    
    // Create Layer that the path will be drawn on
    s_task_color_layer = layer_create(bounds);
    s_weather_icon_layer = layer_create(bounds);
    s_calendar_background_layer = layer_create(bounds);
    
    taskColour = GColorRed;
    layer_set_update_proc(s_task_color_layer, task_background_color);
    layer_add_child(window_layer, s_task_color_layer);
    layer_add_child(window_layer, s_weather_icon_layer);
    layer_add_child(window_layer, s_calendar_background_layer);
    
    layer_set_update_proc(s_weather_icon_layer, set_weather_icon);
    layer_set_update_proc(s_calendar_background_layer, render_calendar_background);
}


void set_fonts()
{
    s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_36));
    s_text_time_font = fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
    s_task_font = fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
    s_text_time_bold_font = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
    s_weather_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    s_day_of_month_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    s_travel_header_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
    s_travel_row_font = fonts_get_system_font(FONT_KEY_GOTHIC_24);
}

void set_text_layer_bounds()
{
    // Create micro time TextLayer
    s_time_layer = text_layer_create(GRect(5, 34, 139, 50));
    text_layer_set_background_color(s_time_layer, GColorClear);
    text_layer_set_text_color(s_time_layer, GColorWhite);
    // Create task TextLayer
    s_task_layer = text_layer_create(GRect(5, 0, 100, 30));
    text_layer_set_background_color(s_task_layer, GColorClear);
    text_layer_set_text_color(s_task_layer, GColorWhite);
    //Create fuzzy time row one layer
    s_row_one_layer = text_layer_create(GRect(5, 32, 200, 100));
    text_layer_set_background_color(s_row_one_layer, GColorClear);
    text_layer_set_text_color(s_row_one_layer, GColorWhite);
    //Create fuzzy time row two layer
    s_row_two_layer = text_layer_create(GRect(5, 74, 200, 100));
    text_layer_set_background_color(s_row_two_layer, GColorClear);
    text_layer_set_text_color(s_row_two_layer, GColorWhite);
    //Create fuzzy time row three layer
    s_row_three_layer = text_layer_create(GRect(5, 116, 200, 100));
    text_layer_set_background_color(s_row_three_layer, GColorClear);
    text_layer_set_text_color(s_row_three_layer, GColorWhite);
    //Create weather layer
    s_weather_layer = text_layer_create(GRect(88, 2, 25, 25));
    text_layer_set_background_color(s_weather_layer, GColorClear);
    text_layer_set_text_color(s_weather_layer, GColorWhite);
    //Create day of month layer
    s_day_of_month_layer = text_layer_create(GRect(114, 2, 25, 25));
    text_layer_set_background_color(s_day_of_month_layer, GColorClear);
    text_layer_set_text_color(s_day_of_month_layer, GColorBlack);
    //Create travel header row layer
    s_travel_row_one_layer = text_layer_create(GRect(5, 70, 139, 30));
    text_layer_set_background_color(s_travel_row_one_layer, GColorClear);
    text_layer_set_text_color(s_travel_row_one_layer, GColorWhite);
    //Create travel data layer two
    s_travel_row_two_layer = text_layer_create(GRect(5, 90, 139, 50));
    text_layer_set_background_color(s_travel_row_two_layer, GColorClear);
    text_layer_set_text_color(s_travel_row_two_layer, GColorWhite);
    //Create travel data layer three
    s_travel_row_three_layer = text_layer_create(GRect(5, 110, 139, 50));
    text_layer_set_background_color(s_travel_row_three_layer, GColorClear);
    text_layer_set_text_color(s_travel_row_three_layer, GColorWhite);
    //Create travel data layer four
    s_travel_row_four_layer = text_layer_create(GRect(5, 130, 139, 50));
    text_layer_set_background_color(s_travel_row_four_layer, GColorClear);
    text_layer_set_text_color(s_travel_row_four_layer, GColorWhite);
}

void apply_fonts_set_alignment()
{
    // Apply to TextLayer
    text_layer_set_font(s_time_layer, s_time_font);
    text_layer_set_font(s_task_layer, s_task_font);
    
    text_layer_set_font(s_row_one_layer, s_text_time_font);
    text_layer_set_font(s_row_two_layer, s_text_time_font);
    text_layer_set_font(s_row_three_layer, s_text_time_font);
    text_layer_set_font(s_weather_layer, s_weather_font);
    text_layer_set_font(s_day_of_month_layer, s_day_of_month_font);
    text_layer_set_font(s_travel_row_one_layer, s_travel_header_font);
    text_layer_set_font(s_travel_row_two_layer, s_travel_row_font);
    text_layer_set_font(s_travel_row_three_layer, s_travel_row_font);
    text_layer_set_font(s_travel_row_four_layer, s_travel_row_font);
    
    text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
    text_layer_set_text_alignment(s_task_layer, GTextAlignmentLeft);
    
    text_layer_set_text_alignment(s_row_one_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_row_two_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_row_three_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_day_of_month_layer, GTextAlignmentCenter);
    //Travel time rows
    text_layer_set_text_alignment(s_travel_row_one_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_travel_row_two_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_travel_row_three_layer, GTextAlignmentLeft);
    text_layer_set_text_alignment(s_travel_row_four_layer, GTextAlignmentLeft);
}

void add_small_time_layer_to_window(Window *window)
{
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

void add_text_time_layers_to_window(Window *window)
{
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_one_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_two_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_three_layer));
}

void add_text_layers_to_window(Window *window)
{
    // Add it as a child layer to the Window's root layer
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_task_layer));
    add_small_time_layer_to_window(window);
    add_text_time_layers_to_window(window);
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_of_month_layer));
    
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_one_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_two_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_three_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_four_layer));
}


void set_font_weight() {
  
  if((minutes > 58 || minutes <= 3) && strcmp(timetextbufferRowThree, clock) !=0 )
  {
    text_layer_set_font(s_row_three_layer, s_text_time_font);
    text_layer_set_font(s_row_one_layer, s_text_time_bold_font);
  }
  else if((minutes > 23 && minutes <= 28) && strcmp(timetextbufferRowThree, five) !=0 )
  {
    text_layer_set_font(s_row_one_layer, s_text_time_bold_font);
    text_layer_set_font(s_row_three_layer, s_text_time_font);
  }
  else if((minutes >3 && minutes <= 23) || (minutes > 28 && minutes <=58))
  {
    text_layer_set_font(s_row_one_layer, s_text_time_font);
    text_layer_set_font(s_row_three_layer, s_text_time_bold_font);
  }
}

void render_small_clock() {
   time_t temp = time(NULL);
   struct tm *tick_time = localtime(&temp);
   text_layer_set_text(s_row_one_layer, strEmpty);
   text_layer_set_text(s_row_two_layer, strEmpty);
   text_layer_set_text(s_row_three_layer, strEmpty);
   strftime(buffer, sizeof("0000"), "%H%M", tick_time);
   text_layer_set_text(s_time_layer, buffer);
}

void render_text_clock() {
  get_row_one_text();
  get_row_two_text();
  get_row_three_text();
  
  text_layer_set_text(s_row_one_layer, timetextbufferRowOne);
  text_layer_set_text(s_row_two_layer, timetextbufferRowTwo);
  text_layer_set_text(s_row_three_layer, timetextbufferRowThree);
  text_layer_set_text(s_time_layer, strEmpty);
  
  set_font_weight();
}

void update_time_layers() {
  layer_set_update_proc(s_task_color_layer, task_background_color);
  text_layer_set_text(s_task_layer,currentTask);
  text_layer_set_text(s_day_of_month_layer, dayOfMonthTextBuffer);
  text_layer_set_text(s_travel_row_one_layer, task_text_row_one);
  text_layer_set_text(s_travel_row_two_layer, task_text_row_two);
  text_layer_set_text(s_travel_row_three_layer, task_text_row_three);
  text_layer_set_text(s_travel_row_four_layer, task_text_row_four);
  

  
  if (is_morning_prep_time() || is_afternoon_prep_time() || is_travel_time())
  {
    render_small_clock();
  }
  else {
    render_text_clock();
  }
}

void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time();
  update_weather();
}
