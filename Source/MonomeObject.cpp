//
//  MonomeObject.cpp
//  ConsoleMonome
//
//  Created by Andy on 22/08/2013.
//
//

#include <math.h>
#include "MonomeObject.h"
#include <chrono>

#include "Squares.h"
#include "MonomeClock.h"
#include "Swirl.h"
#include "Shapes.h"

MonomeObject::MonomeObject()
{
    if(connectMonome())
    {
        monome_register_handler(monome, MONOME_BUTTON_DOWN, down, this);
        
        thread monomeListener(monome_event_loop, monome);
        
        menu();
        
        //Closes up everything about the thread
        monome_led_all(monome, 0);
        monome_unregister_handler(monome, MONOME_BUTTON_DOWN);
        monome_unregister_handler(monome, MONOME_BUTTON_UP);
        monome_close(monome);
        
        monomeListener.join();
    }
    else
        cout << "Failed to connect" << endl;
}
MonomeObject::~MonomeObject() {}

void MonomeObject::loadBitset (bitset<8> bitset, int rowCol, bool across)
{
    if (across)
    {
        for (int i = 0; i < 8; i++)
        {
            if (bitset.test(i))
            {
                grid[rowCol].set(i);
                monome_led_on(monome, rowCol, i);
            }
            else
            {
                grid[rowCol].reset(i);
                monome_led_off(monome, rowCol, i);
            }
            
        }
    }
}

void MonomeObject::buttonDown (const monome_event_t* event)
{
    if (registerPress)
    {
        int x = event->grid.x;
        int y = event->grid.y;

        if (grid[x].test(y))
            monome_led_off(monome, x, y);
        else
            monome_led_on(monome, x, y);
        
        grid[x].flip(y);
    }
}

void MonomeObject::menu()
{    
    while (true) {
        registerPress = true;
        
        cout << "Options: (q to quit)" << endl;
        cout << "1. Console Input" << endl;
        cout << "2. Binary Clock" << endl;
        cout << "3. Swirly Squares" << endl;
        cout << "4. Swirl" << endl;
        cout << "5. Shapes" << endl;
        
        char response;
        cin >> response;
        
        switch (response) {
            case '1':
                getUserInput();
                break;
            case '2':
            {
                registerPress = false;
                MonomeClock clock(monome);
                clock.runActivity();
                break;
            }
            case '3':
            {
                registerPress = false;
                MonomeSwirlySquares squares(monome, chrono::milliseconds(500));
                squares.runActivity();
                break;
            }
            case '4':
            {
                MonomeSwirl swirl (monome);
                swirl.runActivity();
                break;
            }
            case '5':
            {
                MonomeShapes shape (monome);
                shape.runActivity();
                break;
            }
            case 'q':
                return;
            default:
                break;
                
        }
    }
}

bool MonomeObject::connectMonome ()
{
    vector<StringPair> ports;
    MonomeUtility::getSerialPortPaths(ports);
    
    for (int i = 0; i < ports.size(); i++)
        cout << i+1 << ". " << ports[i].first << endl;
    
    cout << "Please select a port number " << endl;
    
    int port = 1;
    //REMEMBER TO UNCOMMENT
    //cin >> port;
    port--;
    
    if (port > -1 && port < ports.size())
        if ((monome = monome_open(ports[port].second.c_str())))
        {
            monome_led_all(monome, 1);
            this_thread::sleep_for(chrono::milliseconds(100));
            monome_led_all(monome, 0);
            return true;
        }
    
    return false;
    
}

void MonomeObject::getUserInput()
{
    while (true)
    {
        cout << "Enter a command" << endl;
        
        string input;
        getline(cin, input);

        int x, y;
        string command;
        
        if (!input.empty())
        {
            x = atoi(&input.at(input.find_first_of(" ")+1));
            y = atoi(&input.at(input.find_last_of(" ")+1));
            
            command = input.substr(0, input.find_first_of(" "));
        }

        if (command == "on")
        {
            if ((x > -1 && x < monome_get_cols(monome))  &&  (y > -1 && y < monome_get_rows(monome)))
            {
                monome_led_on(monome, x, y);
                grid[x].set(y);
            }
        }
        else if (command == "off")
        {
            if ((x > -1 && x < monome_get_cols(monome))  &&  (y > -1 && y < monome_get_rows(monome)))
            {
                monome_led_off(monome, x, y);
                grid[x].reset(y);
            }
        }
        
        else if (command == "all")
        {
            bool set = true;

            if (input.rfind("on") != string::npos)
                monome_led_all(monome, 1);
            else
            {
                monome_led_all(monome, 0);
                set = false;
            }
            
            for (int x = 0; x < monome_get_cols(monome); x++)
            {
                for (int y = 0; y < monome_get_rows(monome); y++)
                    if (set)
                        grid[x].set(y);
                    else
                        grid[x].reset(y);
            }
            
        }
        
        else if (command == "col" || command == "row")
        {
            if (command == "col")
            {
                uint8_t data = pow(2, y)-1;
                bitset<8> set (data);
                
                monome_led_col(monome, x, 0, 1, &data);
                grid[x] = set;
            }
            else
            {
                //Confusion abound, x now refers to the column (y) and y refers to the amount of lights to turn on
                int row = x;
                int number = y;
                
                uint8_t data = pow(2, number)-1;
                bitset<8> set (data);
                
                //row
                monome_led_row(monome, 0, row, 1, &data);
                for (int i = 0; i < 8; i++) {
                    if (set.test(row-1))
                        grid[i].set(row-1);
                    else
                        grid[i].reset(row-1);
                    
                }
            }
        }
        
        else if (command == "in" || command == "intensity")
        {
            int intensity = 0;
            intensity = atoi(&input.at(input.find_last_of(" ")+1));
            
            if (intensity > -1 && intensity < 16)
                monome_led_intensity(monome, intensity);
        }
        
        else if (command == "quit")
            return;
    }
}