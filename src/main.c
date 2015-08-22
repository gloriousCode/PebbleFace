#include <pebble.h>
  
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
static GPathInfo s_task_color_path_info = {
  .num_points = 4,
  .points = (GPoint[]) { {00, 00},  {144, 00}, {144, 34}, {00, 34} }
};

//Cool sys variables
#define KEY_TEMPERATURE 0
#define KEY_CONDITIONS 1
  
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
const char * travelTime = "MOVE";
const char * freeTime = "FREE";
const char * meditate = "MUSE";

const char * strEmpty = "";

GColor taskColour;

//Weather
GDrawCommandImage* Weather_currentWeatherIcon;
//Calendar
GDrawCommandImage* calendar_background;

int days = 0;
int minutes = 0;
int hours = 0;
int dayOfMonth = 0;

//All the long lived buffers
static char buffer[] = "0000";
static char dayOfMonthTextBuffer[] = "0000";
static char timetextbuffer[] = "000000000";
static char timetextbufferRowOne[] = "00000000";
static char timetextbufferRowTwo[] = "00000000";
static char timetextbufferRowThree[] = "00000000";

//Method to draw weather icon
static void set_weather_icon(Layer *layer, GContext *ctx) {
  uint32_t iconToLoad;
  int something = (int)conditionId;
  switch(something) {
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
    default:  iconToLoad = RESOURCE_ID_WEATHER_CLEAR; break;
  }
    GDrawCommandImage* oldImage = Weather_currentWeatherIcon;
    Weather_currentWeatherIcon = gdraw_command_image_create_with_resource(iconToLoad);
    gdraw_command_image_destroy(oldImage);
  //draw it once you go tthe layer
    gdraw_command_image_draw(ctx, Weather_currentWeatherIcon, GPoint(85, 6));
}

static void render_calendar_background(Layer *layer, GContext *ctx) {
    calendar_background = gdraw_command_image_create_with_resource(RESOURCE_ID_CALENDAR_BACKGROUND);
    gdraw_command_image_draw(ctx, calendar_background, GPoint(114, 4));
}

//Method to draw layer
static void task_background_color(Layer *layer, GContext *ctx) {
  // Set the color using RGB values
  graphics_context_set_fill_color(ctx, taskColour);
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



//Important mini methods to get the int values of time
static void set_minutes(struct tm *tick_time) {
  minutes = tick_time->tm_min;
}

static void set_hours(struct tm *tick_time) {
  hours = tick_time->tm_hour;
}

static void set_days(struct tm *tick_time) {
  days = tick_time->tm_wday;
}

static void set_day_of_month(struct tm *tick_time) {
  dayOfMonth = tick_time->tm_mday;
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
  s_weather_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_day_of_month_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_travel_header_font = fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD);
  s_travel_row_font = fonts_get_system_font(FONT_KEY_GOTHIC_24);
}

static void set_text_layer_bounds() {
  // Create micro time TextLayer
  s_time_layer = text_layer_create(GRect(5, 34, 139, 50));
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
  s_weather_layer = text_layer_create(GRect(65, 2, 25, 25));
  text_layer_set_background_color(s_weather_layer, GColorClear);
  text_layer_set_text_color(s_weather_layer, GColorWhite);
  //Create day of month layer
  s_day_of_month_layer = text_layer_create(GRect(114, 2, 25, 25));
  text_layer_set_background_color(s_day_of_month_layer, GColorClear);
  text_layer_set_text_color(s_day_of_month_layer, GColorBlack);
  //Create travel header row layer
  s_travel_row_one_layer = text_layer_create(GRect(5, 70, 139, 50));
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



static void apply_fonts_set_alignment()  {
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
  add_small_time_layer_to_window(window);
  add_text_time_layers_to_window(window);
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_weather_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_day_of_month_layer));
  
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_one_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_two_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_three_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_travel_row_four_layer));
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
  currentlyMeditating = false;
}

static void set_morning_travel_row_text() {
  text_layer_set_text(s_travel_row_one_layer,   "BER    GOS    OUR");
  text_layer_set_text(s_travel_row_two_layer,   "627    615    553");
  text_layer_set_text(s_travel_row_three_layer, "654    624    614");
  text_layer_set_text(s_travel_row_four_layer,  "657    635    618");
}

static void set_travel_row_text() {
  text_layer_set_text(s_travel_row_one_layer,   "M20    BER    GOS");
  text_layer_set_text(s_travel_row_two_layer,   "355    415C   415C");
  text_layer_set_text(s_travel_row_three_layer, "407    432R   432R");
  text_layer_set_text(s_travel_row_four_layer,  "417    445C   445C");
}

static void clear_travel_row_text() {
  text_layer_set_text(s_travel_row_one_layer, strEmpty);
  text_layer_set_text(s_travel_row_two_layer, strEmpty);
  text_layer_set_text(s_travel_row_three_layer, strEmpty);
  text_layer_set_text(s_travel_row_four_layer, strEmpty);
}

