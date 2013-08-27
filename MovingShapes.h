//
//  MovingShapes.h
//  ConsoleMonome
//
//  Created by Andy on 24/08/2013.
//
//

#ifndef ConsoleMonome_MovingShapes_h
#define ConsoleMonome_MovingShapes_h

#include "MonomeTimerActivityIncludes.h"
#include <vector>
#include <math.h>

class MonomeMovingShapes : public MonomeTimerActivity
{
public:
    MonomeMovingShapes(monome_t* monome, chrono::milliseconds timer) : MonomeTimerActivity(monome, timer)
    {
        currentShape = {85,170,85,170,85,170,85,170};
        value = -1;
        flashing = false;
    }
    
    ~MonomeMovingShapes(){}
    
    void threadLogic (MonomeTimerActivity* activity)
    {
        MonomeMovingShapes* shapes = static_cast<MonomeMovingShapes*>(activity);
        
        if (flashing)
        {
            if (checkPattern)
            {
                currentShape = {85,170,85,170,85,170,85,170};
                checkPattern = false;
            }
            else
            {
                currentShape = {170,85,170,85,170,85,170,85};
                checkPattern = true;
            }
            
            for (int i = 0; i < 8; i++)
                monome_led_col(shapes->monome, i, 0, 1, &currentShape[i]);
        }
        
        else if (other)
        {
            if (direction)
            {
                if (++value < 8)
                {
                    for (int i = 0; i < 8; ++i)
                    {
                        int other = value-i;
                        
                        if (other > -1)
                            monome_led_on(monome, other, i);
                    }
                }
                else
                {
                    direction = false;
                    monome_led_all(monome, 0);
                }
            }
            else
            {
                if (--value < -1)
                {
                    for (int i = 0; i < 8; ++i)
                    {
                        int other = value-i;
                        
                        if (other > -1)
                            monome_led_on(monome, other, i);
                    }
                }
                else
                    direction = true;
            }
            
        }
        
       
    }
    
    void additionalUserInput (string input)
    {
        if (input == "flashing check")
        {
            flashing = true;
            other = false;
        }
        else if (input == "other")
        {
            flashing = false;
            other = true;
        }
    }
    
private:
    vector <uint8_t> currentShape;
    bool flashing, other, direction, checkPattern;
    int value;
};

#endif
