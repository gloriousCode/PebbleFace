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

static Layer *s_task_color_layer;
static GPath *s_task_color_path;
static GPathInfo s_task_color_path_info = {
  .num_points = 4,
  .points = (GPoint[]) { {00, 00},  {144, 00}, {144, 30}, {00, 30} }
};

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

const char * workTime = "WORK TIME";
const char * prepTime = "PREP TIME";
const char * gymTime = "GYM TIME";
const char * codeTime = "CODE TIME";
const char * studyTime = "STUDY TIME";
const char * readTime = "READ TIME";
const char * travelTime = "TRAVEL TIME";
const char * freeTime = "FREE TIME";

const char * strEmpty = "";

//All the long lived buffers
static char buffer[] = "0000";
static char minuteStr[] = "00";
static char hoursStr[] = "00";
static char dayStr[] = "0";
static char timetextbuffer[] = "0000000000";

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
static int get_minutes(struct tm *tick_time) {
  strftime(minuteStr, sizeof("00"), "%M", tick_time);
  int minutes = atoi(minuteStr); 
  return minutes;
}

static int get_hours(struct tm *tick_time) {
  strftime(hoursStr, sizeof("00"), "%H", tick_time);
  int hours = atoi(hoursStr);
  return hours;
}

static int get_days(struct tm *tick_time) {
  strftime(dayStr, sizeof("0"), "%w", tick_time);
  int day = atoi(dayStr);
  return day;
}

static char* get_hour_str(struct tm *tick_time, int minutes) 
{
  strftime(hoursStr, sizeof("00"), "%I", tick_time);
  int hours = atoi(hoursStr);
  if(minutes >= 33) {
    hours++;
  }
  if(hours == 1 || hours == 13){
    snprintf(hoursStr, 7, one);
  }
    if(hours == 2 || hours == 14){
    snprintf(hoursStr, 7, two);
  }
    if(hours == 3 || hours == 15){
    snprintf(hoursStr, 7, three);
  }
    if(hours == 4 || hours == 16){
    snprintf(hoursStr, 7, four);
  }
    if(hours == 5 || hours == 17){
    snprintf(hoursStr, 7, five);
  }
    if(hours == 6 || hours == 18){
    snprintf(hoursStr, 7, six);
  }
    if(hours == 7 || hours == 19){
    snprintf(hoursStr, 7, seven);
  }
    if(hours == 8 || hours == 20){
    snprintf(hoursStr, 7, eight);
  }
    if(hours == 9 || hours == 21){
    snprintf(hoursStr, 7, nine);
  }
    if(hours == 10 || hours ==22){
    snprintf(hoursStr, 7, ten);
  }
    if(hours == 11 || hours == 23){
    snprintf(hoursStr, 7, eleven);
  }
    if(hours == 12 || hours == 24){
    snprintf(hoursStr, 7, twelve);
  }  
  return hoursStr;
}


static void set_fonts() {
    // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_36));
  s_text_time_font =  fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT);
  s_task_font =  fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
}

static void set_text_layer_bounds() {
  // Create micro time TextLayer
  s_time_layer = text_layer_create(GRect(5, 52, 139, 50));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorWhite);
  // Create task TextLayer
  s_task_layer = text_layer_create(GRect(40, 0, 100, 24));
  text_layer_set_background_color(s_task_layer, GColorClear);
  text_layer_set_text_color(s_task_layer, GColorWhite);
  //Create fuzzy time row one layer
  s_row_one_layer = text_layer_create(GRect(5, 30, 200, 100));
  text_layer_set_background_color(s_row_one_layer, GColorClear);
  text_layer_set_text_color(s_row_one_layer, GColorWhite);
  //Create fuzzy time row two layer
  s_row_two_layer = text_layer_create(GRect(5, 70, 200, 100));
  text_layer_set_background_color(s_row_two_layer, GColorClear);
  text_layer_set_text_color(s_row_two_layer, GColorWhite);
  //Create fuzzy time row three layer
  s_row_three_layer = text_layer_create(GRect(5, 110, 200, 100));
  text_layer_set_background_color(s_row_three_layer, GColorClear);
  text_layer_set_text_color(s_row_three_layer, GColorWhite);
}

static void apply_fonts_set_alignment()  {
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_task_layer, s_task_font);
  
  text_layer_set_font(s_row_one_layer, s_text_time_font);
  text_layer_set_font(s_row_two_layer, s_text_time_font);
  text_layer_set_font(s_row_three_layer, s_text_time_font);
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_task_layer, GTextAlignmentLeft);
  
  text_layer_set_text_alignment(s_row_one_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_row_two_layer, GTextAlignmentLeft);
  text_layer_set_text_alignment(s_row_three_layer, GTextAlignmentLeft);
}

