#include "definitions.h"


event events[8000]; //array that holds events
timing_event timing_events[500];
int numEvents = 0;
int numTimingEvents = 0;
int currEvent;
int currTiming;
double start = 0;
volatile int keyCode;
volatile bool keyPressed = false;
bool double_time;
//method declarations
void parseIn();
void run_hitcircle();
void run_spinner();
void *eventHandler(void *arg);
void begin();
void sliderParse(char *ptr, int i);
void *keyHandlerFunc(void *arg);
void updateTiming();
CGEventRef on_tap(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon);
#define ARRAYSIZE(x)  (sizeof(x)/sizeof(*(x)))
bool hard_rock;
void in_between_events();
void *rand_handler(void *arg);
int screen_x = 2560;
int screen_y = 1440;
int offset = 0;


int main(int argc, const char * argv[]) {
    char *str = argv[1];
    FILE *osz = fopen(str, "r");
    
    for (int i=0; i<argc; i++) {
        
        switch (argv[i][1])
        {
            case 'd':
                double_time = true;
                break;
            case 'h':
                hard_rock = true;
                break;
        }
    }
    parseIn(osz);
    
    printf("Osu player: start \n");
    
    pthread_t keyHandler;
    pthread_create(&keyHandler,NULL,keyHandlerFunc,NULL);
    
    begin();
}

void run_hitcircle(point p)
{
    
    p = map_to_screen(p);
    click(p);
    
    
}


void run_spinner(point p, double end)
{
    p = map_to_screen(p);
    beginDrag(p);
    
    while (gettime() - (start+offset) < end) {
        p.x = 1280;
        p.y = 720-500;
        moveTo(p);
        usleep(100);
        p.x = 1280-500;
        p.y = 720;
        moveTo(p);
        usleep(100);
        p.x = 1280;
        p.y = 720+500;
        moveTo(p);
        usleep(100);
        p.x = 1280+500;
        p.y = 720;
        moveTo(p);
        usleep(100);
    }
    endDragNull();
    
    
}

void begin()
{
    
    printf("Waiting for begin signal....");
    while (!keyPressed || (keyCode != 1) )
    {
        usleep(100);
    }
    keyPressed = false;
    printf("Begin signal obtained!\n");
    start = gettime();
    for (int i = 0; i<numEvents-1; i++)
    {
        currEvent = i;
        double now = gettime();
        double oldtime;
        while (now-(start+offset) < events[i].time) {
            oldtime = now;
            now = gettime();
            if (keyPressed)
            {
                if (keyCode == 35) {
                    printf("Pause\n");
                    keyPressed = false;
                    while (!keyPressed) {
                        
                        usleep(1000);
                    }
                    printf("Unpaused\n");
                }
                
                else if (keyCode == 24) {
                    offset += 25;
                }
                else if (keyCode == 27) {
                    offset -= 25;
                }
                else if (keyCode == 30) {
                    offset += 10;
                }
                else if (keyCode == 33) {
                    offset -= 10;
                }
                else if (keyCode == 31)
                {
                    offset = now - events[0].time;
                }
                
            }
            keyPressed = false;
            //   if (now-oldtime > 15)
            //  {
            //  in_between_events();
            // }
          //  usleep(1);
        }
        long x = (gettime() - start) - events[i].time;
        printf("Event #%d  offset: %ldms\n",i,x);
        eventHandler(NULL);
        usleep(10);
        updateTiming();

        
    }
    
}

/*void in_between_events()
 {
 point p;
 if (!currEvent==0)
 {
 double x_distance = events[currEvent].p.x - events[currEvent-1].p.x;
 double y_distance = events[currEvent].p.y - events[currEvent-1].p.y;
 double curr_time = (gettime()-(start+offset)) - events[currEvent-1].time;
 int total_time = events[currEvent].time - events[currEvent-1].time;
 
 p.x = (int)(((curr_time/total_time) * x_distance) + events[currEvent-1].p.x);
 p.y = (int)(((curr_time/total_time) * y_distance) + events[currEvent-1].p.y);
 p = map_to_screen(p);
 //      p = add_error(p);
 moveTo(p);
 }
 
 }*/


void updateTiming()
{
    if (gettime() - start >= timing_events[currTiming+1].time && currTiming < numTimingEvents-1)
    {
        currTiming++;
        if (timing_events[currTiming].mpb < 0) {
            timing_events[currTiming].mpb = timing_events[currTiming].mpb + timing_events[currTiming-1].mpb;
        }
        printf("New timing @ t=%f\n", gettime());
        
    }
}

