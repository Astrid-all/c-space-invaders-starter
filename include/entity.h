#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
    int life;
} Entity;

typedef struct {
    float x,y;
    float vx;
    int w,h;
    bool hurt;
} Entity_Alien;

#endif
