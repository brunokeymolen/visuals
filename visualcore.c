/*
 *  visualcore.c
 *
 *  Created by Bruno Keymolen on 08/11/10.
 * 
 *  Code Reused from:
 *
 *
 *  glutBasics.cpp
 *  GLUTTest
 *
 *  Created by GGS on June 17 2003.
 *  Copyright (c) 2003 Apple. All rights reserved.
 *
 
 Disclaimer:	IMPORTANT:  This Apple software is supplied to you by Apple Computer, Inc.
 ("Apple") in consideration of your agreement to the following terms, and your
 use, installation, modification or redistribution of this Apple software
 constitutes acceptance of these terms.  If you do not agree with these terms,
 please do not use, install, modify or redistribute this Apple software.
 
 In consideration of your agreement to abide by the following terms, and subject
 to these terms, Apple grants you a personal, non-exclusive license, under Apple’s
 copyrights in this original Apple software (the "Apple Software"), to use,
 reproduce, modify and redistribute the Apple Software, with or without
 modifications, in source and/or binary forms; provided that if you redistribute
 the Apple Software in its entirety and without modifications, you must retain
 this notice and the following text and disclaimers in all such redistributions of
 the Apple Software.  Neither the name, trademarks, service marks or logos of
 Apple Computer, Inc. may be used to endorse or promote products derived from the
 Apple Software without specific prior written permission from Apple.  Except as
 expressly stated in this notice, no other rights or licenses, express or implied,
 are granted by Apple herein, including but not limited to any patent rights that
 may be infringed by your derivative works or by other works in which the Apple
 Software may be incorporated.
 
 The Apple Software is provided by Apple on an "AS IS" basis.  APPLE MAKES NO
 WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION THE IMPLIED
 WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND OPERATION ALONE OR IN
 COMBINATION WITH YOUR PRODUCTS.
 
 IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION, MODIFICATION AND/OR DISTRIBUTION
 OF THE APPLE SOFTWARE, HOWEVER CAUSED AND WHETHER UNDER THEORY OF CONTRACT, TORT
 (INCLUDING NEGLIGENCE), STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN
 ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 */

#include "visualcore.h"


#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>


#include "trackball.h"


void (*brainMainDraw)(void*, GLuint *polyList);
void* brainMainData;


#define DTOR 0.0174532925
#define CROSSPROD(p1,p2,p3) \
p3.x = p1.y*p2.z - p1.z*p2.y; \
p3.y = p1.z*p2.x - p1.x*p2.z; \
p3.z = p1.x*p2.y - p1.y*p2.x

#ifndef MIN
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#endif

enum {
	kTextureSize = 256
};

typedef struct {
    GLdouble x,y,z;
} recVec;

typedef struct {
	recVec viewPos; // View position
	recVec viewDir; // View direction vector
	recVec viewUp; // View up direction
	recVec rotPoint; // Point to rotate about
	GLdouble focalLength; // Focal Length along view direction
	GLdouble aperture; // gCamera aperture
	GLdouble eyeSep; // Eye separation
	GLint screenWidth,screenHeight; // current window/screen height and width
} recCamera;

GLfloat gShapeSize = 11.0f;

GLint gDollyPanStartPoint[2] = {0, 0};
GLfloat gTrackBallRotation [4] = {0.0, 0.0, 0.0, 0.0};
GLboolean gDolly = GL_FALSE;
GLboolean gPan = GL_FALSE;
GLboolean gTrackBall = GL_FALSE;

//GLfloat gWorldRotation [4] = {155.0, 0.0, -1.0, 0.0};
GLfloat gWorldRotation [4] = {0.0, 0.0, 0.0, 0.0};

int gLines = 0;
GLboolean gPolygons = GL_TRUE;

GLboolean gShowHelp = GL_TRUE;
GLboolean gShowInfo = GL_TRUE;

recCamera gCamera;
recVec gOrigin = {0.0, 0.0, 0.0};

int gLastKey = ' ';

int gMainWindow = 0;

GLuint gPointList = 0;
GLuint gWireList = 0;
GLuint gSolidList = 0;