static void add_small_time_layer_to_window(Window *window){
   layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void add_text_time_layers_to_window(Window *window) {
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_one_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_two_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_three_layer));
}

static void add_text_layers_to_window(Window *window) {
  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_task_layer));
  add_text_time_layers_to_window(window);
}

static void declare_text_layers(Window *window) {
  set_text_layer_bounds();
  set_fonts();
  apply_fonts_set_alignment();
  add_text_layers_to_window(window);
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

  if (hours >= 8 && (hours <= 15 && minutes <=44)  && day >= 1 && day <= 5)
  {
    if(!currentlyWorkTime) {   
      vibes_double_pulse();
      currentlyWorkTime = true;
      currentlyPrepTime = false;
      currentlyGymTime = false;
      currentlyCodeTime = false;
      currentlyStudyTime = false;
      currentlyReadTime = false;
      currentlyTravelTime = false;
      currentlyFreeTime = false;
      
      text_layer_set_text(s_task_layer, workTime);
      layer_set_update_proc(s_task_color_layer, task_background_blue);
    }
  } 
  else if ((hours >= 15 && minutes >44)  && day >= 1 && day <= 5) {
    if(!currentlyPrepTime) {
      vibes_double_pulse();
      currentlyWorkTime = false;
      currentlyPrepTime = true;
      currentlyGymTime = false;
      currentlyCodeTime = false;
      currentlyStudyTime = false;
      currentlyReadTime = false;
      currentlyTravelTime = false;
      currentlyFreeTime = false;
      
      //render small clock to make room for js data
      text_layer_set_text(s_task_layer, prepTime);
      layer_set_update_proc(s_task_color_layer, task_background_purple);
    }
  }
  else if ((hours == 16 && minutes ==14)  && day >= 1 && day <= 5) {
    //render normal time again
  }
  else if ((hours == 16 && minutes <=59) && day >= 1 && day <= 5) 
  {
    if(!currentlyTravelTime) {
      vibes_double_pulse();
      currentlyWorkTime = false;
      currentlyPrepTime = false;
      currentlyGymTime = false;
      currentlyCodeTime = false;
      currentlyStudyTime = false;
      currentlyReadTime = false;
      currentlyTravelTime = true;
      currentlyFreeTime = false;
      
      text_layer_set_text(s_task_layer, travelTime);
      layer_set_update_proc(s_task_color_layer, task_background_orange);
    }
  } 
  else if ((day == 0 && hours >= 10 && hours < 15) || ((hours >=17  && minutes <=59) && (day == 1 || day == 3 || day == 4))) 
  {
    if(!currentlyGymTime) {
      vibes_double_pulse();
      currentlyWorkTime = false;
      currentlyPrepTime = false;
      currentlyGymTime = true;
      currentlyCodeTime = false;
      currentlyStudyTime = false;
      currentlyReadTime = false;
      currentlyTravelTime = false;
      currentlyFreeTime = false;
      
      text_layer_set_text(s_task_layer, gymTime);
      layer_set_update_proc(s_task_color_layer, task_background_red);
    }
  }
  else 
  {
    if(!currentlyFreeTime) {
      vibes_double_pulse();
      currentlyWorkTime = false;
      currentlyPrepTime = false;
      currentlyGymTime = false;
      currentlyCodeTime = false;
      currentlyStudyTime = false;
      currentlyReadTime = false;
      currentlyTravelTime = false;
      currentlyFreeTime = true;
      
      text_layer_set_text(s_task_layer, freeTime);
      layer_set_update_proc(s_task_color_layer, task_background_green);
    }
  }
}

