/*
Author: William Redenbaugh
Last Edit Date: 12/19/2020
Notes:
Cycles through the mqtt subscriptions and figures out which ones have new information implamentation.
*/

#include "mqtt_task.h"

void MQTTReadTask::begin(Adafruit_MQTT_Client *mqtt, Adafruit_MQTT_Subscribe *sub){
  // Sets up the semaphore to deal with task synchronization and 
  // resource usage. 
  this->mqtt_read_mutex = xSemaphoreCreateMutex();
  
  this->mqtt = mqtt; 
  subscriptions.push_back(sub);
  subscriptions_status.push_back(0);
}

void MQTTReadTask::connect_mqtt(void){
  if(this->mqtt->connected())
    return;   

  // Variables used for re attempting connection.
  int8_t ret;
  uint8_t retries = 9;
  
  // So long as we don't have a connection, we will retry until we count down to zero. 
  while((ret = this->mqtt->connect()) != 0){
   Serial.println(this->mqtt->connectErrorString(ret));
   Serial.println("Retrying MQTT connection in a bit");
   this->mqtt->disconnect();
   delay(1000); 
   retries--;
   if (retries == 0) {
     while (1);
   }   
  }
}

void MQTTReadTask::task(void){

  // example_subscriptions 
  Adafruit_MQTT_Subscribe *subscription;
  
  // Ensures we have a connection to the mqtt client
  this->connect_mqtt();

  while(subscription = mqtt->readSubscription(this->delay_ms)){
    // Reading through our list of subscriptions 
    for(uint16_t i = 0; i < subscriptions.size(); i++){
      if(subscriptions[i] == subscription){   
		this->subscriptions_status[i] = 1; 
      }
    }
    delay(5);
  } 
}

uint8_t MQTTReadTask::get_subscription_status(uint16_t i){
  // If index is out of bounds. 
  if(i > this->subscriptions_status.size() - 1){
    return 0;   
  }
  uint8_t latest_sub_status = this->subscriptions_status[i];
  this->subscriptions_status[i] = 0; 
  return latest_sub_status;
}

void MQTTReadTask::use_mqtt(void){
  xSemaphoreTake( this->mqtt_read_mutex, portMAX_DELAY );
}

void  MQTTReadTask::release_mqtt(void){
  xSemaphoreGive(this->mqtt_read_mutex); 
}
