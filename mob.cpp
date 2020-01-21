#include "bot.hpp"
#include <stdlib.h>
#include <stdio.h>

void *startAB(void *arg) {
  printf("bobbin+++++++++++++++++++++++++++++++++/n\n");
  return NULL;
}

Mob::Mob(World *world) {
  
  this->world = world;
  /*
  if ( pthread_create( &thread, NULL, startAB, NULL) != 0) {
    printf("Couldn't create thread for mob\n");
    exit(0);
  }
  pthread_join( thread, NULL);
  */
}
