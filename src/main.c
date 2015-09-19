#include <pebble.h>
#include "task.h"
#include "time.h"

static Window *s_main_window;

static TextLayer *s_time_layer;
static TextLayer *s_task_layer;
static TextLayer *s_row_one_layer;
static TextLayer *s_row_two_layer;
static TextLayer *s_row_three_layer;
static TextLayer *s_weather_layer;
static TextLayer *s_day_of_month_layer;
static TextLayer *s_travel_row_one_layer;
static TextLayer *s_travel_row_two_layer;
static TextLayer *s_travel_row_three_layer;
static TextLayer *s_travel_row_four_layer;

static GFont s_time_font;
static GFont s_task_font;
static GFont s_text_time_font;
static GFont s_text_time_bold_font;
static GFont s_weather_font;
static GFont s_day_of_month_font;
static GFont s_travel_header_font;
static GFont s_travel_row_font;

static Layer *s_task_color_layer;
static Layer *s_weather_icon_layer;
static Layer *s_calendar_background_layer;
static GPath *s_task_color_path;
static GPathInfo s_task_color_path_info =
{
    .num_points = 4,
    .points = (GPoint[])
    {
      {00, 00}, {144, 00}, {144, 34}, {00, 34} 
    }
};

//Cool sys variables
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
#define KEY_TRAFFIC_TITLE 2
#define KEY_TRAFFIC_WARNING_ONE 3
#define KEY_TRAFFIC_WARNING_TWO 4
#define KEY_TRAFFIC_DIRECTION 5

int * conditionId = 0;
//Bools
bool currentlyWorkTime = false;
bool currentlyPrepTime = false;
bool currentlyGymTime = false;
bool currentlyCodeTime = false;
bool currentlyStudyTime = false;
bool currentlyReadTime = false;
bool currentlyTravelTime = false;
bool currentlyFreeTime = false;
bool currentlyMeditating = false;
bool currentlyWaterTime = false;


//Constants


const char * workTime = "WORK";
const char * prepTime = "PREP";
const char * gymTime = "GYM";
const char * codeTime = "CODE";
const char * studyTime = "STUDY";
const char * readTime = "READ";
const char * travelTime = "MOVE";
const char * freeTime = "FREE";
const char * meditate = "MUSE";
const char * water = "H20";

const char * strEmpty = "";

GColor taskColour;

//Weather
GDrawCommandImage* Weather_currentWeatherIcon;
//Calendar
GDrawCommandImage* calendar_background;

//All the long lived buffers
static char buffer[] = "0000";
static char dayOfMonthTextBuffer[] = "0000";


//Method to draw weather icon
static void set_weather_icon(Layer *layer, GContext *ctx)
{
    uint32_t iconToLoad;
    int something = (int)conditionId;
    switch(something)
    {
        //THUNDER
        case 200:
        case 201:
        case 202:
        case 210:
        case 211:
        case 212:
        case 221:
        case 230:
        case 231:
        case 232: iconToLoad = RESOURCE_ID_WEATHER_THUNDERSTORM; break;
        //DRIZZLE
        case 300:
        case 301:
        case 302:
        case 310:
        case 311:
        case 312:
        case 313:
        case 314:
        case 321: iconToLoad = RESOURCE_ID_WEATHER_DRIZZLE; break;
        //RAIN
        case 500:
        case 501:
        case 502:
        case 503:
        case 504:
        case 511:
        case 520:
        case 521:
        case 522:
        case 531: iconToLoad = RESOURCE_ID_WEATHER_RAIN; break;
        //FOG
        case 700:
        case 711:
        case 721:
        case 731:
        case 741:
        case 751:
        case 761:
        case 771:
        case 781: iconToLoad = hours >= 18? RESOURCE_ID_WEATHER_PARTLY_CLOUDY_NIGHT :RESOURCE_ID_WEATHER_PARTLY_CLOUDY; break;
        //CLOUDY WITH A CHANCE OF BALLS
        case 800:
        case 801:
        case 802: iconToLoad = hours >= 18? RESOURCE_ID_WEATHER_CLEAR_NIGHT : RESOURCE_ID_WEATHER_CLEAR; break;
        case 803:
        case 804: iconToLoad = RESOURCE_ID_WEATHER_CLOUDY; break;
        default: iconToLoad = RESOURCE_ID_WEATHER_CLEAR; break;
    }
    GDrawCommandImage* oldImage = Weather_currentWeatherIcon;
    Weather_currentWeatherIcon = gdraw_command_image_create_with_resource(iconToLoad);
    gdraw_command_image_destroy(oldImage);
    //draw it once you go the layer
    gdraw_command_image_draw(ctx, Weather_currentWeatherIcon, GPoint(62, 6));
}

