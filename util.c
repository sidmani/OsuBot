#include "util.h"
#include <mach/mach_time.h>

double gettime(void) {
/* not my code, probably was used for getting time difference */
}

float map(float input, float input_start, float input_end, float output_start, float output_end) {
    float input_range = input_end - input_start;
    float output_range = output_end - output_start;
    
    return (input - input_start)*output_range / input_range + output_start;
    
}

point map_to_screen(point p) {
    if (hard_rock) {
        p.y = (int)map(p.y, 384, 0, 181, 1314);
    } else {
        p.y = (int)map(p.y, 0, 384, 181, 1314);

    }
    p.x = (int)map(p.x, 0, 512, 525, 2035);

    return p;
}
