#ifndef __osu_test__util__
#define __osu_test__util__
#include "definitions.h"

point map_to_screen(point p);
double gettime(void);
float map(float input, float input_start, float input_end, float output_start, float output_end);

#endif /* defined(__osu_test__util__) */
