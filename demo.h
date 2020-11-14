/*
 *  demo.h
 *  visuals
 *
 *  Created by Bruno Keymolen on 03/12/10.
 *  Copyright 2010 Bruno Keymolen. All rights reserved.
 *
 */
#ifndef KOEN_DEMO_H
#define KOEN_DEMO_H

#include <stdio.h>
#include "drawtools.h"

namespace koen{
    
    class CDemo : CDrawToolsCallback {
    public:
        CDemo();
        virtual ~CDemo();
        
        int run();
        
        //CDrawToolsCallback
        int draw();
        
    private:
        CDrawTools *m_dt;
    };
    
}

#endif