//First row of the time. Typically the minutes
static void set_row_one(struct tm *tick_time) {
  int minutes = get_minutes(tick_time);
  char * hoursText = get_hour_str(tick_time, minutes);
  const char * current = text_layer_get_text(s_row_one_layer);
  if((minutes > 58 || minutes <= 3) || (minutes > 23 && minutes <= 28)) {
    text_layer_set_font(s_row_one_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  }
  else {
    text_layer_set_font(s_row_one_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  }
  if(((minutes > 58 || minutes <= 3) || (minutes > 23 && minutes <= 28))  && current != hoursText) {
    
    text_layer_set_text(s_row_one_layer, hoursText);
  }

  if(((minutes > 3 && minutes <= 8) || (minutes > 53 && minutes <= 58)) && current != five) {
    text_layer_set_text(s_row_one_layer, five);
    return;
  }

  if(((minutes > 8 && minutes <= 13) || (minutes > 48 && minutes <= 53)) && current != ten) {
    text_layer_set_text(s_row_one_layer, ten);
    return;
  }

  if(((minutes > 13 && minutes <= 18) || (minutes > 43 && minutes <= 48)) && current != quarter) {
    text_layer_set_text(s_row_one_layer, quarter);
    return;
  }

  if(((minutes > 18 && minutes <= 23) || (minutes > 33 && minutes <= 43)) && current != twenty) {
    text_layer_set_text(s_row_one_layer, twenty);
    return;
  }

  if(minutes > 28 && minutes <= 33 && current != half) {
    text_layer_set_text(s_row_one_layer, half);
    return;
  }
}
//Second row of the time. Typically how much past the hour
static void set_row_two(struct tm *tick_time) {
  int minutes = get_minutes(tick_time);  
  const char * current = text_layer_get_text(s_row_one_layer);

  if((minutes > 58 || minutes <= 3) && current != o) {
    text_layer_set_text(s_row_two_layer, o);
    return;
  }
  if(((minutes > 3 && minutes <= 23) || (minutes > 28 && minutes <= 33)) && current != past) {
    text_layer_set_text(s_row_two_layer,past);
    return;      
  }
  if((minutes > 23 && minutes <= 28) && current != twenty) {
    text_layer_set_text(s_row_two_layer, twenty);
    return;
  }
  if((minutes > 33 && minutes <= 38) && current != fiveTo) {
    text_layer_set_text(s_row_two_layer,fiveTo);
    return;
  }
  if((minutes > 38 && minutes <= 58) && current != to) {
    text_layer_set_text(s_row_two_layer, to);
    return;
  }
}
//Third row of the time. Usually the hour
static void set_row_three(struct tm *tick_time) {
  int minutes = get_minutes(tick_time);
  char * houtsText = get_hour_str(tick_time, minutes);
  const char * current = text_layer_get_text(s_row_one_layer);
  
  if((minutes > 58 || minutes <= 3) && current != clock) {
    text_layer_set_font(s_row_three_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text(s_row_three_layer, clock);
  }
  else if((minutes > 23 && minutes <= 28) && current != five) {
    text_layer_set_font(s_row_three_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
    text_layer_set_text(s_row_three_layer, five);
  }
  else {
    text_layer_set_font(s_row_three_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
     text_layer_set_text(s_row_three_layer, houtsText);
  }
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  // Create a long-lived buffer

  // Write the current hours and minutes into the buffer
  // Use 24 hour format
  strftime(buffer, sizeof("0000"), "%H%M", tick_time);
  update_task(tick_time);
  
  int hours = get_hours(tick_time);
  int minutes = get_minutes(tick_time);
  int day = get_days(tick_time);
  
  if((hours == 15 && minutes ==44)&& day >= 1 && day <= 5) {
    //clear text time
    text_layer_set_text(s_row_one_layer, strEmpty);
    text_layer_set_text(s_row_two_layer, strEmpty);
    text_layer_set_text(s_row_three_layer, strEmpty);
  }
  if (hours == 16 && minutes ==15  && day >= 1 && day <= 5) {
    text_layer_set_text(s_time_layer, strEmpty);
  }
  if (((hours == 15 && minutes >=44) || (hours == 16 && minutes <=14)) && day >= 1 && day <= 5) {
    //render small clock to make room for js data
    text_layer_set_text(s_time_layer, buffer);
  }
  else {
    set_row_one(tick_time);
    set_row_two(tick_time);
    set_row_three(tick_time);
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void destory_small_time_layer() {
  text_layer_destroy(s_time_layer);
}
static void destroy_text_time_layer() {
    // Time rendering
  text_layer_destroy(s_row_one_layer);
  text_layer_destroy(s_row_two_layer);
  text_layer_destroy(s_row_three_layer);
}

static void main_window_unload(Window *window) {
  // Unload GFont
  fonts_unload_custom_font(s_time_font);  
  fonts_unload_custom_font(s_task_font);  
  fonts_unload_custom_font(s_text_time_font);  
  
  gpath_destroy(s_task_color_path);
  destory_small_time_layer();
  // Destroy TextLayer
  text_layer_destroy(s_task_layer);
  destroy_text_time_layer();
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
