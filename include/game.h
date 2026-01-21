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

#define ALIEN_WIDTH 25
#define ALIEN_HEIGHT 25

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet, Entity_Alien* liste_alien,size_t taille_alien,bool *bullet_active, float dt);
void render(SDL_Renderer *renderer, Entity *player, Entity_Alien* liste_alien,size_t taille_alien,Entity *bullet, bool bullet_active);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif
