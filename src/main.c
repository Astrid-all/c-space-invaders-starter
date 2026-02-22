#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
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

    if(!init_ttf()){
        return 1;
    }
    TTF_Font* Police = TTF_OpenFont("pixel-gaming-font/PixelGamingRegular-d9w0g.ttf", 52);

    bool running = true;
    Uint32 last_ticks = SDL_GetTicks();

    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0,
        .life = FULL_LIFE};
    int nb_ennemis = 20;

    /// Définition des ennemis - Initialisation
    Entity_Alien* liste_alien = malloc(nb_ennemis * sizeof(Entity_Alien)); // liste de structures = il faut allouer la mémoire
    if(liste_alien==NULL){
        return NULL;
    }

    for(int i=0;i<nb_ennemis;i++){
        // gestion des classes
        if((i%NUM_ALIEN_PER_LINE == 0)||(i%NUM_ALIEN_PER_LINE == NUM_ALIEN_PER_LINE-1)){
            liste_alien[i].class = 0; // ennemis rapides sur les côtés
        }
        else{
            liste_alien[i].class = rand() % 3 + 1; 
        }
        // initialisation vie selon la classe
        if(liste_alien[i].class ==1){
            liste_alien[i].life = STRONG_ALIEN_LIFE;
        }
        else{
            liste_alien[i].life = ALIEN_LIFE;
        }
        liste_alien[i].w = ALIEN_WIDTH;
        liste_alien[i].h = ALIEN_HEIGHT;
        float dist_typ_x = (SCREEN_WIDTH)/(NUM_ALIEN_PER_LINE +1) ; // espacement typique entre 2 ennemis 
        // espacement des ennemis de façon régulière 
        liste_alien[i].y = SPACE_VERTICAL *(i/NUM_ALIEN_PER_LINE);    
        liste_alien[i].vx = ALIEN_SPEED_X;
        liste_alien[i].x = dist_typ_x*(i%NUM_ALIEN_PER_LINE +1);  

    }
    // Initialisation bullets joueur et ennemie + coeur
    Entity bullet = {0};
    bool bullet_active = false;
    Entity bullet_enemy = {0};
    bool bullet_active_enemy = false;
    bool shooter_turn = false;

    Entity heart = {0};
    bool heart_present = false;
    bool victory = false;
    bool echap = false;

    clock_t start_time = clock();
    int increase_speed = 0;
    clock_t heart_init_time = clock();

    // initialisation chronomètre de partie + score
    int score = 0;
    int* point_score = &score;
    clock_t global_timer = clock();
    float timing = 0;
    // boucle de jeu
    while (running||(echap==false))
    { 
        if(running){
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active,&echap);

        clock_t second_time = clock();
        if((float)((second_time - start_time)/CLOCKS_PER_SEC)>=INCREASE_SPEED_TIME){
            increase_speed +=1;
            start_time=second_time;
            
        }
        //Mise à jour des timer
        timing = (float)((clock()-global_timer)/CLOCKS_PER_SEC);
        clock_t second_heart_time = clock();
        float time_delta = (float)((second_heart_time-heart_init_time)/CLOCKS_PER_SEC);
        // maj des données
        update(&player, &bullet,&bullet_enemy, liste_alien,increase_speed,nb_ennemis, &bullet_active,&bullet_active_enemy,&heart, &heart_present, &shooter_turn, time_delta, point_score, dt);
        render(renderer, &player,liste_alien,nb_ennemis, &bullet, &bullet_enemy, bullet_active, bullet_active_enemy, heart_present, &heart);
        

        if((time_delta>=APPEARANCE_TIME)&&(heart_present)){
            heart_init_time=second_heart_time;
        }
        running = end_game(liste_alien,&player,nb_ennemis,&running,&victory);

        }
        else if((echap==false)&&(running==false)){ 
            // Affichage fin de partie  
            final_message(renderer,&victory,Police,score,timing);
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
            handle_input(&running, keys, &player, &bullet, &bullet_active,&echap);
        }
    }


    cleanup(window, renderer, Police);
    free(liste_alien);
    return 0;
}
