#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include "world.hpp"
#define WIDTH 640
#define HEIGHT 480
#define DEPTH 32


void handleKeyUpEvent(SDL_Event *event) {
  switch(event->key.keysym.sym) {
    case SDLK_ESCAPE:
      exit(0);
      break;
    default:
      break;
  }
}

void handleKeyDownEvent(SDL_Event *event) {
    //
}

void handleKeyEvent(SDL_Event *event) {
  while(SDL_PollEvent(event)) {
    switch (event->type) 
    {
      case SDL_QUIT:
        break;
      case SDL_KEYDOWN:
        return handleKeyDownEvent(event);
        break;  
      case SDL_KEYUP:
        return handleKeyUpEvent(event);
        break;
    }
  }
}

int main() {
    SDL_Surface *screen;
    SDL_Event event;
    

    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) exit(EXIT_FAILURE);
   
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_RESIZABLE|SDL_HWSURFACE)))
    {
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    // int animBankLen;
    // Anim **animBank = loadAnims(screen, &animBankLen);

    // if(!animBank) {
    //     SDL_Quit();
    //     fprintf(stderr, "Couldn't load animation\n");
    //     exit(EXIT_FAILURE);
    // }

    

    World *w = new World(256, 256);
    
    while(true) {
      updateScreen(screen);
      w->draw(screen);
      handleKeyEvent(&event);
    }
}
