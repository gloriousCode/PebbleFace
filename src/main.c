#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_task_layer;

static TextLayer *s_row_one_layer;
static TextLayer *s_row_two_layer;
static TextLayer *s_row_three_layer;

static GFont s_time_font;
static GFont s_task_font;
static GFont s_text_time_font;
static Layer *s_path_layer;
static GPath *s_path;
static GPathInfo PATH_INFO = {
  .num_points = 4,
  .points = (GPoint[]) { {00, 46},  {220, 46}, {220, 120}, {00, 120} }
};

//Important mini methods to get the int values of time
static int get_minutes(struct tm *tick_time) {
  static char minuteStr[] = "00";
  strftime(minuteStr, sizeof("00"), "%M", tick_time);
  int minutes = atoi(minuteStr); 
  
  return minutes;
}

static int get_hours(struct tm *tick_time) {
  static char hoursStr[] = "00";
  strftime(hoursStr, sizeof("00"), "%H", tick_time);
  int hours = atoi(hoursStr);
  
  return hours;
}

static int get_days(struct tm *tick_time) {
  static char dayStr[] = "0";
  strftime(dayStr, sizeof("0"), "%w", tick_time);
  int day = atoi(dayStr);
  
  return day;
}

static char* get_hour_str(struct tm *tick_time, int minutes) 
{
  static char hoursStr[] = "0000000";
  strftime(hoursStr, sizeof("00"), "%I", tick_time);
  int hours = atoi(hoursStr);
  if(minutes >= 40) {
    hours++;
  }
  if(hours == 1 || hours == 13){
    snprintf(hoursStr, 7, "ONE");
  }
    if(hours == 2 || hours == 14){
    snprintf(hoursStr, 7, "TWO");
  }
    if(hours == 3 || hours == 15){
    snprintf(hoursStr, 7, "THREE");
  }
    if(hours == 4 || hours == 16){
    snprintf(hoursStr, 7, "FOUR");
  }
    if(hours == 5 || hours == 17){
    snprintf(hoursStr, 7, "FIVE");
  }
    if(hours == 6 || hours == 18){
    snprintf(hoursStr, 7, "SIX");
  }
    if(hours == 7 || hours == 19){
    snprintf(hoursStr, 7, "SEVEN");
  }
    if(hours == 8 || hours == 20){
    snprintf(hoursStr, 7, "EIGHT");
  }
    if(hours == 9 || hours == 21){
    snprintf(hoursStr, 7, "NINE");
  }
    if(hours == 10 || hours ==22){
    snprintf(hoursStr, 7, "TEN");
  }
    if(hours == 11 || hours == 23){
    snprintf(hoursStr, 7, "ELEVEN");
  }
    if(hours == 12 || hours == 24){
    snprintf(hoursStr, 7, "TWELVE");
  }
  
  
  return hoursStr;
}


//Method to draw layer
static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorSunsetOrange);

  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_path);
}

static void declare_drawing_layer(Window *window) {
    Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  // Create GPath object
  s_path = gpath_create(&PATH_INFO);
  // Create Layer that the path will be drawn on
  s_path_layer = layer_create(bounds);
  layer_set_update_proc(s_path_layer, layer_update_proc);
  layer_add_child(window_layer, s_path_layer);
}
static void set_fonts() {
    // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_44));
  s_text_time_font =  fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_MEDIUM_30));
  s_task_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_ROBOTO_THIN_18));
}

static void declare_text_layers(Window *window) {
    // Create time TextLayer
  s_time_layer = text_layer_create(GRect(5, 52, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
    // Create task TextLayer
  s_task_layer = text_layer_create(GRect(5, 5, 100, 50));
  text_layer_set_background_color(s_task_layer, GColorClear);
  text_layer_set_text_color(s_task_layer, GColorWhite);
  
  s_row_one_layer = text_layer_create(GRect(5, 40, 200, 100));
  text_layer_set_background_color(s_row_one_layer, GColorClear);
  text_layer_set_text_color(s_row_one_layer, GColorWhite);
  
    s_row_two_layer = text_layer_create(GRect(5, 65, 200, 100));
  text_layer_set_background_color(s_row_two_layer, GColorClear);
  text_layer_set_text_color(s_row_two_layer, GColorWhite);
  
    s_row_three_layer = text_layer_create(GRect(5, 90, 200, 100));
  text_layer_set_background_color(s_row_three_layer, GColorClear);
  text_layer_set_text_color(s_row_three_layer, GColorWhite);
  
  set_fonts();
  
    // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_task_layer, s_task_font);
  
  text_layer_set_font(s_row_one_layer, s_text_time_font);
  text_layer_set_font(s_row_two_layer, s_text_time_font);
  text_layer_set_font(s_row_three_layer, s_text_time_font);
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_task_layer, GTextAlignmentCenter);
  
  text_layer_set_text_alignment(s_row_one_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_row_two_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_row_three_layer, GTextAlignmentLeft);
  
    // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_task_layer));
  
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_one_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_two_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_three_layer));
}

