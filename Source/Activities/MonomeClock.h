//
//  MonomeClock.h
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#ifndef __ConsoleMonome__MonomeClock__
#define __ConsoleMonome__MonomeClock__

#include "MonomeTimerActivityIncludes.h"

class MonomeClock : public MonomeTimerActivity
{
public:
    MonomeClock(monome_t* monome) : MonomeTimerActivity(monome)
    {}
    
    ~MonomeClock(){}
    
    void threadLogic (MonomeTimerActivity* activity)
    {
        if (activity->timerDelay != chrono::milliseconds(500))
            activity->timerDelay = chrono::milliseconds(500);
        
        MonomeClock* clock = static_cast<MonomeClock*>(activity);
        
        monome_led_all(clock->monome, 0);
        
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
                monome_led_on(clock->monome, i, 0);
            
            if (minutes.test(i))
                monome_led_on(clock->monome, i, 2);
            
            if (seconds.test(i))
                monome_led_on(clock->monome, i, 4);
        }
    }
private:
};

#endif /* defined(__ConsoleMonome__MonomeClock__) */
