#pragma once
#include <pebble.h>

  //Bools
bool currentlyWorkTime;
bool currentlyPrepTime ;
bool currentlyGymTime;
bool currentlyCodeTime;
bool currentlyStudyTime;
bool currentlyReadTime;
bool currentlyTravelTime;
bool currentlyFreeTime;
bool currentlyMeditating;
bool currentlyWaterTime;

//Constants
char * workTime;
char * prepTime;
char * gymTime;
char * codeTime;
char * studyTime;
char * readTime;
char * travelTime;
char * freeTime;
char * meditate;
char * water;
char * strEmpty;

GColor taskColour;

char currentTask[5];
char task_text_row_one[17];
char task_text_row_two[17];
char task_text_row_three[17];
char task_text_row_four[17];
  
bool is_morning_prep_time();
bool is_afternoon_prep_time();
bool is_reading_time();
bool is_work_time();
bool is_travel_time();
bool is_gym_time();
bool is_meditation_time();
bool is_water_time();

void setTasksToFalse();

void clear_travel_row_text();
void change_task();
 void set_water_clock();

void set_morning_train_row_text();

void set_afternoon_travel_text();

//Perform certain functions depending on what time it is
void update_task();
void task_background_color();