void gCameraReset(void)
{
    gCamera.aperture = 40;
    gCamera.focalLength = 15;
    gCamera.rotPoint = gOrigin;
    
    gCamera.viewPos.x = 0.0;
    gCamera.viewPos.y = 0.0;
    gCamera.viewPos.z = -gCamera.focalLength;
    gCamera.viewDir.x = -gCamera.viewPos.x; 
    gCamera.viewDir.y = -gCamera.viewPos.y; 
    gCamera.viewDir.z = -gCamera.viewPos.z;
    
    gCamera.viewUp.x = 0;  
    gCamera.viewUp.y = 1; 
    gCamera.viewUp.z = 0;
}


void drawGLString3(GLfloat x, GLfloat y, GLfloat z, char *string)
{
    int len, i;
    
//    glRasterPos2f(x, y);
    glRasterPos3f(x, y, z);
    len = (int) strlen(string);
    for (i = 0; i < len; i++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, string[i]);
    }
}


void drawGLString(GLfloat x, GLfloat y, char *string){
    drawGLString3(x, y, 0, string);
}




void SetLighting(unsigned int mode)
{
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_shininess[] = {90.0};
    
	GLfloat position[4] = {7.0,-7.0,12.0,0.0};
	GLfloat ambient[4]  = {0.2,0.2,0.2,1.0};
	GLfloat diffuse[4]  = {1.0,1.0,1.0,1.0};
	GLfloat specular[4] = {1.0,1.0,1.0,1.0};
	
	glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv (GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    
	switch (mode) {
		case 0:
			break;
		case 1:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
			break;
		case 2:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
			break;
		case 3:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);
			break;
		case 4:
			glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
			glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_TRUE);
			break;
	}
	
	glLightfv(GL_LIGHT0,GL_POSITION,position);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specular);
	glEnable(GL_LIGHT0);
}

void drawGLText (GLint window_width, GLint window_height, GLuint *statics_list)
{
	char outString [256] = "";
	GLint matrixMode;
	GLint vp[4];
	GLint lineSpacing = 13;
	GLint line = 0;
	GLint startOffest = 7;
	
	glGetIntegerv(GL_VIEWPORT, vp);
	glViewport(0, 0, window_width, window_height);
	
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glScalef(2.0f / window_width, -2.0f / window_height, 1.0f);
	glTranslatef(-window_width / 2.0f, -window_height / 2.0f, 0.0f);
	
	// draw 
	glDisable(GL_LIGHTING);
	glColor3f (1.0, 1.0, 1.0);
	if (gShowInfo) {
        
		sprintf (outString, "Camera Position: (%0.1f, %0.1f, %0.1f)", gCamera.viewPos.x, gCamera.viewPos.y, gCamera.viewPos.z);
		drawGLString (10, window_height - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "Trackball Rotation: (%0.1f, %0.2f, %0.2f, %0.2f)", gTrackBallRotation[0], gTrackBallRotation[1], gTrackBallRotation[2], gTrackBallRotation[3]);
		drawGLString (10, window_height - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "World Rotation: (%0.1f, %0.2f, %0.2f, %0.2f)", gWorldRotation[0], gWorldRotation[1], gWorldRotation[2], gWorldRotation[3]);
		drawGLString (10, window_height - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "Aperture: %0.1f", gCamera.aperture);
		drawGLString (10, window_height - (lineSpacing * line++) - startOffest, outString);
		sprintf (outString, "Focus Distance: %0.1f", gCamera.focalLength);
		drawGLString (10, window_height - (lineSpacing * line++) - startOffest, outString);
	}
	
	if (gShowHelp) {
		line = 1;
		sprintf (outString, "Controls:\n");
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "left button drag: rotate camera\n");
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "right (or crtl-left) button drag: dolly (zoom) camera\n");
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "arrows: aperture & focal length\n");
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "3: toggle help\n");
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
		sprintf (outString, "2: toggle info\n");
		drawGLString (10, (lineSpacing * line++) + startOffest, outString);
	}
	

	
    //glTranslatef(-window_width / 2.0f, -window_height / 2.0f, 0.0f);
