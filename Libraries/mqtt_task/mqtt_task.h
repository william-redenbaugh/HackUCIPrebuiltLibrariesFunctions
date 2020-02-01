/*
Author: William Redenbaugh
Last Edit Date: 12/19/2020
Notes:
Cycles through the mqtt subscriptions and figures out which ones have new information class and definitions
*/

#ifndef _MQTT_TASK_H
#define _MQTT_TASK_H

#include "Arduino.h"
#include "WiFi.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

class MQTTReadTask{
  public: 
    // Reads the mqtt client and adds the first subscription to a vector array
    void begin(Adafruit_MQTT_Client *mqtt, Adafruit_MQTT_Subscribe *sub);
    void attach_subs(Adafruit_MQTT_Subscribe *sub);
    
    uint8_t get_subscription_status(uint16_t i);

    // Allows us to also use the mutexes 
    // so threads can share the resources. 
    void use_mqtt(void);
    void release_mqtt(void);

    // Task that actually runs in a separate task.
    // Get's called repeatedly in intervals. 
    void task(void);
	
    // Delay value for intervals between reading the next task.
    uint32_t delay_ms = 100; 
    
  private:
    // A way to setup our mqtt client and ensure that it is connected. 
    void connect_mqtt(void);

    // Pointer reference to the mqtt client. 
    Adafruit_MQTT_Client *mqtt; 
    
    // Vector list of MQTT subscriptions. 
    std::vector<Adafruit_MQTT_Subscribe*> subscriptions; 
    std::vector<uint8_t> subscriptions_status; 
    // Mutex that allows us to run this 
    // wholes show. 
    SemaphoreHandle_t mqtt_read_mutex;
};

#endif 
