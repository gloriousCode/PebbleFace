#pragma once
#include <pebble.h>
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
GPathInfo s_task_color_path_info;

void render_calendar_background();
void declare_drawing_layer();
void set_fonts();
void set_text_layer_bounds();
void apply_fonts_set_alignment();
void add_small_time_layer_to_window();
void add_text_time_layers_to_window();
void add_text_layers_to_window();
void set_font_weight();
void render_small_clock();
void render_text_clock();
void update_time_layers();
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
