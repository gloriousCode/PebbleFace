#pragma once
int days;
int minutes;
int hours;
int dayOfMonth;

const char * clock;
const char * o;
const char * past;
const char * fivePast;
const char * to;
const char * fiveTo;

const char * quarter;
const char * twenty;
const char * half;

const char * one;
const char * two;
const char * three;
const char * four;
const char * five;
const char * six;
const char * seven;
const char * eight;
const char * nine;
const char * ten;
const char * eleven;
const char * twelve;

char timetextbuffer[9];
char timetextbufferRowOne[8];
char timetextbufferRowTwo[8];
char timetextbufferRowThree[8];

//Important mini methods to get the int values of time
void set_minutes() ;
void set_hours();
void set_days();
void set_day_of_month();

void get_row_one_text();
void get_row_two_text();
void get_row_three_text();
void get_hours_string();