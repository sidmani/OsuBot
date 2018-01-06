#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <mach/mach_time.h>
#include <pthread.h>
#include "util.h"
#include "slider.h"
#include "mouse.h"
#include <ApplicationServices/ApplicationServices.h> /* ApplicationServices.framework needed */

#ifndef osu_test_definitions_h
#define osu_test_definitions_h
extern int offset;

typedef struct
{
    float x;
    float y;
} point;
typedef struct
{
    double time;
    int spinner_end_time;
    int type; // 0=spinner, 1=hit circle, 2=slider
    point p;
    char slider_type;
    point slider_points[30];
    int num_of_points;
    int slider_length; //pixel length
    int slider_repeat;
    
    //add slider definitions here
} event;
typedef struct
{
    int time;
    float mpb; //beat length
    float meter;
} timing_event;
extern float sliderMultiplier;
extern int currEvent;
extern int currTiming;
extern event events[8000];
extern timing_event timing_events[500];
extern double start;
extern bool double_time;
extern bool hard_rock;
#endif
