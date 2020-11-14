/*
 *  drawtools.cpp
 *  visuals
 *
 *  Created by Bruno Keymolen on 03/12/10.
 *  Copyright 2010 Bruno Keymolen. All rights reserved.
 *
 */

#include <stdio.h>
#include "drawtools.h"
#include "visualcore.h"

namespace koen {

    CDrawTools* CDrawTools::_instance = NULL;

    extern "C" void drawGLString(GLfloat x, GLfloat y, char *string);
    extern "C" void drawGLString3(GLfloat x, GLfloat y, GLfloat z, char *string);

    extern "C" void _mainDraw(void* data, GLuint *polyList){
        CDrawTools* pthis = (CDrawTools*)data;
        pthis->mainDraw(polyList);
    }
    
    
    CDrawTools* CDrawTools::Instance(double x, double y, double z){
        if(_instance == NULL){
            _instance = new CDrawTools(x, y, z);
        }
        return _instance;
    }


    CDrawTools::CDrawTools(double w, double h, double d):m_callback(NULL), m_polyList(0), m_w(w), m_h(h), m_d(d){
    }
    
    void CDrawTools::run(){
        const char* opt = (const char*)"";         
        startVisual (0, &opt, m_w, m_h, m_d, &_mainDraw, (void*)this);
    }

    CDrawToolsCallback* CDrawTools::setCallbacks(CDrawToolsCallback* c){
        CDrawToolsCallback* old = m_callback;
        m_callback = c;
        return old;
    }

    void CDrawTools::mainDraw(GLuint *polyList){
        if(m_polyList)
            glDeleteLists(m_polyList, 1);
        
        m_polyList =  glGenLists (1);
        glNewList(m_polyList, GL_COMPILE);
        glShadeModel(GL_SMOOTH);
        glDisable(GL_LIGHTING);
        glPushMatrix();        

        
        if(m_callback){
            m_callback->draw();
        }
        
        
       
        glPopMatrix();              
        glEndList ();
        
        *polyList = m_polyList;
    }

    void CDrawTools::line(double x0, double y0, double z0, double x1, double y1, double z1){
               
        glBegin(GL_LINES);
        
        glVertex3f(x0, y0, z0);    // lower left vertex
        glVertex3f(x1, y1, z1);    // lower right vertex
        
        glEnd();
    }
    
    void CDrawTools::text(double x0, double y0, double z0, const char* text){
        drawGLString3(x0, y0, z0, (char*)text);
    }
    
    void CDrawTools::color(float r, float g, float b){
        glColor3f(r, g, b);
    }
    
    
}
