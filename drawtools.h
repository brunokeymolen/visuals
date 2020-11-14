/*
 *  drawtools.h
 *  visuals
 *
 *  Created by Bruno Keymolen on 03/12/10.
 *  Copyright 2010 Bruno Keymolen. All rights reserved.
 *
 */

#ifndef KOEN_DRAWTOOLS_H
#define KOEN_DRAWTOOLS_H

#include <stdio.h>

#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

namespace koen{

    struct CDrawToolsCallback {
        virtual int draw() = 0;
    };
    
    class CDrawTools{
    public:
        static CDrawTools* Instance(double w, double h, double d);
    public:
        ~CDrawTools();
        
        void mainDraw(GLuint *polyList);
        CDrawToolsCallback* setCallbacks(CDrawToolsCallback* c);
        void run();
        
        void line(double x0, double y0, double z0, double x1, double y1, double z1);
        void text(double x0, double y0, double z0, const char* text);
        void color(float r, float g, float b);

    private:
        CDrawTools(double x, double y, double z);
        CDrawToolsCallback* m_callback;
        GLuint m_polyList;
        double m_w;
        double m_h;
        double m_d;        
    private:
        static CDrawTools* _instance;        
    };

}


#endif
