#include <pebble.h>
  
static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_task_layer;
static TextLayer *s_row_one_layer;
static TextLayer *s_row_two_layer;
static TextLayer *s_row_three_layer;
static TextLayer *s_weather_layer;

static GFont s_time_font;
static GFont s_task_font;
static GFont s_text_time_font;
static GFont s_text_time_bold_font;
static GFont s_weather_font;

static Layer *s_task_color_layer;
static GPath *s_task_color_path;
static GPathInfo s_task_color_path_info = {
  .num_points = 4,
  .points = (GPoint[]) { {00, 00},  {144, 00}, {144, 34}, {00, 34} }
};

//Cool sys variables
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1


//Bools
bool currentlyWorkTime = false;
bool currentlyPrepTime = false;
bool currentlyGymTime = false;
bool currentlyCodeTime = false;
bool currentlyStudyTime = false;
bool currentlyReadTime = false;
bool currentlyTravelTime = false;
bool currentlyFreeTime = false;

//Constants
const char * clock = "clock";
const char * o = "o'";
const char * past = "past";
const char * fivePast = "five past";
const char * to = "to";
const char * fiveTo = "five to";

const char * quarter = "quarter";
const char * twenty = "twenty";
const char * half = "half";

const char * one = "one";
const char * two = "two";
const char * three = "three";
const char * four = "four";
const char * five = "five";
const char * six = "six";
const char * seven = "seven";
const char * eight = "eight";
const char * nine = "nine";
const char * ten = "ten";
const char * eleven = "eleven";
const char * twelve = "twelve";

const char * workTime = "WORK";
const char * prepTime = "PREP";
const char * gymTime = "GYM";
const char * codeTime = "CODE";
const char * studyTime = "STUDY";
const char * readTime = "READ";
const char * travelTime = "TRAVEL";
const char * freeTime = "FREEDOM";

const char * strEmpty = "";

int days = 0;
int minutes = 0;
int hours = 0;

//All the long lived buffers
static char buffer[] = "0000";
static char minuteStr[] = "00";
static char hoursStr[] = "00";
static char dayStr[] = "0";
static char timetextbuffer[] = "000000000";
static char timetextbufferRowOne[] = "00000000";
static char timetextbufferRowTwo[] = "00000000";
static char timetextbufferRowThree[] = "00000000";

//Method to draw layer
static void task_background_red(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorRed);
  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_task_color_path);
}

static void task_background_blue(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorBlueMoon);
  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_task_color_path);
}

static void task_background_orange(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorOrange);
  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_task_color_path);
}

static void task_background_purple(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorPurple);
  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_task_color_path);
}

static void task_background_green(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorIslamicGreen);
  // Draw the filled shape in above color
  gpath_draw_filled(ctx, s_task_color_path);
}

static void declare_drawing_layer(Window *window) {
   Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  // Create GPath object
  s_task_color_path = gpath_create(&s_task_color_path_info);
  // Create Layer that the path will be drawn on
  s_task_color_layer = layer_create(bounds);
  layer_set_update_proc(s_task_color_layer, task_background_red);
  layer_add_child(window_layer, s_task_color_layer);
}



//Important mini methods to get the int values of time
static void set_minutes(struct tm *tick_time) {
  strftime(minuteStr, sizeof("00"), "%M", tick_time);
  minutes = atoi(minuteStr); 
}

static void set_hours(struct tm *tick_time) {
  strftime(hoursStr, sizeof("00"), "%H", tick_time);
  hours = atoi(hoursStr);
}

static void set_days(struct tm *tick_time) {
  strftime(dayStr, sizeof("0"), "%w", tick_time);
  days = atoi(dayStr);
}

