/*
 *  visualcore.h
 *  abstractions
 *
 *  Created by Bruno Keymolen on 08/11/10.
 *  Copyright 2010 Bruno Keymolen. All rights reserved.
 *
 */

#ifdef __cplusplus
extern "C" {
#endif
    
#include <GL/gl.h>
    
int startVisual (int argc, const char * argv[], int w, int h, int d, void (*func)(void*, GLuint *polyList), void* );


void reshape (int w, int h);
void maindisplay(void);
void special(int key, int px, int py);
void mouseDolly (int x, int y);
void mousePan (int x, int y);
void mouseTrackball (int x, int y);
void mouse (int button, int state, int x, int y);
void keypress(unsigned char inkey, int px, int py);
void spaceballmotion (int x, int y, int z);
void spaceballrotate (int rx, int ry, int rz);

#ifdef __cplusplus
};
#endif



