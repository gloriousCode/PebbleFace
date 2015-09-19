#include <pebble.h>
#include "time.h"

int days = 0;
int minutes = 0;
int hours = 0;
int dayOfMonth = 0;

//Important mini methods to get the int values of time
void set_minutes(struct tm *tick_time) 
{
  minutes = tick_time->tm_min;
}

void set_hours(struct tm *tick_time) 
{
  hours = tick_time->tm_hour;
}

void set_days(struct tm *tick_time)
{
    days = tick_time->tm_wday;
}

void set_day_of_month(struct tm *tick_time)
{
    dayOfMonth = tick_time->tm_mday;
}