//    if(*statics_list != 0)
//        glCallList(*statics_list); 
    
    
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(matrixMode);
	
	glViewport(vp[0], vp[1], vp[2], vp[3]);
}



////////////
/*
void BuildWorld (GLuint * polyList, GLuint * lineList, GLuint * pointList, int colorScheme){
}

void BuildInfo (GLuint * polyList, GLuint * lineList, GLuint * pointList, int colorScheme){
}

void BuildMemory (GLuint * polyList, GLuint * lineList, GLuint * pointList, int colorScheme)
{
    int spheres = 10;
    float w = 3.0f; //until the middle, full size = w * 2
    float s = (w * 2.0f) / (float)spheres ;
    
    *polyList = glGenLists (1);
	glNewList(*polyList, GL_COMPILE);

    glShadeModel(GL_SMOOTH);
    glColor3f(0.3f,0.3f,1.0f);
    
    
    //Cells
    glTranslatef( -w, -w, -w);
    for(int x=0;x<spheres;x++){
        glTranslatef( s, 0.0f, 0.0f);
        for(int y=0;y<spheres;y++){
            glTranslatef( 0.0f, s, 0.0f);
            for(int z=0;z<spheres;z++){
                glTranslatef( 0.0f, 0.0f, s);
                glutSolidSphere(0.20f,10,10);            
            }
            glTranslatef( 0.0f, 0.0f, -s * (float)spheres);
        }
        glTranslatef( 0.0f, -s  * (float)spheres, 0.0f);
    }
    glTranslatef( -s * (float)spheres, 0.0f, 0.0f);
    
    
    //Transmitters
    
    
    
    glEndList ();
}

void BuildCube2 (GLuint * polyList, GLuint * lineList, GLuint * pointList, int colorScheme)
{
    GLint cube_num_vertices = 8;
    
    GLfloat cube_vertices [8][3] = {
        {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0},
        {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0} };
    
    GLfloat cube_vertex_colors [8][3] = {
        {1.0, 1.0, 1.0}, {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 1.0},
        {1.0, 0.0, 1.0}, {1.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 1.0} };
    
    GLint cube_num_faces = 6;
    
    short cube_faces [6][4] = {
        {3, 2, 1, 0}, {2, 3, 7, 6}, {0, 1, 5, 4}, {3, 0, 4, 7}, {1, 2, 6, 5}, {4, 5, 6, 7} };

    GLfloat cube_texCoords [2][4] = {
        {0.0, 0.0, 1.0, 1.0}, {0.0, 1.0, 1.0, 0.0} };
    
    
    
	float fSize = 2.0f;
	long f, i;
	*polyList = glGenLists (1);
	glNewList(*polyList, GL_COMPILE);

    
    glBegin (GL_QUADS);

    ////
    GLUquadricObj *quadratic;
    quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )
    gluSphere(quadratic,5.0f,10,10);	
    ////
    
    for (f = 0; f < cube_num_faces; f++)
        for (i = 0; i < 4; i++) {
            if (colorScheme)
                glColor3f (cube_vertex_colors[cube_faces[f][i]][0], cube_vertex_colors[cube_faces[f][i]][1], cube_vertex_colors[cube_faces[f][i]][2]);
            else
                glColor3f (1.0f, 1.0f, 1.0f);
            glTexCoord2f (cube_texCoords [0][i], cube_texCoords [1][i]);
            glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
        }
    glEnd ();
    glColor3f (0.0, 0.0, 0.0);
    for (f = 0; f < cube_num_faces; f++) {
        glBegin (GL_LINE_LOOP);
        for (i = 0; i < 4; i++)
            glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
        glEnd ();
    }
	glEndList ();
	
	*lineList = glGenLists (1);
	glNewList(*lineList, GL_COMPILE);
    glColor3f (1.0, 1.0, 1.0);
    for (f = 0; f < cube_num_faces; f++) {
        glBegin (GL_LINE_LOOP);
        for (i = 0; i < 4; i++)
            glVertex3f(cube_vertices[cube_faces[f][i]][0] * fSize, cube_vertices[cube_faces[f][i]][1] * fSize, cube_vertices[cube_faces[f][i]][2] * fSize);
        glEnd ();
    }
	glEndList ();
	
	*pointList = glGenLists (1);
	glNewList(*pointList, GL_COMPILE);
    glColor3f (1.0, 1.0, 1.0);
    for (f = 0; f < cube_num_vertices; f++) {
        glBegin (GL_POINTS);
        glVertex3f(cube_vertices[f][0] * fSize, cube_vertices[f][1] * fSize, cube_vertices[f][2] * fSize);
        glEnd ();
    }
	glEndList ();
}
*/
////////////


