//
//  Swirl.h
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#ifndef ConsoleMonome_Swirl_h
#define ConsoleMonome_Swirl_h

#include "MonomeTimerActivityIncludes.h"

class MonomeSwirl : public MonomeTimerActivity
{
public:
    MonomeSwirl(monome_t* monome, chrono::milliseconds timer = chrono::milliseconds(500)) : MonomeTimerActivity(monome, timer){}
    ~MonomeSwirl(){}
    
    void threadLogic (MonomeTimerActivity* activity)
    {
        MonomeSwirl* swirl = static_cast<MonomeSwirl*>(activity);
        
        monome_led_all(swirl->monome, 1);
    }
};


#endif
