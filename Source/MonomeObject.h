//
//  MonomeObject.h
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#ifndef __ConsoleMonome__MonomeObject__
#define __ConsoleMonome__MonomeObject__

#include <monome.h>

#include <bitset>
#include <chrono>
#include <thread>
#include <string>
#include <vector>

#include "MonomeUtility.h"

using namespace std;

class MonomeObject
{
public:
    MonomeObject();
    ~MonomeObject();
    
    void loadBitset(bitset<8> bitset, int rowCol, bool across);
    void buttonDown(const monome_event_t* event);
    
    static void down (const monome_event_t *event, void *data)
    {
        MonomeObject* mon = static_cast<MonomeObject*>(data);
        mon->buttonDown(event);
    }
    
private:
    void getUserInput();
    bool connectMonome();
    void menu();
    
    monome_t* monome;
    bitset<8> grid[8];
    bool stopTimer, registerPress;
    
};

#endif /* defined(__ConsoleMonome__MonomeObject__) */
