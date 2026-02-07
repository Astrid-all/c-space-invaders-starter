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

void update(Entity *player, Entity *bullet,Entity *bullet_enemy,Entity_Alien* alien,int increase_speed,
    size_t taille_alien, bool *bullet_active, bool *bullet_active_enemy, Entity *heart, bool *heart_present, float time_delta, float dt){

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
        
        // vérification touché bullet
        if((((bullet->x>=alien[i].x)&&(bullet->x<=alien[i].x+ALIEN_WIDTH)) ||(((bullet->x+BULLET_WIDTH)>=alien[i].x)&&(bullet->x+BULLET_WIDTH<=alien[i].x+ALIEN_WIDTH)))
                && ((bullet->y>alien[i].y)&&(bullet->y <= alien[i].y + ALIEN_HEIGHT)) ){
            alien[i].hurt = true;
            alien[i].x =0;
            alien[i].y = 0;
            alien[i].w = 0;
            alien[i].h =0;
            alien[i].vx = 0;
            *bullet_active = false;
            bullet->x = 0;
            bullet->vx = 0;
            bullet->vy = 0;
            bullet->w = 0;
            bullet->h = 0;
            bullet->y = 0;
        }
        if(alien[i].hurt==false){
            nb_aliens_restants +=1 ;
        }
        
    }

    int* liste_aliens_restants = malloc(sizeof(int)*nb_aliens_restants);
    int compteur = 0;
    bool cond_left = false;
    bool cond_right = false;

    for(size_t j =0; j<taille_alien;j++){
        if (alien[j].hurt ==false){
            liste_aliens_restants[compteur] = j; // on garde les indices des aliens restants
            compteur +=1;
            // on regarde si l'un des aliens restants touche un des bords
        if((alien[j].x>=SCREEN_WIDTH-20)&&(alien[j].vx>0)){
            cond_right = true;
        }
        else if((alien[j].x<=15)&&(alien[j].vx<0)){
            cond_left = true;
        }
    }
    
    }
    // Déplacement des aliens restants
    for(int k =0;k<nb_aliens_restants;k++){
        if(cond_left){
            alien[liste_aliens_restants[k]].vx = ALIEN_SPEED_X + INCREASE_SPEED*increase_speed;
            alien[liste_aliens_restants[k]].x += alien[liste_aliens_restants[k]].vx*dt;
        }
        else if(cond_right){
            alien[liste_aliens_restants[k]].vx = -ALIEN_SPEED_X - increase_speed * INCREASE_SPEED;
            alien[liste_aliens_restants[k]].y += SPACE_VERTICAL; 
        }
        else{
            alien[liste_aliens_restants[k]].x += alien[liste_aliens_restants[k]].vx*dt; 
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

    // gestion des coeurs
    // position aléatoire sur la même ligne que celle du joueur (mais différente de la position joueur)
    // temps d'apparition du premier coeur
    // temps entre 2 coeurs                
    // récupération du coeur
    if(*heart_present ==true){
        if(((heart->x<=player->x + PLAYER_WIDTH)&&(heart->x>=player->x))||((heart->x + HEART_WIDTH<=player->x + PLAYER_WIDTH)&&
        (heart->x+HEART_WIDTH>=player->x))){
            if(player->life + RECOVERY <FULL_LIFE){
                player->life += RECOVERY;
            }
            else{
                player->life = FULL_LIFE;
            }
            *heart_present = false;
            heart->x = 0;
            heart->y = 0;
            heart->w =0;
            heart->h =0;
            heart->vx =0;
            heart->life = 0;
            heart->vy =0;
        }
    }

    else if((int)time_delta>=APPEARANCE_TIME){

        srand(time(NULL));
        *heart_present = true;
        int pixel_pos = rand() % (SCREEN_WIDTH -30) + 15;
        if ((pixel_pos>=player->x)&&(pixel_pos<= player->x + PLAYER_WIDTH)){
            // le coeur apparaît sur le joueur => on le déplace
            if((pixel_pos + PLAYER_WIDTH + 30) <=(SCREEN_WIDTH - 15)){
                // il y a assez de place à droite du joueur
                pixel_pos = pixel_pos + PLAYER_WIDTH + 30;
                heart->x = pixel_pos;
                heart->y = player->y;
                heart->w = HEART_WIDTH;
                heart->h = HEART_HEIGHT;
                heart->vx =0;
                heart->vy = 0;
                heart->life = 0;

            }
            else if((pixel_pos-PLAYER_WIDTH - 30) >= 15){
                // il y a assez de place à gauche
                pixel_pos = pixel_pos - PLAYER_WIDTH - 30;
                heart->x = pixel_pos;
                heart->y = player->y;
                heart->w = HEART_WIDTH;
                heart->h = HEART_HEIGHT;
                heart->vx =0;
                heart->vy = 0;
                heart->life = 0;

            }
        }
        else{
            heart->x = pixel_pos;
            heart->y = player->y ;
            heart->w = HEART_WIDTH;
            heart->h = HEART_HEIGHT;
            heart->vx =0;
            heart->vy = 0;
            heart->life = 0;
        }

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
    bool bullet_active_enemy, bool heart_present, Entity *heart)
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
    SDL_Rect current_life = {30,15,(int)(LIFE_CHART_WIDTH*((float)player->life / (float)FULL_LIFE)),LIFE_CHART_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 139, 0, 0, 255);
    SDL_RenderFillRect(renderer, &current_life);

    //affichage des coeurs
    if (heart_present)
    {
        SDL_Rect heart_rect ={
            (int)heart->x, (int)heart->y, heart->w, heart->h
        };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &heart_rect);
    }

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
    // affichage bullets
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
