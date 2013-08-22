//
//  Squares.h
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#ifndef __ConsoleMonome__Squares__
#define __ConsoleMonome__Squares__

#include "MonomeTimerActivityIncludes.h"

class MonomeSwirlySquares : public MonomeTimerActivity
{
public:
    MonomeSwirlySquares(monome_t* monome, chrono::milliseconds timer) : MonomeTimerActivity(monome, timer){}
    
    ~MonomeSwirlySquares(){}
    
    void threadLogic (MonomeTimerActivity* activity)
    {
        MonomeSwirlySquares* squares = static_cast<MonomeSwirlySquares*>(activity);
        
        monome_led_all(squares->monome, 0);
        
        //first
        if (squares->first)
        {
            uint8_t half = 60;
            monome_led_col(squares->monome, 2, 0, 1, &half);
            monome_led_col(squares->monome, 5, 0, 1, &half);
            monome_led_row(squares->monome, 0, 2, 1, &half);
            monome_led_row(squares->monome, 0, 5, 1, &half);
            
            uint8_t all = 255;
            monome_led_col(squares->monome, 0, 0, 1, &all);
            monome_led_col(squares->monome, 7, 0, 1, &all);
            monome_led_row(squares->monome, 0, 0, 1, &all);
            monome_led_row(squares->monome, 0, 7, 1, &all);
        }
        else
        {
            uint8_t half = 24;
            monome_led_col(squares->monome, 3, 0, 1, &half);
            monome_led_col(squares->monome, 4, 0, 1, &half);
            monome_led_row(squares->monome, 0, 3, 1, &half);
            monome_led_row(squares->monome, 0, 4, 1, &half);
            
            uint8_t all = 126;
            monome_led_col(squares->monome, 1, 0, 1, &all);
            monome_led_col(squares->monome, 6, 0, 1, &all);
            monome_led_row(squares->monome, 0, 1, 1, &all);
            monome_led_row(squares->monome, 0, 6, 1, &all);
        }
        
        squares->first = !squares->first;
    }
private:
    bool first;
};


#endif /* defined(__ConsoleMonome__Squares__) */
