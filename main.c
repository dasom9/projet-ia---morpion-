#include <stdlib.h>
#include <stdio.h>
#include "morpion.h"
#include "morpion_sdl.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int main() {
  
  // Initialisation graphique

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  TTF_Init();
  SDL_Window * window = NULL;
  SDL_Renderer * renderer = NULL;
  TTF_Font * police = NULL;
  SDL_Event event;
  SDL_bool quit = SDL_FALSE;

  police = TTF_OpenFont("BebasNeue-Regular.ttf", 65);
  SDL_Color orange = {
    255,
    127,
    40,
    255
  };
  SDL_Color grey = {
    22,
    22,
    22,
    255
  };
  int continuer = 1;

  // Initialisation jeu morpion

  int i, j, tour, tour_max, rejouer, N, mode;
  char player;
  morpion jeu;

  window = SDL_CreateWindow("Morpion by Théo KERNEVES & Aymeric LAVEDRINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    600, 600, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

  setWindowColor(renderer, orange);

  drawButton(renderer, grey, 200, 200, 200, 50, "Jouer contre IA");
  drawButton(renderer, grey, 200, 300, 200, 50, "Jouer contre un ami");
  
  SDL_RenderPresent(renderer);

  // Gestion des 2 boutons du menu principal
  while (continuer) {
    SDL_WaitEvent( & event);
    switch (event.type) {
    case SDL_QUIT:
      continuer = 0;
      break;
    case SDL_MOUSEBUTTONUP:
      if (isInCoords(200, 200, 200, 50, event)) {
        mode = 1;
        continuer = 0;
      } else if (isInCoords(200, 300, 200, 50, event)) {
        mode = 2;
        continuer = 0;
      }

      break;
    }

  }

  N = askN(event, renderer);
  jeu = initGame(N);
  tour_max = N * N;
  drawGrille(N, renderer, jeu);

  if (mode == 1) { // IA
    for (tour = 0; tour < tour_max && !gagnant(jeu, player, N); tour++) {
      player = tour % 2;

      if (player == 1) {
        do {
          waitToPlay(N, renderer, player, event, & i, & j);
          rejouer = jouer(jeu, i, j, 1, N);
        } while (rejouer);

      } else if (player == 0) {
        MinMax_AB(jeu, N, & i, & j, -1000, 1000, N*N);
        jouer(jeu, i, j, 0, N);
      }

      afficherGrille(jeu, N);
      drawGrille(N, renderer, jeu);
    }

    if (gagnant(jeu, player, N) == -1) drawButton(renderer, orange, 100, 200, 300, 50, "Egalite !");
    else if (player == 1) drawButton(renderer, orange, 100, 200, 300, 50, "Vous avez gagne !");
    else drawButton(renderer, orange, 100, 200, 300, 50, "Vous avez perdu !");
    SDL_RenderPresent(renderer);

  } else if (mode == 2) { // 1v1
    for (tour = 0; tour < tour_max && !gagnant(jeu, player, N); tour++) {
      player = tour % 2;
      do {
        waitToPlay(N, renderer, player, event, & i, & j);
        rejouer = jouer(jeu, i, j, player, N);
      } while (rejouer);
      drawGrille(N, renderer, jeu);
    }
    if (gagnant(jeu, player, N) == -1) drawButton(renderer, orange, 100, 200, 300, 50, "Egalite !");
    else if (gagnant(jeu, 0, N) == 1) drawButton(renderer, orange, 100, 200, 300, 50, "Le joueur des carres a gagne !");
    else drawButton(renderer, orange, 100, 200, 300, 50, "Le joueur des croix a gagne !");
    SDL_RenderPresent(renderer);
  }

  SDL_Delay(3000);

  TTF_CloseFont(police);
  TTF_Quit();
  SDL_Quit();

}