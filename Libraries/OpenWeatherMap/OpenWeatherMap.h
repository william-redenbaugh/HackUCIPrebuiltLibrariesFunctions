/*
Author: William Redenbaugh
Last Edit Date: 12/19/2020
Notes:
Reads weather data from  openweathermap with specified key class and definitions. 
*/

#ifndef _OPENWEATHERMAP_H
#define _OPENWEATHERMAP_H

#include "Arduino.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

enum OpenWeatherMapReturnState{
    OPEN_WEATHER_MAP_SUCCESS = 255, 
    OPEN_WEATHER_MAP_UNKNOWN_FAILIURE = 0,
    OPEN_WEATHER_MAP_WIFI_FAILED = 1,
    OPEN_WEATHER_MAP_REQUEST_FAILED = 2,
    OPEN_WEATHER_MAP_JSON_PARSE_FAILED = 3
};

class OpenWeatherMapJsonParse{
  public: 

    // Setup.
    void begin(String city_state, String key);

    // Changes the location
    void change_place(String city_state);

    // grabs the data. 
    uint8_t get(void);
    
    // Saved Weather Information
    volatile float tempurature_f = 0.0;
    volatile float perceived_tempurature_f = 0.0;  
    volatile uint64_t sunrise = 0; 
    volatile uint64_t sunset = 0; 
    volatile uint8_t humidity = 0;
    
  private: 
    // Json document containing weather info. 
    StaticJsonDocument<2000> weather_doc;
    HTTPClient open_weather_http; 

    // String that help with the http request. 
    String security_key; 
    String city_state; 
    String http_req;     
};

#endif 
