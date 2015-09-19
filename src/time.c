#include <pebble.h>
#include "time.h"

int days = 0;
int minutes = 0;
int hours = 0;
int dayOfMonth = 0;

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

char timetextbuffer[] = "000000000";
char timetextbufferRowOne[] = "00000000";
char timetextbufferRowTwo[] = "00000000";
char timetextbufferRowThree[] = "00000000";

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

//First row of the time. Typically the minutes
void get_row_one_text()
{
    if(((minutes > 58 || minutes <= 3) || (minutes > 23 && minutes <= 28)) && strcmp(timetextbufferRowOne, timetextbuffer) !=0)
    {
      snprintf(timetextbufferRowOne, 8, timetextbuffer);
    }
    else if(((minutes > 3 && minutes <= 8) || (minutes > 53 && minutes <= 58)) && strcmp(timetextbufferRowOne, five) !=0 )
    {
      snprintf(timetextbufferRowOne, 5, five);
    }
    else if(((minutes > 8 && minutes <= 13) || (minutes > 48 && minutes <= 53)) && strcmp(timetextbufferRowOne, ten) !=0 )
    {
      snprintf(timetextbufferRowOne, 4, ten);
    }
    else if(((minutes > 13 && minutes <= 18) || (minutes > 43 && minutes <= 48)) && strcmp(timetextbufferRowOne, quarter) !=0 )
    {
      snprintf(timetextbufferRowOne, 8, quarter);
    }
    else if(((minutes > 18 && minutes <= 23) || (minutes > 33 && minutes <= 43)) && strcmp(timetextbufferRowOne, twenty) !=0 )
    {
      snprintf(timetextbufferRowOne, 7, twenty);
    }
    else if(minutes > 28 && minutes <= 33 && strcmp(timetextbufferRowOne, half) !=0 )
    {
      snprintf(timetextbufferRowOne, 5, half);
    }
}
//Second row of the time. Typically how much past the hour
void get_row_two_text()
{
    if((minutes > 58 || minutes <= 3) && strcmp(timetextbufferRowTwo, o) !=0 )
    {
      snprintf(timetextbufferRowTwo, 3, o);
    }
    else if(((minutes > 3 && minutes <= 23) || (minutes > 28 && minutes <= 33)) && strcmp(timetextbufferRowTwo, past) !=0 )
    {
      snprintf(timetextbufferRowTwo, 5, past);
    }
    else if((minutes > 23 && minutes <= 28) && strcmp(timetextbufferRowTwo, twenty) !=0 )
    {
      snprintf(timetextbufferRowTwo, 7, twenty);
    }
    else if((minutes > 33 && minutes <= 38) && strcmp(timetextbufferRowTwo, fiveTo) !=0 )
    {
      snprintf(timetextbufferRowTwo, 8, fiveTo);
    }
    else if((minutes > 38 && minutes <= 58) && strcmp(timetextbufferRowTwo, to) !=0 )
    {
      snprintf(timetextbufferRowTwo, 3, to);
    }
}
//Third row of the time. Usually the hour
void get_row_three_text()
{
    if((minutes > 58 || minutes <= 3) && strcmp(timetextbufferRowThree, clock) !=0 )
    {
      snprintf(timetextbufferRowThree, 6, clock);
    }
    else if((minutes > 23 && minutes <= 28) && strcmp(timetextbufferRowThree, five) !=0 )
    {
      snprintf(timetextbufferRowThree, 5, five);
    }
    else if((minutes >3 && minutes <= 23) || (minutes > 28 && minutes <=58))
    {
      snprintf(timetextbufferRowThree, 8, timetextbuffer);
    }
}