/*
Author: William Redenbaugh
Last Edit Date: 1/15/2020
Notes:
Main Matrix Stock Clock file 
*/

#include "wifi_setup.h"
#include "mqtt_task.h"
#include "WiFiTimeModule.h"
#include "matrix_display_helper.h"
#include "OpenWeatherMap.h"
#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

// ADAFRUIT IO VARIABLES BEGIN // 
// Communication information
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "wredenba"
#define AIO_KEY         "aio_VOvt36DtLob6R6509gy6dP4VYMQV"

// Objects to deal with actual connection and mqtt stuff. 
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);       
Adafruit_MQTT_Subscribe Matrix_Control = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/MatrixControlSub");
Adafruit_MQTT_Publish MatrixControlPublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/MatrixControlPub");

// Task that will help with background reading of mqtt stuff. 
MQTTReadTask mqtt_read_task; 
// ADAFRUIT IO VARIABLES END //

// MATRIX VARIABLES BEGIN //
RGB64x32MatrixPanel_I2S_DMA dma_display;
MatrixHelper matrix_helper; 
// MATRIX VARIABLES END //

// TIME VARIABLES BEGIN // 
TimeModule ntp_time_module; 
// TIME VARIABLES END // 

// WEATHER VARIABLES BEGIN //
const String place = "Irvine, US";
const String key = "6768de66d4aade4436d0fa4be8581c3e";
OpenWeatherMapJsonParse weather; 
// WEATHER VARIABLE END // 

// Booleans for which animations are enabled or disabled. 
bool show_time = true;
bool show_top_line_fill = true; 
bool show_temp = true; 
bool show_messages = true; 

uint16_t main_col = 0xFFFF; 
uint16_t secondary_col = 0xFFFF; 

// Counts up the seconds to the hour. 
volatile uint16_t seconds_to_minute = 0; 

void setup_matrix(void){
  dma_display.begin();
  // Will help us do animations. 
  matrix_helper.begin(&dma_display); 
  matrix_helper.show_message("No matter where you go, there you are :)", &main_col);
}

void setup() {

  // Setting up low level libraries and wifi.  
  Serial.begin(115200);
  setup_matrix();
  setup_wifi();
  
  // Setting up the ntp time module for California time. 
  // NTP Time stuff. 
  ntp_time_module.begin(-8);
  // Weather stuff.  
  weather.begin(place, key); 

  // Setting up connected stuff. 
  // MQTT Stuff. 
  mqtt.subscribe(&Matrix_Control);
  mqtt_read_task.begin(&mqtt, &Matrix_Control);
  
  xTaskCreate(networking_task, "Networking Task", 20000, NULL, 40, NULL);

  // MATRIX ANIMATION SETUP BEGIN // 
  // Gotta make an attempt at the weather a couple times. 
  weather.get();
  ntp_time_module.get();
  weather.get();
  ntp_time_module.get();
  weather.get();
  ntp_time_module.get();
  weather.get();
  ntp_time_module.get();
  
  matrix_helper.show_temp_colored(uint8_t(weather.tempurature_f));
  matrix_helper.show_time(ntp_time_module.hour, ntp_time_module.minute, seconds_to_minute, main_col);
  matrix_helper.show_date(ntp_time_module.month, ntp_time_module.day, main_col);
  // MATRIX ANIMATION SETUP END // 
  
  // Setting up our seconds counter. 
  seconds_to_minute = ntp_time_module.seccond;
  
  // Start the animations!
  xTaskCreate(matrix_animation_task, "Matrix Animation Task", 20000, NULL, 40, NULL);  
}

void loop() {
  vTaskDelete(NULL);
}
