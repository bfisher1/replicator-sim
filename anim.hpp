/**
    @file anim.h
    @author Ben Fisher

    Defines enumerations for recalling animations, the structure
    of an animation, and functions for loading and playing
    animations.
*/
#ifndef ANIM_H
#define ANIM_H

#include <SDL/SDL.h>
#include "graphics.h"
#include <stdbool.h>
#include <time.h>
#include <map>
#include <string>
#include "direction.hpp"

using namespace std;

typedef enum {
    stoneAnim,
    waterAnim
} AnimName;

/**
    Structure for an animation. It contains a pointer to the screen
    it will be drawn on to simplify drawing functions. Also contains
    the number of frames, the image that makes up an animation, and
    the frames per second it is played at. Contains flags for if the
    animation is flipped horizontally or repeats endlessly. The
    background color is ignored when drawing the image.
*/
typedef struct {
    SDL_Surface *screen;
    int frames;
    Image *spriteSheet;
    float a;
    float fps;
    bool flippedHoriz;
    bool repeat;
    //clock_t lastPlayed;
    Color *background;
    int width;
    int height;
} Anim;

/**
    Loads the animations needed for the game. Allocates an array of
    animations that it saves the length of. Animations are stored at
    indexes pertaining to their enumeration.

    @param screen the screen the anims will belong to

    @return pointer to an array of animations
*/
map<string, Anim*> *loadAnims(SDL_Surface *screen);

/**
    Frees an allocated array of anims of length len.

    @param anims the anims being freed
    @param len the number of anims being freed
*/
void freeAnims(Anim **anims, int len);

/**
    Plays an animation, returning true if it has finished.

    @param anim the anim being played
    @param x the x coordinate where the anim's center is on its screen
    @param y the y coordinate where the anim's center is on its screen
    @param angle the angle the animation is played at
    @param frame the frame of the animation being played

    @return true if the animation has finished, false if not
*/
bool playAnim(Anim *anim, int x, int y, float angle, int *frame, clock_t *lastPlayed, bool flippedHoriz);
void drawStillFrame(Anim *anim, int x, int y, int frame, bool flippedHoriz);
void drawStillFrameRot(Anim *anim, int x, int y, int frame, bool flippedHoriz, double dir);
void setAnimScreen(SDL_Surface *screen);
Anim *getAnim(string name);
#endif /* ANIM_H */