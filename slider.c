
#include "slider.h"
#include "definitions.h"

float sliderMultiplier;
float getSliderTime();
const int precision = 5;
int current_repeat = 1;

point bernstein(float t,int n);
long double factorial(int x);
long double binomial(int f, int n);
void update_slider(event e);

void run_slider()
{
    current_repeat=1;
    double x = (gettime() - start) - events[currEvent].time;
    
    printf("Slider start offset: %f\n", x);
    
    printf("Slider init\n");
    event e = events[currEvent];
    
    point next = map_to_screen(e.p);
    //next = add_error(next);

    beginDrag(next);
    ////////
    while (gettime()-(start+offset) < e.time + (getSliderTime(e)*e.slider_repeat)) {
        update_slider(e);
        if (gettime() - (start+offset) >= e.time + getSliderTime(e)*current_repeat && e.slider_repeat > 1) {
            current_repeat++;
        }
    }
    endDragNull();
}
void update_slider(event e)
{
    float time = (((gettime() - (start+offset)) - e.time) - (getSliderTime(e)*(current_repeat-1))) / getSliderTime(e);
    if (current_repeat % 2 == 0)
    {
        time = 1.0 - time;
    }
   
    point next = bernstein(time, e.num_of_points-1);
    next = map_to_screen(next);
   // next = add_error(next);
    moveTo(next);
    usleep(1);
}

float getSliderTime(event e) //get slider duration
{
  //  if(!double_time)
   // {
    return (timing_events[currTiming].mpb * (e.slider_length / sliderMultiplier) / 100);
  //  }
  //  else
   // {
   // return (timing_events[currTiming].mpb * (e.slider_length / sliderMultiplier) / 100) / 2;
   // }
}

long double factorial(int x){
    long double fact=1;
    int p=1;
    if(x==0 || x==1)
        return 1;
    else{
        while(p<=x){
            fact*=p;
            p++;}
    }
    return fact;
}

long double binomial( int f, int n){
    double com=factorial(n)/(factorial(f)*factorial(n-f));
    return com;
}

point bernstein(float t,int n)
{
    point N;N.x=N.y=0;
    n++;
    for(int i=0;i<=n;i++){
        if (i==0)
        {
            N.x+=events[currEvent].p.x*binomial(i,n)*pow(t,i)*pow((1-t),(n-i));
            N.y+=events[currEvent].p.y*binomial(i,n)*pow(t,i)*pow((1-t),(n-i));
            
        }
        else
        {
            int y = i - 1;
            N.x+=events[currEvent].slider_points[y].x*binomial(i,n)*pow(t,i)*pow((1-t),(n-i));
            N.y+=events[currEvent].slider_points[y].y*binomial(i,n)*pow(t,i)*pow((1-t),(n-i));
        }
    }
    return N;
    
}