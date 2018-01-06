
#include "mouse.h"
#include "definitions.h"
void click(point p);
void beginDrag(point p);
void endDrag(point p);
void endDragNull();
void moveTo(point p);
void click(point p)
{
    CGEventRef leftDown = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(p.x, p.y), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, leftDown);
    CFRelease(leftDown);
    
    CGEventRef leftUp = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(p.x, p.y), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, leftUp);
    CFRelease(leftUp);
}

void endDrag(point p)
{
    CGEventRef leftUp = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, CGPointMake(p.x, p.y), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, leftUp);
    CFRelease(leftUp);
}
void endDragNull()
{
    CGEventRef mouse_position = CGEventCreate(NULL);
    CGPoint p = CGEventGetLocation(mouse_position);
    CGEventRef leftUp = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseUp, p, kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, leftUp);
    CFRelease(leftUp);
}

void beginDrag(point p)
{
    CGEventRef leftDown = CGEventCreateMouseEvent(NULL, kCGEventLeftMouseDown, CGPointMake(p.x, p.y), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, leftDown);
    CFRelease(leftDown);
}

void moveTo(point p)
{
    CGEventRef moved = CGEventCreateMouseEvent(NULL, kCGEventMouseMoved, CGPointMake(p.x, p.y), kCGMouseButtonLeft);
    CGEventPost(kCGHIDEventTap, moved);
    CFRelease(moved);
}