//Perform certain functions depending on what time it is
static void update_task(struct tm *tick_time) {
  // Morning prep time
  if (((hours >= 5 && minutes >55) || (hours <= 6 && minutes <= 55)) && days >= 1 && days <= 5) {
    if(!currentlyPrepTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyPrepTime = true;
      
      text_layer_set_text(s_task_layer, prepTime);
      taskColour = GColorPurple;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      set_morning_travel_row_text();
    }
  }
  // Morning reading time
  else if ((hours <= 8 && hours >= 7) && (days >= 1 && days <= 5)) {
    if(!currentlyReadTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyReadTime = true;
      
      text_layer_set_text(s_task_layer, readTime);
      taskColour = GColorDukeBlue;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      clear_travel_row_text();
    }
  }
  // Afternoon prep time
  else if ((hours >= 15 && hours <= 16) && minutes >44 && days >= 1 && days <= 5) {
    if(!currentlyPrepTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyPrepTime = true;
      
      text_layer_set_text(s_task_layer, prepTime);
      taskColour = GColorPurple;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      set_travel_row_text();
    }
  } 
  // Work time
  else if ((hours >= 8 && hours <= 15) && days >= 1 && days <= 5)
  {
    if(!currentlyWorkTime) {
      text_layer_set_text(s_time_layer, strEmpty);
      vibes_double_pulse();
      setTasksToFalse();
      currentlyWorkTime = true;
      
      text_layer_set_text(s_task_layer, workTime);
      taskColour = GColorBlueMoon;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      clear_travel_row_text();
    }
  } 
  // Travel time
  else if (hours == 16 && minutes <=59 && days >= 1 && days <= 5) 
  {
    if(!currentlyTravelTime) {
      vibes_double_pulse();
      setTasksToFalse();
      currentlyTravelTime = true;

      text_layer_set_text(s_task_layer, travelTime);
      taskColour = GColorOrange;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      set_travel_row_text();
    }
  } 
  // Gym time
  else if ((days == 0 && (hours >= 10 && hours < 15)) || (hours >=17  && minutes <=59 && hours <=19 && (days == 1 || days == 3 || days == 4))) 
  {
    if(!currentlyGymTime) {
      text_layer_set_text(s_time_layer, strEmpty);
      vibes_double_pulse();
      setTasksToFalse();
      currentlyGymTime = true;
      
      text_layer_set_text(s_task_layer, gymTime);
      taskColour = GColorRed;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      clear_travel_row_text();
    }
  }
  // Meditation time
  else if(hours == 20 && (minutes >= 29 || minutes <= 49))
    {
     if(!currentlyMeditating) {
      text_layer_set_text(s_time_layer, strEmpty);
      vibes_double_pulse();
      setTasksToFalse();
      currentlyMeditating = true;
      
      text_layer_set_text(s_task_layer, meditate);
      taskColour = GColorMelon;
      layer_set_update_proc(s_task_color_layer, task_background_color);
       clear_travel_row_text();
    }
  }
  //Freeeeee time
  else 
  {
    if(!currentlyFreeTime) {
      text_layer_set_text(s_time_layer, strEmpty);
      vibes_double_pulse();
      setTasksToFalse();
      currentlyFreeTime = true;
      
      text_layer_set_text(s_task_layer,freeTime);
      taskColour = GColorIslamicGreen;
      layer_set_update_proc(s_task_color_layer, task_background_color);
      clear_travel_row_text();
    }
  }
}

//First row of the time. Typically the minutes
static void set_row_one() {

  if((minutes > 58 || minutes <= 3)|| (minutes > 23 && minutes <= 28)) {
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
  //Set all the time based values
  set_hours(tick_time);
  set_minutes(tick_time);
  set_hours_string(tick_time);
  set_days(tick_time);
  set_day_of_month(tick_time);
  snprintf(dayOfMonthTextBuffer, sizeof(dayOfMonthTextBuffer), "%d", dayOfMonth);
  text_layer_set_text(s_day_of_month_layer, dayOfMonthTextBuffer);
  update_task(tick_time);
  if (((hours >= 15 && hours <= 16) && minutes >44 && days >= 1 && days <= 5) || (hours == 16 && minutes <=59 && days >= 1 && days <= 5)  ) {
    //Clear text time and make it smaller for data
    text_layer_set_text(s_row_one_layer, strEmpty);
    text_layer_set_text(s_row_two_layer, strEmpty);
    text_layer_set_text(s_row_three_layer, strEmpty);
    strftime(buffer, sizeof("0000"), "%H%M", tick_time);
    text_layer_set_text(s_time_layer, buffer);
  }
  else {
    set_row_one();
    set_row_two();
    set_row_three();
  }
}

static void update_weather() {
  // Get weather update every 30 minutes
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  if(tick_time->tm_min % 30 == 0) {
    // Begin dictionary
    DictionaryIterator *iter;
    app_message_outbox_begin(&iter);
    // Add a key-value pair
    dict_write_uint8(iter, 0, 0);
    // Send the message!
    app_message_outbox_send();
  }
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
  update_weather();
}

static void main_window_unload(Window *window) {
  fonts_unload_custom_font(s_time_font);  
  
  layer_destroy(s_task_color_layer);
  gpath_destroy(s_task_color_path);
  layer_destroy(s_weather_icon_layer);
  layer_destroy(s_calendar_background_layer);

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
}

static void main_window_load(Window *window) {   
  declare_drawing_layer(window);
  set_text_layer_bounds();
  set_fonts();
  apply_fonts_set_alignment();
  add_text_layers_to_window(window);
  
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
APP_LOG(APP_LOG_LEVEL_ERROR, context);
static char temperature_buffer[8];
static char conditions_buffer[32];
static char condition_layer_buffer[32];
static char weather_layer_buffer[32];
// Read first item
  Tuple *t = dict_read_first(iterator);

  // For all items
  while(t != NULL) {
    // Which key was received?
    switch(t->key) {
   case KEY_TEMPERATURE:
  snprintf(temperature_buffer, sizeof(temperature_buffer), "%d", (int)t->value->int32);
  break;
case KEY_CONDITIONS:
  conditionId = (int *)t->value->int32;
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
  layer_set_update_proc(s_weather_icon_layer, set_weather_icon);
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
