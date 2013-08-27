//
//  MonomeTimerActivity.h
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#ifndef ConsoleMonome_MonomeTimerActivity_h
#define ConsoleMonome_MonomeTimerActivity_h

#include <monome.h>

#include <string>
#include <iostream>
#include <bitset>
#include <chrono>
#include <thread>

using namespace std;

class MonomeTimerActivity
{
public:
    MonomeTimerActivity(monome_t* mon, chrono::milliseconds timer = chrono::milliseconds(500))
    {
        monome = mon;
        activityRun = false;
        monome_led_all(monome, 0);
        
        timerDelay = timer;
    }
    
    virtual ~MonomeTimerActivity() {}
    
    
    void runActivity()
    {
        class thread activity(timerLogic, this);
        
        activityRun = true;
        user();
        
        activity.join();
    }
    
    static void timerLogic(MonomeTimerActivity* activity)
    {
        while (activity->activityRun)
        {
            activity->threadLogic(activity);
            
            this_thread::sleep_for(activity->timerDelay);
        }
    }

    
    void user()
    {
        while (true)
        {
            cout << "q to exit" << endl;
            
            string input;
            getline(cin, input);
            
            if (input != "q")
            {
                if (!input.empty())
                {
                    if (input.find("in ") != string::npos)
                    {
                    int intensity = 0;
                    
                    if (!input.empty())
                        intensity = atoi(&input.at(input.find_last_of(" ")+1));
                    
                    if (intensity > -1 && intensity < 16)
                        monome_led_intensity(monome, intensity);
                    }
                    else if (input.find("time") != string::npos)
                    {
                        int timer = 500;
                        
                        if (!input.empty())
                            timer = atoi(&input.at(input.find_last_of(" ")+1));
                        
                        timerDelay = chrono::milliseconds(timer);
                    }
                    else
                    {
                        additionalUserInput (input);
                    }
                }
            }
            else
            {
                activityRun = false;
                monome_led_all(monome, 0);
                return;
            }
        }
    }
    
    virtual void additionalUserInput (string input) {}
    virtual void buttonDown (const monome_event_t* event) {}
    
    monome_t* monome;
    bool activityRun;
    thread* thread;
    chrono::milliseconds timerDelay;

    virtual void threadLogic(MonomeTimerActivity* activity) = 0;
    /*{
        monome_led_all(activity->monome, 0);
        
        chrono::system_clock::time_point now = chrono::system_clock::now();
        chrono::system_clock::duration tp = now.time_since_epoch();
        
        typedef chrono::duration<int, ratio_multiply<chrono::hours::period, ratio<24> >::type> days;
        
        days d = chrono::duration_cast<days>(tp);
        tp -= d;
        chrono::hours h = chrono::duration_cast<chrono::hours>(tp);
        tp -= h;
        chrono::minutes m = chrono::duration_cast<chrono::minutes>(tp);
        tp -= m;
        chrono::seconds s = chrono::duration_cast<chrono::seconds>(tp);
        tp -= s;
        
        bitset<8> seconds (s.count());
        bitset<8> minutes (m.count());
        bitset<8> hours (h.count());
        
        for (int i = 0; i < 8; i++)
        {
            if (hours.test(i))
                monome_led_on(activity->monome, i, 0);
                
            if (minutes.test(i))
                monome_led_on(activity->monome, i, 2);
                    
            if (seconds.test(i))
                monome_led_on(activity->monome, i, 4);
        }
    }*/

};


#endif
