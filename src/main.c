#include <pebble.h>
  
static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_task_layer;
static GFont s_time_font;
static GFont s_task_font;
static Layer *s_path_layer;
static GPath *s_path;
static GPathInfo PATH_INFO = {
  .num_points = 4,
  .points = (GPoint[]) { {00, 60},  {220, 60}, {220, 100}, {00, 100} }
};

//Method to draw layer
static void layer_update_proc(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, GColorFromRGB(255, 0, 0));

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
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FONT_IMAGINE_40));
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
  
  set_fonts();
  
    // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_font(s_task_layer, s_task_font);
  
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  text_layer_set_text_alignment(s_task_layer, GTextAlignmentCenter);
  
    // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_task_layer));
}

static void main_window_load(Window *window) {  
  declare_drawing_layer(window);  
  declare_text_layers(window);
}

//Perform certain functions depending on what time it is
static void update_task(struct tm *tick_time) {
    static char hoursStr[] = "00";
    strftime(hoursStr, sizeof("00"), "%H", tick_time);
    int hours = atoi(hoursStr);
    //strftime(dayChecker, sizeof("0"), "%w", tick_time);
  if (hours >= 8 && hours <= 16)
  {
    text_layer_set_text(s_task_layer, "WORK TIME");
  } else if (hours >= 16 && hours <=17) {
    text_layer_set_text(s_task_layer, "TRAVEL TIME");
  } else if (hours >=17 && hours <=18){
    text_layer_set_text(s_task_layer, "GYM TIME");
  }
  else{
    text_layer_set_text(s_task_layer, "FREE TIME");
  }
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

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
  text_layer_set_text(s_time_layer, buffer);
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