static void set_hours_string(struct tm *tick_time) 
{
  int timeHours = hours;
  if(minutes > 33) {
    timeHours++;
  }
  if(timeHours == 1 || timeHours == 13){
    snprintf(timetextbuffer, 4, one);
  }
    if(timeHours == 2 || timeHours == 14){
    snprintf(timetextbuffer, 4, two);
  }
    if(timeHours == 3 || timeHours == 15){
    snprintf(timetextbuffer, 6, three);
  }
    if(timeHours == 4 || timeHours == 16){
    snprintf(timetextbuffer, 5, four);
  }
    if(timeHours == 5 || timeHours == 17){
    snprintf(timetextbuffer, 5, five);
  }
    if(timeHours == 6 || timeHours == 18){
    snprintf(timetextbuffer, 4, six);
  }
    if(timeHours == 7 || timeHours == 19){
    snprintf(timetextbuffer, 6, seven);
  }
    if(timeHours == 8 || timeHours == 20){
    snprintf(timetextbuffer, 6, eight);
  }
    if(timeHours == 9 || timeHours == 21){
    snprintf(timetextbuffer, 5, nine);
  }
    if(timeHours == 10 || timeHours ==22){
    snprintf(timetextbuffer, 4, ten);
  }
    if(timeHours == 11 || timeHours == 23){
    snprintf(timetextbuffer, 8, eleven);
  }
    if(timeHours == 12 || timeHours == 24){
    snprintf(timetextbuffer, 8, twelve);
  }  
}


static void set_fonts() {
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_36));
  s_text_time_font =  fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
  s_task_font =  fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD);
  s_text_time_bold_font = fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD);
  s_weather_font = fonts_get_system_font(FONT_KEY_GOTHIC_14);
}

static void set_text_layer_bounds() {
  // Create micro time TextLayer
  s_time_layer = text_layer_create(GRect(5, 40, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  // Create task TextLayer
  s_task_layer = text_layer_create(GRect(5, 0, 100, 30));
  text_layer_set_background_color(s_task_layer, GColorClear);
  text_layer_set_text_color(s_task_layer, GColorWhite);
  //Create fuzzy time row one layer
  s_row_one_layer = text_layer_create(GRect(5, 34, 200, 100));
  text_layer_set_background_color(s_row_one_layer, GColorClear);
  text_layer_set_text_color(s_row_one_layer, GColorWhite);
  //Create fuzzy time row two layer
  s_row_two_layer = text_layer_create(GRect(5, 74, 200, 100));
  text_layer_set_background_color(s_row_two_layer, GColorClear);
  text_layer_set_text_color(s_row_two_layer, GColorWhite);
  //Create fuzzy time row three layer
  s_row_three_layer = text_layer_create(GRect(5, 114, 200, 100));
  text_layer_set_background_color(s_row_three_layer, GColorClear);
  text_layer_set_text_color(s_row_three_layer, GColorWhite);
  //Create weather layer
  s_weather_layer = text_layer_create(GRect(80, 14, 100, 30));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);

  
}

static void apply_fonts_set_alignment()  {
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_task_layer, s_task_font);
  
  text_layer_set_font(s_row_one_layer, s_text_time_font);
  text_layer_set_font(s_row_two_layer, s_text_time_font);
  text_layer_set_font(s_row_three_layer, s_text_time_font);
  text_layer_set_font(s_weather_layer, s_weather_font);
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_task_layer, GTextAlignmentLeft);
  
  text_layer_set_text_alignment(s_row_one_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_row_two_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_row_three_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_weather_layer, GTextAlignmentLeft);
}

static void add_small_time_layer_to_window(Window *window){
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
}

static void add_text_time_layers_to_window(Window *window) {
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_one_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_two_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_three_layer));
}

static void add_text_layers_to_window(Window *window) {
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_task_layer));
  add_small_time_layer_to_window(window);
  add_text_time_layers_to_window(window);
}

static void setTasksToFalse() {
  currentlyWorkTime = false;
  currentlyPrepTime = false;
  currentlyGymTime = false;
  currentlyCodeTime = false;
  currentlyStudyTime = false;
  currentlyReadTime = false;
  currentlyTravelTime = false;
  currentlyFreeTime = false;
}

