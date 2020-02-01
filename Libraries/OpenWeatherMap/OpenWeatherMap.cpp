/*
Author: William Redenbaugh
Last Edit Date: 12/19/2020
Notes:
Reads weather data from  openweathermap with specified key implamentation
*/
#include "OpenWeatherMap.h"

void OpenWeatherMapJsonParse::begin(String city_state, String key){
  this->city_state = city_state; 
  this->security_key = key; 

  this->http_req = "http://api.openweathermap.org/data/2.5/weather?q=" + this->city_state + "&APPID=" + this->security_key;

  this->get();
}

void OpenWeatherMapJsonParse::change_place(String city_state){
  this->city_state = city_state; 
  this->http_req = "http://api.openweathermap.org/data/2.5/weather?q=" + this->city_state + "&APPID=" + this->security_key;
}

uint8_t OpenWeatherMapJsonParse::get(void){
  if ((WiFi.status() == WL_CONNECTED)) {
    this->open_weather_http.begin(this->http_req);
    int httpCode = open_weather_http.GET();  //Make the request
    if (httpCode > 0) {
      // Parsing out the string data from the http request
      String payload = open_weather_http.getString();
      // Free the resources
      open_weather_http.end(); 
          
      // Then parsing out the weather information from that string data. 
      DeserializationError error = deserializeJson(weather_doc, payload);

      // If there was a json parsing issue. 
      if(error)
        return OPEN_WEATHER_MAP_JSON_PARSE_FAILED;

      // Getting the actual tempurature data. 

      // Converting from kelvin to Farenheit. 
      this->tempurature_f = (weather_doc["main"]["temp"]);
      this->tempurature_f = (this->tempurature_f - 273.15) * 1.8 +32;

      // Perceived Tempurature in Farenheit 
      this->perceived_tempurature_f = weather_doc["main"]["feels_like"];
      this->perceived_tempurature_f = (this->perceived_tempurature_f - 273.15) * 1.8 + 32;
      
      this->sunset = weather_doc["sys"]["sunset"];
      this->sunrise = weather_doc["sys"]["sunrise"];
      this->humidity = weather_doc["main"]["humidity"];
    }
    else{
      // Free the resources, even if it failed anyway. 
      open_weather_http.end(); 
      
      return OPEN_WEATHER_MAP_REQUEST_FAILED;
    }
  }

  // WiFi wasn't connected. 
  else 
    return OPEN_WEATHER_MAP_WIFI_FAILED;   
}