static void render_calendar_background(Layer *layer, GContext *ctx)
{
    //Kill the old struct!
    GDrawCommandImage* oldImage = calendar_background;
    calendar_background = gdraw_command_image_create_with_resource(RESOURCE_ID_CALENDAR_BACKGROUND);
    gdraw_command_image_destroy(oldImage);
    gdraw_command_image_draw(ctx, calendar_background, GPoint(114, 4));
}

//Method to draw layer
static void task_background_color(Layer *layer, GContext *ctx)
{
    // Set the color using RGB values
    graphics_context_set_fill_color(ctx, taskColour);
    // Draw the filled shape in above color
    gpath_draw_filled(ctx, s_task_color_path);
}

static void declare_drawing_layer(Window *window)
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

static void set_hours_string(struct tm *tick_time)

{
    int timeHours = hours;
    if(minutes > 33)
    {
        timeHours++;
    }
    if(timeHours == 1 || timeHours == 13)
    {
        snprintf(timetextbuffer, 4, one);
    }
    if(timeHours == 2 || timeHours == 14)
    {
        snprintf(timetextbuffer, 4, two);
    }
    if(timeHours == 3 || timeHours == 15)
    {
        snprintf(timetextbuffer, 6, three);
    }
    if(timeHours == 4 || timeHours == 16)
    {
        snprintf(timetextbuffer, 5, four);
    }
    if(timeHours == 5 || timeHours == 17)
    {
        snprintf(timetextbuffer, 5, five);
    }
    if(timeHours == 6 || timeHours == 18)
    {
        snprintf(timetextbuffer, 4, six);
    }
    if(timeHours == 7 || timeHours == 19)
    {
        snprintf(timetextbuffer, 6, seven);
    }
    if(timeHours == 8 || timeHours == 20)
    {
        snprintf(timetextbuffer, 6, eight);
    }
    if(timeHours == 9 || timeHours == 21)
    {
        snprintf(timetextbuffer, 5, nine);
    }
    if(timeHours == 10 || timeHours ==22)
    {
        snprintf(timetextbuffer, 4, ten);
    }
    if(timeHours == 11 || timeHours == 23)
    {
        snprintf(timetextbuffer, 8, eleven);
    }
    if(timeHours == 12 || timeHours == 24)
    {
        snprintf(timetextbuffer, 8, twelve);
    }
}


static void set_fonts()
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

static void set_text_layer_bounds()
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
    s_row_one_layer = text_layer_create(GRect(5, 30, 200, 100));
    text_layer_set_background_color(s_row_one_layer, GColorClear);
    text_layer_set_text_color(s_row_one_layer, GColorWhite);
    //Create fuzzy time row two layer
    s_row_two_layer = text_layer_create(GRect(5, 72, 200, 100));
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



static void apply_fonts_set_alignment()
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

static void add_small_time_layer_to_window(Window *window)
{
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
}

static void add_text_time_layers_to_window(Window *window)
{
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_one_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_two_layer));
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_row_three_layer));
}

static void add_text_layers_to_window(Window *window)
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

static void setTasksToFalse()
{
    currentlyWorkTime = false;
    currentlyPrepTime = false;
    currentlyGymTime = false;
    currentlyCodeTime = false;
    currentlyStudyTime = false;
    currentlyReadTime = false;
    currentlyTravelTime = false;
    currentlyFreeTime = false;
    currentlyMeditating = false;
    currentlyWaterTime = false;
    light_enable_interaction();
}
static void trigger_js() {
          // Begin dictionary
        DictionaryIterator *iter;
        app_message_outbox_begin(&iter);
        // Add a key-value pair
        dict_write_uint8(iter, 0, 0);
        // Send the message!
        app_message_outbox_send();
}

static void update_weather()
{
    // Get weather update every 30 minutes
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    if(tick_time->tm_min % 30 == 0)
    {
      trigger_js();
    }
}

static void render_small_clock() {
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  text_layer_set_text(s_row_one_layer, strEmpty);
  text_layer_set_text(s_row_two_layer, strEmpty);
  text_layer_set_text(s_row_three_layer, strEmpty);
  strftime(buffer, sizeof("0000"), "%H%M", tick_time);
  text_layer_set_text(s_time_layer, buffer);
}

