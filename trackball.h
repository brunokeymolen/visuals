/*
 *  trackball.h
 *  abstractions
 *
 *  Created by Bruno Keymolen on 08/11/10.
 *  Copyright 2010 Bruno Keymolen. All rights reserved.
 *
 */


#ifndef __trackball_h__
#define __trackball_h__

#ifdef __cplusplus
extern "C" {
#endif
    
    void startTrackball (long x, long y, long originX, long originY, long width, long height);
    void rollToTrackball (long x, long y, float rot [4]); // rot is output rotation angle
    void addToRotationTrackball (float * dA, float * A);
    
#ifdef __cplusplus
}
#endif

#endif