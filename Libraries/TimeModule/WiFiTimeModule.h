#ifndef _WIFITIMEMODULE_H
#define _WIFITIMEMODULE_H

#include "Arduino.h"
#include <Time.h>
#include "time.h"
#include <WiFi.h>
#include <M5Stack.h>

class TimeModule{
  public:
    void begin(int timezone);
    void get(void);

    // the "safe" way that ensures we get time. 
    bool get_blocking(uint32_t timeout_ms);
    
    // TIME STRING Variables //
    uint8_t hour; 
    uint8_t minute; 
    uint8_t seccond;
    uint8_t day; 
    uint8_t month; 
    uint32_t year; 
    
  private: 
  
    // Time defines. Since we are in the west coast, we have the time set to -8  :)))
    int timezone = -8 * 3600;
    int dst = 0;
};

#endif 
