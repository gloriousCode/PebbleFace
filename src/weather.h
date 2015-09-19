#pragma once
#include <pebble.h>
  
uint32_t iconToLoad;

int weatherConditionId;

int * conditionId;

void get_weather_icon();
void set_weather_icon();
void update_weather();