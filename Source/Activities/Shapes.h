//
//  Shapes.h
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#ifndef ConsoleMonome_Shapes_h
#define ConsoleMonome_Shapes_h

#include "MonomeTimerActivityIncludes.h"
#include <vector>

class MonomeShapes : public MonomeTimerActivity
{
public:
    MonomeShapes (monome_t* monome, chrono::milliseconds timer = chrono::milliseconds(500))
    : MonomeTimerActivity (monome, timer)
    { additionalUserInput("up arrow"); /*Init with a smile*/}
    
    ~MonomeShapes(){}
    
    void threadLogic (MonomeTimerActivity* activity)
    {
        MonomeShapes* faces = static_cast<MonomeShapes*>(activity);
        
            for (int i = 0; i < 8; i++)
                monome_led_col(faces->monome, i, 0, 1, &currentShape[i]);
    }

    void additionalUserInput (string input)
    {
        if (input == "happy")
        {
            currentShape = {16,32,71,128,128,71,32,16};
        }
        else if (input == "sad")
        {
            currentShape = {135,71,39,16,16,39,71,135};
        }
        else if (input == "dead")
        {
            currentShape = {133,66,37,16,16,37,66,133};
        }
        else if (input.find("down arrow") != string::npos)
        {
            if (input.find ("full") == string::npos)
                currentShape = {16,48,95,129,129,95,48,16};
            else
                currentShape = {16,48,127,255,255,127,48,16};
        }
        else if (input.find("up arrow") != string::npos)
        {
            if (input.find ("full") == string::npos)
                currentShape = {8,12,250,129,129,250,12,8};
            else
                currentShape = {8,12,254,255,255,254,12,8};
        }
    }
    
private:
    vector<uint8_t> currentShape;
};


#endif