static void set_font_weight() {
  
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

static void render_text_clock() {
  get_row_one_text();
  get_row_two_text();
  get_row_three_text();
  
  text_layer_set_text(s_row_one_layer, timetextbufferRowOne);
  text_layer_set_text(s_row_two_layer, timetextbufferRowTwo);
  text_layer_set_text(s_row_three_layer, timetextbufferRowThree);
  text_layer_set_text(s_time_layer, strEmpty);
  
  set_font_weight();
}

static void set_morning_train_row_text()
{
  text_layer_set_text(s_travel_row_one_layer,    "   BER  GOS  OUR");
  text_layer_set_text(s_travel_row_two_layer,    "   627   615  553");
  text_layer_set_text(s_travel_row_three_layer,  "   654   624  614");
  text_layer_set_text(s_travel_row_four_layer,   "   657   635  618");
}

static void set_afternoon_travel_text()
{
  text_layer_set_text(s_travel_row_one_layer,    "  M20  BER   GOS");
  text_layer_set_text(s_travel_row_two_layer,    "  355  415C  415C");
  text_layer_set_text(s_travel_row_three_layer,  "  407  432R  432R");
  text_layer_set_text(s_travel_row_four_layer,   "  417  445C  445C");
}

static void clear_travel_row_text()
{
  text_layer_set_text(s_travel_row_one_layer, strEmpty);
  text_layer_set_text(s_travel_row_two_layer, strEmpty);
  text_layer_set_text(s_travel_row_three_layer, strEmpty);
  text_layer_set_text(s_travel_row_four_layer, strEmpty);
}

static void change_task(GColor color) {
  taskColour = color;
  vibes_double_pulse();
  setTasksToFalse();
  layer_set_update_proc(s_task_color_layer, task_background_color);
  clear_travel_row_text();
}

static void set_water_clock() {
  text_layer_set_text(s_row_one_layer, "drink");
  text_layer_set_text(s_row_two_layer, "some");
  text_layer_set_text(s_row_three_layer, "water");
}


//Perform certain functions depending on what time it is
static void update_task(struct tm *tick_time)
{
  if(is_water_time()) {
    if(!currentlyWaterTime) {
      change_task(GColorLiberty);
      set_water_clock();
      currentlyWaterTime = true;
      text_layer_set_text(s_task_layer, water);
    }
  }
  else if ((is_morning_prep_time() || is_afternoon_prep_time()))
    {
      if(!currentlyPrepTime) {
        change_task(GColorPurple);
        currentlyPrepTime = true;
        text_layer_set_text(s_task_layer, prepTime);
        trigger_js();
        if(is_morning_prep_time()) {
          set_morning_train_row_text();
        }
        else {
          set_afternoon_travel_text();
        }   
      }
    }
    else if (is_reading_time()) { 
      if(!currentlyReadTime)
      {
        change_task(GColorDukeBlue);
        currentlyReadTime = true;
        text_layer_set_text(s_task_layer, readTime);
      }
    }
    else if (is_work_time())    
    {
      if(!currentlyWorkTime) {
        change_task(GColorBlueMoon);
        currentlyWorkTime = true;
        text_layer_set_text(s_task_layer, workTime);
      }
    }
    else if (is_travel_time())    
    {
      if(!currentlyTravelTime) {
        change_task(GColorOrange);
        currentlyTravelTime = true;
        text_layer_set_text(s_task_layer, travelTime);
        if(hours == 6) {
          set_morning_train_row_text();
        }
        if(hours == 17) {
          set_afternoon_travel_text();
        }
        trigger_js();
      }
    }
    else if (is_gym_time())    
    {
      if(!currentlyGymTime) {
        change_task(GColorRed);
        currentlyGymTime = true;
        text_layer_set_text(s_task_layer, gymTime);
      }
    }
    else if(is_meditation_time())    
    {
      if(!currentlyMeditating) {
        change_task(GColorMelon);
        currentlyMeditating = true;
        text_layer_set_text(s_task_layer, meditate);
      }
    }
    else if(!currentlyFreeTime)    
    {
      change_task(GColorIslamicGreen);
      currentlyFreeTime = true;
      text_layer_set_text(s_task_layer,freeTime);
    }
}


static void update_time()
{
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    //Set all the time based values
    set_hours(tick_time);
    set_minutes(tick_time);
    set_hours_string(tick_time);
    set_days(tick_time);
    set_day_of_month(tick_time);

    snprintf(dayOfMonthTextBuffer, sizeof(dayOfMonthTextBuffer), "%d", dayOfMonth);
    text_layer_set_text(s_day_of_month_layer, dayOfMonthTextBuffer);
    update_task(tick_time);
    if (is_morning_prep_time() || is_afternoon_prep_time() || is_travel_time())
    {
       render_small_clock();
    }
    else if(is_water_time()) {
      set_water_clock();
    }
    else {
      render_text_clock();
    }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
    update_time();
    update_weather();
}

static void main_window_unload(Window *window)
{
    fonts_unload_custom_font(s_time_font);
    //Kill the images
    layer_destroy(s_calendar_background_layer);
    
    gdraw_command_image_destroy(Weather_currentWeatherIcon);
    gdraw_command_image_destroy(calendar_background);
    
    // Destroy TextLayer
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


static void inbox_dropped_callback(AppMessageResult reason, void *context)
{
    APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context)
{
    APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context)
{
    APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context)
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