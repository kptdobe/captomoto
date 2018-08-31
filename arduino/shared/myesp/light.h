#ifndef MyLight_h
#define MyLight_h

#include "debug.h"

#define ON LOW
#define OFF HIGH

void setLight(short pin, bool light)
{
    if (light)
    {
        // turn the light on
        println("Setting light ON");
        digitalWrite(pin, ON);
    }
    else
    {
        // turn the light off
        println("Setting light OFF");
        digitalWrite(pin, OFF);
    }
}

#endif