//Perform certain functions depending on what time it is
static void update_task(struct tm *tick_time) {
  if ((hours >= 15 && hours <= 16) && minutes >44 && days >= 1 && days <= 5) {
    if(!currentlyPrepTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyPrepTime = true;
      
      text_layer_set_text(s_task_layer, prepTime);
      layer_set_update_proc(s_task_color_layer, task_background_purple);
    }
  } else if ((hours >= 8 && hours <= 15) && days >= 1 && days <= 5)
  {
    if(!currentlyWorkTime) {   
      vibes_double_pulse();
      setTasksToFalse();
      currentlyWorkTime = true;
      
      text_layer_set_text(s_task_layer, workTime);
      layer_set_update_proc(s_task_color_layer, task_background_blue);
    }
  } 
  else if (hours == 16 && minutes <=59 && days >= 1 && days <= 5) 
  {
    if(!currentlyTravelTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyTravelTime = true;

      text_layer_set_text(s_task_layer, travelTime);
      layer_set_update_proc(s_task_color_layer, task_background_orange);
    }
  } 
  else if ((days == 0 && (hours >= 10 && hours < 15)) || (hours >=17  && minutes <=59 && (days == 1 || days == 3 || days == 4))) 
  {
    if(!currentlyGymTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyGymTime = true;
      
      text_layer_set_text(s_task_layer, gymTime);
      layer_set_update_proc(s_task_color_layer, task_background_red);
    }
  }
  else 
  {
    if(!currentlyFreeTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyFreeTime = true;
      
      text_layer_set_text(s_task_layer,freeTime);
      layer_set_update_proc(s_task_color_layer, task_background_green);
    }
  }
}