static void main_window_load(Window *window) {  
  declare_drawing_layer(window);  
  declare_text_layers(window);
}


//Perform certain functions depending on what time it is
static void update_task(struct tm *tick_time) {
  int hours = get_hours(tick_time);
  int minutes = get_minutes(tick_time);
  int day = get_days(tick_time);

  //strftime(dayChecker, sizeof("0"), "%w", tick_time);
  if (hours >= 8 && (hours <= 15 && minutes <=59)  && day >= 1 && day <= 5)
  {
    text_layer_set_text(s_task_layer, "WORK TIME");
  } 
  else if (hours == 16 && minutes <=59 && day >= 1 && day <= 5) 
  {
    text_layer_set_text(s_task_layer, "TRAVEL TIME");
  } 
  else if ((day == 0 && hours >= 10 && hours < 15) || ((hours >=17  && minutes <=59) && (day == 1 || day == 3 || day == 4))) 
  {
    text_layer_set_text(s_task_layer, "GYM TIME");
  }
  else
  {
    text_layer_set_text(s_task_layer, "FREE TIME");
  }
}

//First row of the time. Typically the minutes
static void set_row_one(struct tm *tick_time) {
  int minutes = get_minutes(tick_time);
  char * hoursStr = get_hour_str(tick_time, minutes);
  
  if(minutes > 56 || minutes <= 1) {
    text_layer_set_text(s_row_one_layer, hoursStr);
  }
  if(minutes > 1 && minutes <= 6) {
    text_layer_set_text(s_row_one_layer, "FIVE");
  }
  if(minutes > 6 && minutes <= 11) {
    text_layer_set_text(s_row_one_layer, "TEN");
  }
    if(minutes > 11 && minutes <= 16) {
    text_layer_set_text(s_row_one_layer, "QUARTER");
  }
  if(minutes > 16 && minutes <= 21) {
    text_layer_set_text(s_row_one_layer, "TWENTY");
  }
  if(minutes > 21 && minutes <= 26) {
    text_layer_set_text(s_row_one_layer, "TWENTY");
  }
  if(minutes > 26 && minutes <= 31) {
    text_layer_set_text(s_row_one_layer, "HALF");
  }
  if(minutes > 31 && minutes <= 36) {
    text_layer_set_text(s_row_one_layer, "TWENTY");
  }
  if(minutes > 36 && minutes <= 41) {
    text_layer_set_text(s_row_one_layer, "TWENTY");
  }
  if(minutes > 41 && minutes <= 46) {
    text_layer_set_text(s_row_one_layer, "QUARTER");
  }
  if(minutes > 46 && minutes <= 51) {
    text_layer_set_text(s_row_one_layer, "TEN");
  }
  if(minutes > 51 && minutes <= 56) {
    text_layer_set_text(s_row_one_layer, "FIVE");
  }
}
//Second row of the time. Typically how much past the hour
static void set_row_two(struct tm *tick_time) {
  int minutes = get_minutes(tick_time);  
  if(minutes > 56 || minutes <= 1) {
    text_layer_set_text(s_row_two_layer, "O'");
  }
  if(minutes > 1 && minutes <= 21) {
    text_layer_set_text(s_row_two_layer, "PAST");
  }
  if(minutes > 21 && minutes <= 26) {
    text_layer_set_text(s_row_two_layer, "FIVE PAST");
  }
  if(minutes > 26 && minutes <= 31) {
    text_layer_set_text(s_row_two_layer, "PAST");
  }
  if(minutes > 31 && minutes <= 36) {
    text_layer_set_text(s_row_two_layer, "FIVE TO");
  }
  if(minutes > 36 && minutes <= 56) {
    text_layer_set_text(s_row_two_layer, "TO");
  }
}
//Third row of the time. Usually the hour
static void set_row_three(struct tm *tick_time) {
  int minutes = get_minutes(tick_time);
  char * hoursStr = get_hour_str(tick_time, minutes);
  
  if(minutes > 56 || minutes <= 1) {
    text_layer_set_text(s_row_three_layer, "CLOCK");
  }
  else {
     text_layer_set_text(s_row_three_layer, hoursStr);
  }
  
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "0000";
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("0000"), "%H%M", tick_time);
    update_task(tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("0000"), "%I%M", tick_time);
  }
  // Display this time on the TextLayer
  //text_layer_set_text(s_time_layer, buffer);
  set_row_one(tick_time);
  set_row_two(tick_time);
  set_row_three(tick_time);
  

}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_unload(Window *window) {
  // Unload GFont
  fonts_unload_custom_font(s_time_font);  
  // Destroy layer and path
  layer_destroy(s_path_layer);
  gpath_destroy(s_path);
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_task_layer);
  // Time rendering
  text_layer_destroy(s_row_one_layer);
  text_layer_destroy(s_row_two_layer);
  text_layer_destroy(s_row_three_layer);
}
  
static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();
    // Specify custom Window color using byte
  window_set_background_color(s_main_window, GColorBlack);
  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
  // Make sure the time is displayed from the start
  update_time();
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
