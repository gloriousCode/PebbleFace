#include <pebble.h>
#include "task.h"
#include "time.h"

bool is_morning_prep_time() {
  if(days >= 1 && days <= 5) {
    if(hours>=5 && hours <=6) {
      if(hours == 5 && minutes >=55)   {
        return true;
      }
      if(hours == 6 && minutes <= 15) {
        return true;
      }
    }
  }
  return false;
}
bool is_afternoon_prep_time() {
  if(days >= 1 && days <= 5) {
    if(hours==15) {
      if(minutes>=55) {
        return true;
      }
    }
    if(hours < 17 && hours >15) {
      return true;
    }
  }
  return false;
}
bool is_reading_time() {
  if(days >= 1 && days <= 5)
    {
    if(hours == 7) {
      return true;
    }
  }
  return false;
}
bool is_work_time() {
  return ((hours >= 8 && hours <= 16) && days >= 1 && days <= 5);
}
bool is_travel_time() {
  if(days >= 1 && days <= 5)
    {
    if(hours == 6)
      {
      return true;
    }
    if(hours == 17)
      {
      return true;
    }
  }
  return false;
}
bool is_gym_time() {
  if(days == 0 && hours >=10 && hours <15)
    {
      return true;
  }
  if(days == 1 || days == 2|| days ==4 || days ==5)
    {
    if(hours >=17 && hours <= 19)
      {
      return true;
    }
  }
  return false;
}
bool is_meditation_time() {
  if(days >=1 && days <=5)
    {
    if(hours == 20)
      {
      if(minutes >= 30)
        {
        return true;
      }
    }
  }
  return false;
}

bool is_water_time() {
  if(days >=1 && days <=5 && hours >= 9 && hours < 16) {
    if(minutes == 0) {
      return true;
    }
  }
  return false;
}