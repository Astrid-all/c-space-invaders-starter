#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
    int life;
} Entity;

enum Enemy {
    FAST,
    TOUGH,
    SHOOTER,
    BASIC
};
typedef enum Enemy enemy_class;

typedef struct {
    float x,y;
    float vx;
    int w,h;
    int life;
    enemy_class class;
} Entity_Alien;



#endif
