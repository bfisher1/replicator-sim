#define ANIMS_NUM 2
#include "anim.hpp"
#include <stdlib.h>
#include "util.hpp"
#include <vector>

/**
    Loads an animation from the given file. Width and height pertain
    to the size of each frame in the animation sheet. Fps controls
    the number of frames played per second.

    @param fileName the name of the animation sprite sheet
    @param frames the number of frames in the animation
    @param width the width of the anim's frames
    @param height the height of the anim's frame
    @param fps the frames played each second by the anim
    @param screen the screen the anim is played on
    @param background the background of the spritesheet, this color
           is ignored when drawing the animation

    @return the created anim
*/
Anim *loadAnim(char *fileName, int frames, float fps, SDL_Surface *screen, Color *background) {
    Anim *anim = (Anim *) malloc(sizeof(Anim));
    anim->screen = screen;
    anim->frames = frames;
    anim->spriteSheet = loadPNG(fileName);
    if(anim->spriteSheet == NULL) {
        free(anim);
        return NULL;
    }
    anim->fps = fps;
    anim->flippedHoriz = false;
    anim->repeat = true;
    anim->background = background;
    anim->width = anim->spriteSheet->width;
    anim->height = anim->spriteSheet->height / anim->frames;
    return anim;
}

/**
    Frees an animation from memory.

    @param anim the anim being freed
*/
void freeAnim(Anim *anim) {
    freeImage(anim->spriteSheet);
    free(anim);
}

// todo: add scaling to load anim
// null backgrounds
// 0 fps?
map<string, Anim*> *loadAnims(SDL_Surface *screen) {

    map<string, Anim*> *anims = new map<string, Anim*>();
    vector<string> *names = getFileNamesInPath("./anims/");
    for(int i = 0; i < names->size(); i++) {
    if(stringEndsWith((*names)[i], ".png")) {
        string fileDir = string("./anims/") + string((*names)[i]);
        (*anims)[(*names)[i]] = loadAnim((char *) fileDir.data(), 1, 1, screen, NULL);
        }
    }
    return anims;
}

void freeAnims(Anim **anims, int len) {
    for(int i = 0; i < len; i++){
        freeAnim(anims[i]);
    }
    free(anims);
}

bool playAnim(Anim *anim, int x, int y, float angle, int *frame, clock_t *lastPlayed, bool flippedHoriz) {
    drawSubImage(anim->spriteSheet, anim->screen,
    x, y, 0, *frame * anim->height,
    anim->width, anim->height, anim->background,
    flippedHoriz, angle);

    if( (float) (clock() - *lastPlayed) / CLOCKS_PER_SEC >= (1.0 / anim->fps) ){
       *lastPlayed = clock();
       *frame += 1;
        if(*frame >= anim->frames) {
            *frame = 0;
            return true;
        }
    }
    return false;
}