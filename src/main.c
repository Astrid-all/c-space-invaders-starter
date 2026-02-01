#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>
#include "entity.h"
#include "game.h"

int main(void)
{
    TTF_Init();
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface* surfaceMessage=NULL;
    SDL_Texture* Message=NULL;

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
        .vy = 0,
        .life = FULL_LIFE};
    int nb_ennemis = 20;

    /// Définition des ennemis - Initialisation
    Entity_Alien* liste_alien = malloc(nb_ennemis * sizeof(Entity_Alien)); // liste de structures = il faut allouer la mémoire
    if(liste_alien==NULL){
        return NULL;
    }

    for(int i=0;i<nb_ennemis;i++){
        liste_alien[i].w = ALIEN_WIDTH;
        liste_alien[i].h = ALIEN_HEIGHT;
        float dist_typ_x = (SCREEN_WIDTH)/(NUM_ALIEN_PER_LINE +1) ; // espacement typique entre 2 ennemis 
        // espacement des ennemis de façon régulière 
        liste_alien[i].y = SPACE_VERTICAL *(i/NUM_ALIEN_PER_LINE);    
        liste_alien[i].vx = ALIEN_SPEED_X;
        liste_alien[i].x = dist_typ_x*(i%NUM_ALIEN_PER_LINE +1);
        liste_alien[i].hurt = false;
        

    }
    // Initialisation bullets joueur et ennemie
    Entity bullet = {0};
    bool bullet_active = false;
    Entity bullet_enemy = {0};
    bool bullet_active_enemy = false;
    bool victory = false;
    bool echap = false;

    clock_t start_time = clock();
    int increase_speed = 0;
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
        if((float)((second_time - start_time)/CLOCKS_PER_SEC)>=12){
            increase_speed +=1;
            start_time=second_time;
        }
        
        update(&player, &bullet,&bullet_enemy, liste_alien,increase_speed,nb_ennemis, &bullet_active,&bullet_active_enemy, dt);
        render(renderer, &player,liste_alien,nb_ennemis, &bullet, &bullet_enemy, bullet_active, bullet_active_enemy);
        running = end_game(liste_alien,&player,nb_ennemis,&running,victory);
        }
        else if((running==false)&&(echap ==false)){
            const Uint8 *keys = SDL_GetKeyboardState(NULL);
           // TTF_Font* Police = TTF_OpenFont("../pixel-gaming-font/PixelGamingRegular-d9w0g.ttf", 22);
           // SDL_Color White = {.r =255, .g =255, .b=255};
            
           // if (victory==true){
           //     surfaceMessage = TTF_RenderText_Solid(Police, "VICTOIRE", White); 
           // }
           // else{
           //     surfaceMessage = TTF_RenderText_Solid(Police, "GAME OVER", White);                          affichage des messages de victoire/défaite = à modifier
           // }                                                                                                + à mettre dans game.c

            //Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

           // SDL_Rect Message_rect; 
          //  Message_rect.x = (SCREEN_WIDTH-MESSAGE_WIDTH)/2;  
          //  Message_rect.y = (SCREEN_HEIGHT-MESSAGE_HEIGHT)/2; 
          //  Message_rect.w = MESSAGE_WIDTH; 
          //  Message_rect.h = MESSAGE_HEIGHT; 

           // SDL_RenderCopy(renderer, Message, NULL, &Message_rect);
            handle_input(&running, keys, &player, &bullet, &bullet_active,&echap);
          //  TTF_CloseFont(Police);
        }
    }

    // Affichage fin de partie  

    SDL_FreeSurface(surfaceMessage);
    SDL_DestroyTexture(Message);

    cleanup(window, renderer);
    free(liste_alien);
    TTF_Quit();
    return 0;
}
