#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f
#define FULL_LIFE 100

#define ALIEN_WIDTH 25
#define ALIEN_HEIGHT 25
#define ALIEN_SPEED_X 15.0f
#define INCREASE_SPEED 5.0f
#define INCREASE_SPEED_TIME 11
#define NUM_ALIEN_PER_LINE 10
#define SPACE_VERTICAL 54

#define STRONG_ALIEN_LIFE 60
#define ALIEN_LIFE 20
#define ALIEN_LIFE_CHART_WIDTH 20
#define ALIEN_LIFE_CHART_HEIGHT 10
#define DISTANCE_LIFE_CHART 15

#define HEART_WIDTH 20
#define HEART_HEIGHT 20
#define RECOVERY 20
#define APPEARANCE_TIME 12

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f
#define BULLET_HARM 20

#define MESSAGE_WIDTH 500
#define MESSAGE_HEIGHT 100
#define LIFE_CHART_WIDTH 100
#define LIFE_CHART_HEIGHT 10

bool init(SDL_Window **window, SDL_Renderer **renderer);
bool init_ttf();
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active, bool *echap);
void update(Entity *player, Entity *bullet, Entity *bullet_enemy, Entity_Alien* liste_alien,int increase_speed, size_t taille_alien,bool *bullet_active,bool *bullet_active_enemy, Entity *heart,bool *heart_present,bool *shooter_turn, float time_delta, int* point_score, float dt);
bool end_game(Entity_Alien* alien,Entity *player,size_t taille_alien, bool *running, bool *victory);
void render(SDL_Renderer *renderer, Entity *player, Entity_Alien* liste_alien,size_t taille_alien,Entity *bullet, Entity *bullet_enemy, bool bullet_active, bool bullet_active_enemy, bool heart_present, Entity *heart);
void final_message(SDL_Renderer *renderer, bool *victory,TTF_Font* Police, int score, float timing);
void cleanup(SDL_Window *window, SDL_Renderer *renderer,TTF_Font* Police);

#endif