void init (void)
{
	glEnable(GL_DEPTH_TEST);
    
	glShadeModel(GL_SMOOTH);    
	glFrontFace(GL_CCW);
    
	glColor3f(1.0,1.0,1.0);
    
	
	glClearColor(0.0,0.0,0.0,0.0);         /* Background recColor */
	gCameraReset ();
	
	glPolygonOffset (1.0, 1.0);
	SetLighting(4);
	glEnable(GL_LIGHTING);


}

void reshape (int w, int h)
{
//	glViewport(0,0,(GLsizei)w*0.70f,(GLsizei)h);
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	gCamera.screenWidth = w;
	gCamera.screenHeight = h;
	glutPostRedisplay();
}

void maindisplay(void)
{
  
	GLdouble xmin, xmax, ymin, ymax;
	// far frustum plane
	GLdouble zFar = -gCamera.viewPos.z + gShapeSize * 0.5;
	// near frustum plane clamped at 1.0
	GLdouble zNear = MIN (-gCamera.viewPos.z - gShapeSize * 0.5, 1.0);
	// window aspect ratio
	GLdouble aspect = gCamera.screenWidth / (GLdouble)gCamera.screenHeight; 
    
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
    
	if (aspect > 1.0) {
		ymax = zNear * tan (gCamera.aperture * 0.5 * DTOR);
		ymin = -ymax;
		xmin = ymin * aspect;
		xmax = ymax * aspect;
	} else {
		xmax = zNear * tan (gCamera.aperture * 0.5 * DTOR);
		xmin = -xmax;
		ymin = xmin / aspect;
		ymax = xmax / aspect;
	}
	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
	
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	gluLookAt (gCamera.viewPos.x, gCamera.viewPos.y, gCamera.viewPos.z,
               gCamera.viewPos.x + gCamera.viewDir.x,
               gCamera.viewPos.y + gCamera.viewDir.y,
               gCamera.viewPos.z + gCamera.viewDir.z,
               gCamera.viewUp.x, gCamera.viewUp.y ,gCamera.viewUp.z);
    
	// track ball rotation
	glRotatef (gTrackBallRotation[0], gTrackBallRotation[1], gTrackBallRotation[2], gTrackBallRotation[3]);
	glRotatef (gWorldRotation[0], gWorldRotation[1], gWorldRotation[2], gWorldRotation[3]);
	
	glClearColor (0.2f, 0.2f, 0.4f, 1.0f);	// clear the surface
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    brainMainDraw(brainMainData, &gWireList, &gSolidList);//BRUNO

    
    //
    // Here we draw the lists
    //
    brainMainDraw(brainMainData, &gSolidList);
    gWireList = 0;
    
    glEnable(GL_LIGHTING);
    glCallList (gSolidList);
	
	drawGLText (gCamera.screenWidth, gCamera.screenHeight, &gWireList);
    
    
    ////
    GLUquadricObj *quadratic;
    quadratic=gluNewQuadric();			// Create A Pointer To The Quadric Object ( NEW )
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals ( NEW )
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords ( NEW )
  //  gluSphere(quadratic,2.5f,50,50);	
    
        
	glutSwapBuffers();
    
   
}

