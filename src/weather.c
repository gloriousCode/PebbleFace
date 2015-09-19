#include <pebble.h>
#include "weather.h"
#include "system.h"
#include "javascripthandler.h"

uint32_t iconToLoad;
int weatherConditionId;
int * conditionId;
  
void get_weather_icon() {
   weatherConditionId = (int)conditionId;
    switch(weatherConditionId)
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
}


void set_weather_icon(Layer *layer, GContext *ctx)
{
  get_weather_icon();
  GDrawCommandImage* oldImage = Weather_currentWeatherIcon;
  Weather_currentWeatherIcon = gdraw_command_image_create_with_resource(iconToLoad);
  gdraw_command_image_destroy(oldImage);
  // Draw it once you go the layer
  gdraw_command_image_draw(ctx, Weather_currentWeatherIcon, GPoint(62, 6));
}

void update_weather()
{
    // Get weather update every 30 minutes
    time_t temp = time(NULL);
    struct tm *tick_time = localtime(&temp);
    if(tick_time->tm_min % 30 == 0)
    {
     trigger_js();
    }
}