//First row of the time. Typically the minutes
static void set_row_one() {

  if((minutes > 58 || minutes <= 3)) {
    text_layer_set_font(s_row_one_layer, s_text_time_bold_font);
  }
  else {
    text_layer_set_font(s_row_one_layer, s_text_time_font);
  }
  if(((minutes > 58 || minutes <= 3) || (minutes > 23 && minutes <= 28))  && strcmp(timetextbufferRowOne, timetextbuffer) !=0) {
    snprintf(timetextbufferRowOne, 8, timetextbuffer);
  }
  else if(((minutes > 3 && minutes <= 8) || (minutes > 53 && minutes <= 58)) && strcmp(timetextbufferRowOne, five) !=0 ) {
     snprintf(timetextbufferRowOne, 5, five);
  }
  else if(((minutes > 8 && minutes <= 13) || (minutes > 48 && minutes <= 53)) && strcmp(timetextbufferRowOne, ten) !=0 ) {
     snprintf(timetextbufferRowOne, 4, ten);
  }
  else if(((minutes > 13 && minutes <= 18) || (minutes > 43 && minutes <= 48)) && strcmp(timetextbufferRowOne, quarter) !=0 ) {
     snprintf(timetextbufferRowOne, 8, quarter);
  }
  else if(((minutes > 18 && minutes <= 23) || (minutes > 33 && minutes <= 43)) && strcmp(timetextbufferRowOne, twenty) !=0 ) {
     snprintf(timetextbufferRowOne, 7, twenty);
  }
  else if(minutes > 28 && minutes <= 33 && strcmp(timetextbufferRowOne, half) !=0 ) {
     snprintf(timetextbufferRowOne, 5, half);
  }
  text_layer_set_text(s_row_one_layer, timetextbufferRowOne);
}
//Second row of the time. Typically how much past the hour
static void set_row_two() {
  if((minutes > 58 || minutes <= 3) && strcmp(timetextbufferRowTwo, o) !=0 ) {
     snprintf(timetextbufferRowTwo, 3, o);
  }
  else if(((minutes > 3 && minutes <= 23) || (minutes > 28 && minutes <= 33)) && strcmp(timetextbufferRowTwo, past) !=0 ) {
     snprintf(timetextbufferRowTwo, 5, past);
  }
  else if((minutes > 23 && minutes <= 28) && strcmp(timetextbufferRowTwo, twenty) !=0 ) {
     snprintf(timetextbufferRowTwo, 7, twenty);
  }
  else if((minutes > 33 && minutes <= 38) && strcmp(timetextbufferRowTwo,  fiveTo) !=0 ) {
     snprintf(timetextbufferRowTwo, 8, fiveTo);
  }
  else if((minutes > 38 && minutes <= 58) && strcmp(timetextbufferRowTwo, to) !=0 ) {
     snprintf(timetextbufferRowTwo, 3, to);
  }
  text_layer_set_text(s_row_two_layer, timetextbufferRowTwo);
}
//Third row of the time. Usually the hour
static void set_row_three() {
  if((minutes > 58 || minutes <= 3) && strcmp(timetextbufferRowThree, clock) !=0 ) {
    text_layer_set_font(s_row_three_layer, s_text_time_font);
    snprintf(timetextbufferRowThree, 6, clock);
  }
  else if((minutes > 23 && minutes <= 28) && strcmp(timetextbufferRowThree, five) !=0 ) {
    text_layer_set_font(s_row_three_layer, s_text_time_font);
    snprintf(timetextbufferRowThree, 5, five);
  }
  else if((minutes >3 && minutes <= 23) || (minutes > 28 && minutes <=58)) {
    text_layer_set_font(s_row_three_layer, s_text_time_bold_font);
    snprintf(timetextbufferRowThree, 8, timetextbuffer);
  }
  text_layer_set_text(s_row_three_layer, timetextbufferRowThree);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  // Create a long-lived buffer

  // Write the current hours and minutes into the buffer
  // Use 24 hour format
  strftime(buffer, sizeof("0000"), "%H%M", tick_time);

 
  //Set all the time based values
  set_hours(tick_time);
  set_minutes(tick_time);
  set_days(tick_time);
  set_hours_string(tick_time);
  
  update_task(tick_time);
  
  if ((hours == 15 && minutes >44)  && days >= 1 && days <= 5) {
    //clear text time
    text_layer_set_text(s_row_one_layer, strEmpty);
    text_layer_set_text(s_row_two_layer, strEmpty);
    text_layer_set_text(s_row_three_layer, strEmpty);
    text_layer_set_text(s_time_layer, buffer);
  }
  if (hours >= 16 && minutes >=15  && days >= 1 && days <= 5) {
    text_layer_set_text(s_time_layer, strEmpty);
    set_row_one();
    set_row_two();
    set_row_three();
  }
  else {
    set_row_one();
    set_row_two();
    set_row_three();
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void main_window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);  
  
  layer_destroy(s_task_color_layer);
  gpath_destroy(s_task_color_path);

  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  text_layer_destroy(s_task_layer);
  text_layer_destroy(s_row_one_layer);
  text_layer_destroy(s_row_two_layer);
  text_layer_destroy(s_row_three_layer);
  text_layer_destroy(s_weather_layer);
}

static void main_window_load(Window *window) {   
  declare_drawing_layer(window);
  set_text_layer_bounds();
  set_fonts();
  apply_fonts_set_alignment();
  add_text_layers_to_window(window);
  text_layer_set_text(s_weather_layer, "28");
}


static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  // Store incoming information
static char temperature_buffer[8];
static char conditions_buffer[32];
static char weather_layer_buffer[32];
// Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
   case KEY_TEMPERATURE:
  snprintf(temperature_buffer, sizeof(temperature_buffer), "%d*", (int)t->value->int32);
  break;
case KEY_CONDITIONS:
  snprintf(conditions_buffer, sizeof(conditions_buffer), "%s", t->value->cstring);
  break;
default:
  APP_LOG(APP_LOG_LEVEL_ERROR, "Key %d not recognized!", (int)t->key);
  break;
    }
    // Look for next item
    t = dict_read_next(iterator);
  }
  // Assemble full string and display
  snprintf(weather_layer_buffer, sizeof(weather_layer_buffer),  temperature_buffer);
  text_layer_set_text(s_weather_layer, weather_layer_buffer);
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

static void deinit() {
  // Destroy Window
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
