/*
 *  demo.cpp
 *  visuals
 *
 *  Created by Bruno Keymolen on 03/12/10.
 *  Copyright 2010 Bruno Keymolen. All rights reserved.
 *
 */

#include "demo.h"
#include "math.h"

#define screen_w 1200
#define screen_h 600

namespace koen {

    CDemo::CDemo():m_dt(NULL){
    }
    
    CDemo::~CDemo(){
    }
    
    int CDemo::run(){
        m_dt  = CDrawTools::Instance(screen_w, screen_h, 100);
        m_dt->setCallbacks(dynamic_cast<CDrawToolsCallback*> (this));
        m_dt->run();
        return 0;
    }


    int CDemo::draw(){

        m_dt->color(1.0f, 0.0f, 0.0f);        
        m_dt->line(-5,0,0, 5,0,0); //x
        
        m_dt->color(0.0f, 1.0f, 0.0f);        
        m_dt->line(0,-5,0, 0,5,0); //y

        m_dt->color(0.0f, 0.0f, 1.0f);        
        m_dt->line(0,0,-5, 0,0,5); //z
        
        m_dt->color(0.0f, 5.0f, 1.0f);
        //x
        m_dt->text(-5,0,0, "-5(x)");
        m_dt->text(5,0,0, "5(x)");

        //y
        m_dt->text(0,-5,0, "-5(y)");
        m_dt->text(0,5,0,  "5(y)");

        //z
        m_dt->text(0,0,-5, "-5(z)");
        m_dt->text(0,0,5, "5(z)");

        
        m_dt->color(1.0f, 1.0f, 0.0f);
        for(double a=0.0;a<2.0*M_PI;a+=((2.0*M_PI)/100.0)){
            double y = sin(a) * 5.0;
            m_dt->line( 5.0-((a/(2.0*M_PI)) * 10),0.5,0,  5.0-((a/(2.0*M_PI)) * 10),y,0);
        }
        
        return 0;
    }

}