void *eventHandler(void *arg)
{
    event e = events[currEvent];
    if (e.type == 1 || e.type == 5)
    {
        run_hitcircle(e.p);
    }
    else if (e.type == 2 || e.type == 6)
    {
        run_slider();
    }
    else if (e.type==12)
    {
 //       run_spinner(e.p, e.spinner_end_time);
    }
    return NULL;
}

void parseIn(FILE *osz)
{
    if ( osz )
    {
        size_t i, j;
        char buffer[BUFSIZ], *ptr;
        fgets(buffer, sizeof buffer, osz);
        while (strncmp(buffer, "SliderMultiplier:",12) != 0)
        {
            fgets(buffer, sizeof buffer, osz);
        }
        ptr = buffer;
        ptr+=17;
        sliderMultiplier = strtod(ptr, &ptr);
        fgets(buffer, sizeof buffer, osz);
        while (strncmp(buffer, "[TimingPoints]",6) != 0)
        {
            fgets(buffer, sizeof buffer, osz);
        }
        ///////timing points//////////
        int count;
        fgets(buffer, sizeof buffer, osz);
        while (strncmp(buffer, "\r\n",6) != 0)
        {
            ptr = buffer;
            timing_events[count].time = (int)strtol(ptr, &ptr, 10);
            ptr++;
            timing_events[count].mpb = strtod(ptr, &ptr);
            ptr++;
            timing_events[count].meter = (int)strtol(ptr, &ptr, 10);
            fgets(buffer, sizeof buffer, osz);
            count++;
        }
        numTimingEvents = count;
        ////////////events////////////
        fgets(buffer, sizeof buffer, osz);
        while (strncmp(buffer, "[HitObjects]",6) != 0)
        {
            fgets(buffer, sizeof buffer, osz);
        }
        for ( i = 0; fgets(buffer, sizeof buffer, osz); ++i )
        {
            numEvents++;
            for ( j = 0, ptr = buffer; j < 5; ++j, ++ptr )
            {
                switch (j) {
                    case 0:
                        events[i].p.x = (int)strtol(ptr, &ptr, 10);
                        break;
                        
                    case 1:
                        events[i].p.y = (int)strtol(ptr, &ptr, 10);
                        break;
                    case 2:
                        events[i].time = (int)strtol(ptr, &ptr, 10);
                        break;
                    case 3:
                        events[i].type = (int)strtol(ptr, &ptr, 10);
                        if (events[i].type == 2 || events[i].type == 6) {
                            sliderParse(ptr, (int)i);
                        }
                        else if (events[i].type == 12)
                        {
                            ptr++;
                            strtol(ptr, &ptr, 10);
                            ptr++;
                            events[i].spinner_end_time =(int)strtol(ptr, &ptr, 10);
                        }
                        break;
                }
                
            }
        }
        numEvents++;
        /////////////////////////////////
        fclose(osz);
    }
}

void sliderParse (char *ptr, int i)
{
    ptr++;
    strtol(ptr, &ptr, 10);
    ptr++;
    events[i].slider_type = ptr[0];
    int count = 0;
    ptr++;
    while(strncmp(&ptr[0], "|",1) == 0)
    {
        ptr++;
        events[i].slider_points[count].x = (int)strtol(ptr, &ptr, 10);
        ptr++;
        events[i].slider_points[count].y = (int)strtol(ptr, &ptr, 10);
        count++;
    }
    events[i].num_of_points = count;
    ptr++;
    events[i].slider_repeat = (int)strtol(ptr, &ptr, 10);
    ptr++;
    events[i].slider_length = (int)strtol(ptr, &ptr, 10);
    
}


CGEventRef on_tap(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *refcon) {
    keyCode = (int)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
    keyPressed = true;
    return event;
}


void *keyHandlerFunc(void *arg)
{
    printf("\n");
    CGEventFlags flags = CGEventSourceFlagsState(kCGEventSourceStateCombinedSessionState);
    CGEventMask mask = CGEventMaskBit(kCGEventKeyDown);
    CFMachPortRef tap = CGEventTapCreate(kCGSessionEventTap, kCGHeadInsertEventTap, 0, mask, on_tap, &flags);
    if (!tap) {
        fprintf(stdout, "This program requires sudo.");
        return NULL;
    }
    CFRunLoopSourceRef runloop = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, tap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runloop, kCFRunLoopCommonModes);
    CGEventTapEnable(tap, true);
    CFRunLoopRun();
    return NULL;
}
