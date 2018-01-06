#ifndef __osu_test__mouse__
#define __osu_test__mouse__

#include <stdio.h>
#include "definitions.h"
void click(point p);
void beginDrag(point p);
void endDrag(point p);
void endDragNull();
void moveTo(point p);
#endif /* defined(__osu_test__mouse__) */
