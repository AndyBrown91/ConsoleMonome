//
//  DrumMachine.h
//  ConsoleMonome
//
//  Created by Andy on 26/08/2013.
//
//

#ifndef ConsoleMonome_DrumMachine_h
#define ConsoleMonome_DrumMachine_h

#include "MonomeTimerActivityIncludes.h"

#include <fmod/api/inc/fmod.hpp>
#include <fmod/api/inc/fmod_errors.h>

class MonomeDrumMachine : public MonomeTimerActivity
{
public:
    MonomeDrumMachine(monome_t* mon, chrono::milliseconds timer = chrono::milliseconds(60000/120)) : MonomeTimerActivity(mon, timer)
    {
        FMOD::System_Create(&system);
        ERRCHECK(system->init(32, FMOD_INIT_NORMAL, NULL));
        
        beat = 0;
        currentPattern = {0,0,0,0,0,0,0,0};
        filePaths = {"/snare.wav", "/kick.wav","","","","","",""};
    }
    
    ~MonomeDrumMachine(){ ERRCHECK(system->close()); ERRCHECK(system->release());}
    
    void ERRCHECK(FMOD_RESULT result)
    {
        if (result != FMOD_OK)
        {
            printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
            exit(-1);
        }
    }
    
    void threadLogic (MonomeTimerActivity* activity)
    {
        //Vector of vectors, each represents a bar of 8th, allow the user to change between 16th and 8ths
        MonomeDrumMachine* drum = static_cast<MonomeDrumMachine*>(activity);
        
        //Sets previous column to saved pattern
        if (beat-1 >= 0)
            monome_led_col(monome, beat-1, 0, 1, &currentPattern[beat-1]);
        
        //Resets bar
        if (beat >= 8)
            beat = 0;
        
        //Play sounds here
        bitset<8> column = currentPattern[beat];
        for (int i = 0; i < 8; i++)
        {
            if (column.test(i) == true && filePaths.at(i).empty() == false)
            {
                FMOD::Channel* channel = 0;
                FMOD::Sound* sound;
                
                ERRCHECK(system->createSound(filePaths.at(i).c_str(), FMOD_2D, 0, &sound));
                ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, sound, 0, &channel));
            }
            
            system->update();
        }
        
        uint8_t all = 255;
        monome_led_col(monome, beat, 0, 1, &all);
        
        beat++;
        
        
    }
    
    void additionalUserInput (string input)
    {
        if (input.find("tempo") != string::npos)
        {
            double tempo;
            
            if (!input.empty())
                tempo = atoi(&input.at(input.find_last_of(" ")+1));
            
            timerDelay = chrono::milliseconds(long(60000/tempo));
        }
    }
    
    void buttonDown (const monome_event_t* event)
    {
        bitset<8> column(currentPattern[event->grid.x]);
        
        column.flip(event->grid.y);
        
        currentPattern[event->grid.x] = column.to_ullong();
    }
private:
    int beat, bar;
    
    FMOD::System* system;
    
    vector<uint8_t> currentPattern;
    vector<string> filePaths;
};

#endif
