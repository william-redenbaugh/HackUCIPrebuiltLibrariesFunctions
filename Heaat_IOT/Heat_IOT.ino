/*
Author: William Redenbaugh
Last Edit Date: 2/1/2020
Notes:
Just a quick sketch to have an RGB light in our project
*/

#include "wifi_setup.h"
#include "mqtt_task.h"
#include "RGB_control.h"
#include "fan.h"

// RGB control and fan stuff. 
RGBControl led;
Fan fan;

// ADAFRUIT IO VARIABLES BEGIN // 
// Communication information
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                  
#define AIO_USERNAME    "wredenba"
#define AIO_KEY         "aio_VOvt36DtLob6R6509gy6dP4VYMQV"

// Objects to deal with actual connection and mqtt stuff. 
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);       
Adafruit_MQTT_Subscribe HeaatControl = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/HeaatControlSub");
Adafruit_MQTT_Publish HeaatStatus = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/HeaatStatusPub");

// Task that will help with background reading of mqtt stuff. 
MQTTReadTask mqtt_read_task; 
// ADAFRUIT IO VARIABLES END //

void setup_hardware(void){
  fan.begin(GPIO_NUM_5, GPIO_NUM_16, GPIO_NUM_19);
  led.begin(GPIO_NUM_14, GPIO_NUM_22, GPIO_NUM_23);
  led.set_brightness(10);
  led.fade(40, 40, 40, 1000);
}

void setup_mqtt(void){
  // Setting up connected stuff. 
  // MQTT Stuff. 
  mqtt.subscribe(&HeaatControl);
  mqtt_read_task.begin(&mqtt, &HeaatControl);
}

void setup() {
  setup_hardware();
  setup_mqtt();

  xTaskCreate(cloud_task, "Cloud Task", 20000, NULL, 40, NULL);  
  xTaskCreate(led_task, "Bidet Task", 20000, NULL, 40, NULL);
}

void loop() {
  vTaskDelete(NULL);
}