void special(int key, int px, int py)
{
    gLastKey = key;
    switch (key) {
        case GLUT_KEY_UP: // arrow forward, close in on world
            gCamera.focalLength -= 0.5f;
            if (gCamera.focalLength < 0.0f)
                gCamera.focalLength = 0.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // arrow back, back away from world
            gCamera.focalLength += 0.5f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // arrow left, smaller aperture
            gCamera.aperture -= 0.5f;
            if (gCamera.aperture < 0.0f)
                gCamera.aperture = 0.0f;
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // arrow right, larger aperture
            gCamera.aperture += 0.5f;
            glutPostRedisplay();
            break;
    }
}

void mouseDolly (int x, int y)
{
	if (gDolly) {
		GLfloat dolly = (gDollyPanStartPoint[1] - y) * -gCamera.viewPos.z / 200.0f;
		GLfloat eyeRelative = gCamera.eyeSep / gCamera.focalLength;
		gCamera.focalLength += gCamera.focalLength / gCamera.viewPos.z * dolly; 
		if (gCamera.focalLength < 1.0)
			gCamera.focalLength = 1.0;
		gCamera.eyeSep = gCamera.focalLength * eyeRelative;
		gCamera.viewPos.z += dolly;
		if (gCamera.viewPos.z == 0.0) // do not let z = 0.0
			gCamera.viewPos.z = 0.0001;
		gDollyPanStartPoint[0] = x;
		gDollyPanStartPoint[1] = y;
		glutPostRedisplay();
	}
}

void mousePan (int x, int y)
{
	if (gPan) {
		GLfloat panX = (gDollyPanStartPoint[0] - x) / (900.0f / -gCamera.viewPos.z);
		GLfloat panY = (gDollyPanStartPoint[1] - y) / (900.0f / -gCamera.viewPos.z);
		gCamera.viewPos.x -= panX;
		gCamera.viewPos.y -= panY;
		gDollyPanStartPoint[0] = x;
		gDollyPanStartPoint[1] = y;
		glutPostRedisplay();
	}
}

void mouseTrackball (int x, int y)
{
	if (gTrackBall) {
		rollToTrackball (x, y, gTrackBallRotation);
		glutPostRedisplay();
	}
}

void mouse (int button, int state, int x, int y)
{
	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)) {
		if (gDolly) { // if we are currently dollying, end dolly
			mouseDolly (x, y);
			gDolly = GL_FALSE;
			glutMotionFunc (NULL);
			gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
			glutMotionFunc (NULL);
		} else if (gPan) {
			mousePan (x, y);
			gPan = GL_FALSE;
			glutMotionFunc (NULL);
			gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
			glutMotionFunc (NULL);
		}
		startTrackball (x, y, 0, 0, gCamera.screenWidth, gCamera.screenHeight);
		glutMotionFunc (mouseTrackball);
		gTrackBall = GL_TRUE;
	} else if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_UP)) {
		gTrackBall = GL_FALSE;
		glutMotionFunc (NULL);
		rollToTrackball (x, y, gTrackBallRotation);
		if (gTrackBallRotation[0] != 0.0)
			addToRotationTrackball (gTrackBallRotation, gWorldRotation);
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
	}
	else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_DOWN)) {
		if (gTrackBall) {// if we are currently trackballing, end trackball
			gTrackBall = GL_FALSE;
			glutMotionFunc (NULL);
			rollToTrackball (x, y, gTrackBallRotation);
			if (gTrackBallRotation[0] != 0.0)
				addToRotationTrackball (gTrackBallRotation, gWorldRotation);
			gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
		} else if (gPan) {
			mousePan (x, y);
			gPan = GL_FALSE;
			glutMotionFunc (NULL);
			gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
			glutMotionFunc (NULL);
		}
		gDollyPanStartPoint[0] = x;
		gDollyPanStartPoint[1] = y;
		glutMotionFunc (mouseDolly);
		gDolly = GL_TRUE;
	} else if ((button == GLUT_RIGHT_BUTTON) && (state == GLUT_UP)) {
		mouseDolly (x, y);
		gDolly = GL_FALSE;
		glutMotionFunc (NULL);
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
		glutMotionFunc (NULL);
	}
	else if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_DOWN)) {
		if (gTrackBall) {// if we are currently trackballing, end trackball
			gTrackBall = GL_FALSE;
			glutMotionFunc (NULL);
			rollToTrackball (x, y, gTrackBallRotation);
			if (gTrackBallRotation[0] != 0.0)
				addToRotationTrackball (gTrackBallRotation, gWorldRotation);
			gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
		} else if (gDolly) {
			mouseDolly (x, y);
			gDolly = GL_FALSE;
			glutMotionFunc (NULL);
			gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
			glutMotionFunc (NULL);
		}
		gDollyPanStartPoint[0] = x;
		gDollyPanStartPoint[1] = y;
		glutMotionFunc (mousePan);
		gPan = GL_TRUE;
	} else if ((button == GLUT_MIDDLE_BUTTON) && (state == GLUT_UP)) {
		mousePan (x, y);
		gPan = GL_FALSE;
		glutMotionFunc (NULL);
		gTrackBallRotation [0] = gTrackBallRotation [1] = gTrackBallRotation [2] = gTrackBallRotation [3] = 0.0f;
		glutMotionFunc (NULL);
	}
}

