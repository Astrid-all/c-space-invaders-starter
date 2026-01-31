#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet,bool *bullet_active,bool *echap) 
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;

    if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
    {
        *bullet_active = true;
        bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
        bullet->y = player->y;
        bullet->w = BULLET_WIDTH;
        bullet->h = BULLET_HEIGHT;
        bullet->vy = -BULLET_SPEED;
    }
    if(keys[SDL_SCANCODE_RETURN]){
        *echap = true;
    }
}

void update(Entity *player, Entity *bullet,Entity *bullet_enemy,Entity_Alien* alien,
    size_t taille_alien, bool *bullet_active, bool *bullet_active_enemy, float dt){

    player->x += player->vx * dt;

    if (player->x < 0)
        player->x = 0;
    if (player->x + player->w > SCREEN_WIDTH)
        player->x = SCREEN_WIDTH - player->w;

    // Gestion bullet joueur
    if (*bullet_active)
    {
        bullet->y += bullet->vy * dt;
        if (bullet->y + bullet->h < 0)
            *bullet_active = false;
    }
 
    // Gestion aliens
    int nb_aliens_restants = 0;
    for(size_t i =0;i<taille_alien;i++){
        alien[i].y += alien[i].vy*dt;
        // vérification touché bullet
        if((((bullet->x>=alien[i].x)&&(bullet->x<=alien[i].x+ALIEN_WIDTH)) ||(((bullet->x+BULLET_WIDTH)>=alien[i].x)&&(bullet->x+BULLET_WIDTH<=alien[i].x+ALIEN_WIDTH)))
                && ((bullet->y>alien[i].y)&&(bullet->y <= alien[i].y + ALIEN_HEIGHT)) ){
            alien[i].hurt = true;
            alien[i].x =0;
            alien[i].y = 0;
            alien[i].w = 0;
            alien[i].h =0;
            alien[i].vy = 0;
            *bullet_active = false;
        }
        if(alien[i].hurt==false){
            nb_aliens_restants +=1 ;
        }
        
    }

    int* liste_aliens_restants = malloc(sizeof(int)*nb_aliens_restants);
    int compteur = 0;
    for(size_t j =0; j<taille_alien;j++){
        if (alien[j].hurt ==false){
            liste_aliens_restants[compteur] = j; // on garde les indices des aliens restants
            compteur +=1;
    }
    }

    //Gestion bullet ennemies 
    if(*bullet_active_enemy)
    {
        bullet_enemy->y+=bullet_enemy->vy *dt;
        
        // joueur touché
        if(
            (((bullet_enemy->x <= player->x+PLAYER_WIDTH)&&(bullet_enemy->x>=player->x)) ||
            ((bullet_enemy->x + BULLET_WIDTH <= player->x + PLAYER_WIDTH) && (bullet_enemy->x + BULLET_WIDTH >=player->x))) &&
            ((bullet_enemy->y + BULLET_HEIGHT <=player->y+PLAYER_HEIGHT ) &&(bullet_enemy->y + BULLET_HEIGHT>=player->y))
        ){
            player->life -= BULLET_HARM;
            *bullet_active_enemy = false; 
        }
        else if(bullet_enemy->y>SCREEN_HEIGHT){
            *bullet_active_enemy = false;
        }
    }
    

    else if(*bullet_active_enemy == false){

        srand(time(NULL)); //initialisation du random
        int pos_emetteur = rand() % nb_aliens_restants; // choix aléatoire d'un ennemi parmi ceux restants
        bullet_enemy->y = alien[liste_aliens_restants[pos_emetteur]].y + ALIEN_HEIGHT;
        bullet_enemy->x = alien[liste_aliens_restants[pos_emetteur]].x + ALIEN_WIDTH / 2;
        bullet_enemy->vy = BULLET_SPEED * 0.4;
        bullet_enemy->w = BULLET_WIDTH;
        bullet_enemy->h = BULLET_HEIGHT;
        *bullet_active_enemy = true;
    }
    free(liste_aliens_restants);

}


// gestion fin de partie
bool end_game(Entity_Alien* alien, Entity *player, size_t taille_alien, bool *running,bool victory){
    int compt = 0;
    // contact avec les aliens
    for(size_t i = 0; i<taille_alien;i++){
        if(alien[i].y + ALIEN_HEIGHT >= player->y+PLAYER_HEIGHT){  //touche le sol
            victory = false;
            return *running = false;
        }
        if((alien[i].y + ALIEN_HEIGHT>= player->y )&&(((alien[i].x + ALIEN_WIDTH>=player->x)&&(alien[i].x+ALIEN_WIDTH<= player->x + PLAYER_WIDTH))||
        ((alien[i].x >=player->x)&&(alien[i].x<= player->x + PLAYER_WIDTH)))){ // touche le joueur
            victory=false;
            return *running = false;
        }
        else if(alien[i].hurt == true){
            compt +=1;
        }
    }
    // destruction des aliens
    if(compt == (int)taille_alien){ // tous les aliens sont détruits
        victory = true;
        return *running = false;
    }
    // fin de vie
    else if(player->life<=0){
        victory = false;
        return *running = false;
    }

    return *running = true;
}


void render(SDL_Renderer *renderer, Entity *player,Entity_Alien* alien,size_t taille_alien, Entity *bullet, Entity *bullet_enemy, bool bullet_active,
    bool bullet_active_enemy)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // aperçu joueur
    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    // barre de niveau de vie
    SDL_Rect full_life = {30,15,LIFE_CHART_WIDTH,LIFE_CHART_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &full_life);
    SDL_Rect current_life = {30,15,(int)LIFE_CHART_WIDTH*(player->life / FULL_LIFE),LIFE_CHART_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
    SDL_RenderFillRect(renderer, &current_life);

    // aperçu graphique des aliens
    SDL_Rect alien_rect[taille_alien];
    for(size_t i =0; i<taille_alien;i++){ 
        // on définit les coordonnées du rectangle si l'alien n'a pas été touché
        if(alien[i].hurt==false){
        alien_rect[i].x = (int)alien[i].x;
        alien_rect[i].y = (int)alien[i].y;
        alien_rect[i].w = alien[i].w;
        alien_rect[i].h =  alien[i].h;
        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        SDL_RenderFillRect(renderer, &alien_rect[i]);
        }
    }
    // afichage bullets
    if (bullet_active)
    {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
    }

    if(bullet_active_enemy){
        SDL_Rect bullet_enemy_rect = {
            (int)bullet_enemy->x, (int)bullet_enemy->y, bullet_enemy->w, bullet_enemy->h};
        SDL_SetRenderDrawColor(renderer,255,0,0,255);
        SDL_RenderFillRect(renderer,&bullet_enemy_rect);
        }
    
    SDL_RenderPresent(renderer);

}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
