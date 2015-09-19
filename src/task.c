#include <pebble.h>
#include "task.h"
#include "time.h"
#include "system.h"

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
char * workTime = "WORK";
char * prepTime = "PREP";
char * gymTime = "GYM";
char * codeTime = "CODE";
char * studyTime = "STUDY";
char * readTime = "READ";
char * travelTime = "MOVE";
char * freeTime = "FREE";
char * meditate = "MUSE";
char * water = "H20";
char * strEmpty = "";

GColor taskColour;

char currentTask[] = "00000";
char task_text_row_one[] = "00000000000000000";
char task_text_row_two[] = "00000000000000000";
char task_text_row_three[] = "00000000000000000";
char task_text_row_four[] = "00000000000000000";
  
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

void setTasksToFalse()
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

void clear_travel_row_text() {
  snprintf(task_text_row_one, 1, strEmpty);
  snprintf(task_text_row_two, 1, strEmpty);
  snprintf(task_text_row_three, 1, strEmpty);
  snprintf(task_text_row_four, 1, strEmpty);
}

void change_task(GColor color) {
  clear_travel_row_text();
  taskColour = color;
  vibes_double_pulse();
  setTasksToFalse();
}
 void set_water_clock() {
  snprintf(timetextbufferRowOne, 5, "drink");
  snprintf(timetextbufferRowTwo, 5, "some");
  snprintf(timetextbufferRowThree, 5, "water");
}

void set_morning_train_row_text()
{
  snprintf(task_text_row_one, 17,  "   BER  GOS  OUR");
  snprintf(task_text_row_two, 17,  "   627   615  553");
  snprintf(task_text_row_three, 17,"   654   624  614");
  snprintf(task_text_row_four, 17, "   657   635  618");

}

void set_afternoon_travel_text()
{
  snprintf(task_text_row_one, 17,  "  M20  BER   GOS");
  snprintf(task_text_row_two, 17,  "  355  415C  415C");
  snprintf(task_text_row_three, 17,"  407  432R  432R");
  snprintf(task_text_row_four, 17, "  417  445C  445C");
}

//Perform certain functions depending on what time it is
void update_task(struct tm *tick_time)
{
  if(is_water_time()) {
    if(!currentlyWaterTime) {
      change_task(GColorLiberty);
      set_water_clock();
      currentlyWaterTime = true;
      snprintf(currentTask, 5, water);
    }
  }
  else if ((is_morning_prep_time() || is_afternoon_prep_time()))
    {
      if(!currentlyPrepTime) {
        change_task(GColorPurple);
        currentlyPrepTime = true;
        snprintf(currentTask, 5, prepTime);
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
        snprintf(currentTask, 5, readTime);
      }
    }
    else if (is_work_time())    
    {
      if(!currentlyWorkTime) {
        change_task(GColorBlueMoon);
        currentlyWorkTime = true;
        snprintf(currentTask, 5, workTime);
      }
    }
    else if (is_travel_time())    
    {
      if(!currentlyTravelTime) {
        change_task(GColorOrange);
        currentlyTravelTime = true;
        snprintf(currentTask, 5, travelTime);
        if(hours == 6) {
          set_morning_train_row_text();
        }
        if(hours == 17) {
          set_afternoon_travel_text();
        }
      }
    }
    else if (is_gym_time())    
    {
      if(!currentlyGymTime) {
        change_task(GColorRed);
        currentlyGymTime = true;
        snprintf(currentTask, 5, gymTime);
      }
    }
    else if(is_meditation_time())    
    {
      if(!currentlyMeditating) {
        change_task(GColorMelon);
        currentlyMeditating = true;
        snprintf(currentTask, 5, meditate);
      }
    }
    else if(!currentlyFreeTime)    
    {
      change_task(GColorIslamicGreen);
      currentlyFreeTime = true;
      snprintf(currentTask, 5, freeTime);
    }
}

void task_background_color(Layer *layer, GContext *ctx)
{
    // Set the color using RGB values
    graphics_context_set_fill_color(ctx, taskColour);
    // Draw the filled shape in above color
    gpath_draw_filled(ctx, s_task_color_path);
}