void keypress(unsigned char inkey, int px, int py)
{
    gLastKey = inkey;
    switch (inkey) {
        case 27:
            exit(0);
            break;
        case '3': // help
            gShowHelp =  1 - gShowHelp;
            glutPostRedisplay();
            break;
        case '2': // info
            gShowInfo =  1 - gShowInfo;
            glutPostRedisplay();
            break;
        case '1': // toggle wire
            gLines = ++gLines>=3?0:gLines;
            gPolygons = gLines==1?0:1;//1 - gPolygons;
            glutPostRedisplay();
            break;
      }
}

void spaceballmotion (int x, int y, int z)
{
	long deadZone = 105;
	float scale = -gCamera.viewPos.z * 0.00000001f;
	if (abs (x) > deadZone) {
		GLfloat panX = abs (x) * x * scale;
		gCamera.viewPos.x += panX;
	}
	if (abs (y) > deadZone) {
		GLfloat panY = abs (y) * y * scale;
		gCamera.viewPos.y -= panY;
	}
	if (abs (z) > deadZone) {
		GLfloat dolly = abs (z) * z * scale;
		gCamera.viewPos.z += dolly;
		if (gCamera.viewPos.z == 0.0) // do not let z = 0.0
			gCamera.viewPos.z = 0.0001;
	}
	glutPostRedisplay();
}

void spaceballrotate (int rx, int ry, int rz)
{
	long deadZone = 60;
	float rotation[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	// handle rotations about each respective axis
	if (abs (rx) > deadZone) {
		rotation[0] = abs (rx) * -rx * 0.0000008f;
		rotation[1] = 1.0f;
		rotation[2] = 0.0f;
		rotation[3] = 0.0f;
		addToRotationTrackball (rotation, gWorldRotation);
	}
	if (abs (ry) > deadZone) {
		rotation[0] = abs (ry) * ry * 0.0000008f;
		rotation[1] = 0.0f;
		rotation[2] = 1.0f;
		rotation[3] = 0.0f;
		addToRotationTrackball (rotation, gWorldRotation);
	}
	if (abs(rz) > deadZone) {
		rotation[0] = abs (rz) * -rz * 0.0000008f;
		rotation[1] = 0.0f;
		rotation[2] = 0.0f;
		rotation[3] = 1.0f;
		addToRotationTrackball (rotation, gWorldRotation);
	}
	glutPostRedisplay();
}



int startVisual (int argc, const char * argv[], int w, int h, int d, void (*func)(void*, GLuint *polyList), void* data)
{
    brainMainData = data;
    brainMainDraw = func;
    
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // non-stereo for main window
	glutInitWindowPosition (30, 50);
	glutInitWindowSize (w, h);
	gMainWindow = glutCreateWindow("Koen's Playground");
    
    init(); // standard GL init
        
    glutReshapeFunc (reshape);
    glutDisplayFunc (maindisplay);
	glutKeyboardFunc (keypress);
	glutSpecialFunc (special);
	glutMouseFunc (mouse);
	glutSpaceballMotionFunc(spaceballmotion);
	glutSpaceballRotateFunc(spaceballrotate);
    glutMainLoop();
    return 0;
}





