#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"

int main(void)
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer))
    {
        return 1;
    }

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();

    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0};
    int nb_ennemis = 10;

    /// Définition des ennemis - Initialisation
    Entity_Alien* liste_alien = malloc(nb_ennemis * sizeof(Entity_Alien)); // liste de structures = il faut allouer la mémoire
    if(liste_alien==NULL){
        return NULL;
    }
    float vitesse_alien = 7;

    for(int i=0;i<nb_ennemis;i++){
        //int num_ligne =  i/4;
        int nb_en_ligne = 7; // nb_max_d'ennemis en ligne
        liste_alien[i].w = ALIEN_WIDTH;
        liste_alien[i].h = ALIEN_HEIGHT;
        float dist_typ_x = (SCREEN_WIDTH)/(nb_en_ligne +1) ; // espacement typique entre 2 ennemis sur une même ligne
        float dist_typ_y = SCREEN_HEIGHT/11 ; 
        // espacement des ennemis de façon régulière 
        liste_alien[i].y = dist_typ_y *(i/nb_en_ligne);    
        liste_alien[i].vy = vitesse_alien;
        liste_alien[i].x = dist_typ_x*(i%nb_en_ligne +1);
        liste_alien[i].hurt = false;
        // A revoir ...
        // disposition sur la dernière ligne
        //if(num_ligne != (nb_ennemis/4+1)){  // on réparti régulièrement les aliens restants
            
            
        //}
        //else{
        //    liste_alien[i].x = ((SCREEN_WIDTH*2) /(nb_ennemis%4))*(i%4 + 1);
        //}

    }

    Entity bullet = {0};
    bool bullet_active = false;

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        update(&player, &bullet,liste_alien,nb_ennemis, &bullet_active, dt);
        render(renderer, &player,liste_alien,nb_ennemis, &bullet, bullet_active);
    }

    cleanup(window, renderer);
    free(liste_alien);
    return 0;
}
