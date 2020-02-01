/*
Author: William Redenbaugh
Last Edit Date1 1/15/2020
Notes:
Cloud integration to talk to the mqtt broker.  
*/

void networking_task(void *parameters){
  uint16_t network_ticker = 0; 
  TickType_t last_network_time;

  for(;;){
    
    // Check latest tick!
    last_network_time = xTaskGetTickCount();
    
    // Since the mqtt module uses WiFi, 
    // and WiFi isn't a threadsafe module
    // we need to just make sure we do everything
    // a single thread.

    network_ticker++; 

    // Get time every 2.5 seconds. 
    if(network_ticker%5){
      ntp_time_module.get();  
    }

    // Every second, read mqtt stuff. 
    if(network_ticker&2){
      mqtt_read_task.task();  
    }

    // resets ticker counter every minute, also includes any minute periodic functions. 
    if(network_ticker == 120){
      // Get the weather every second. 
      weather.get();
      network_ticker = 0; 
    }

    // Half second tick total, allowing us to check for networking stuff twice a second at max speed. 
    TickType_t del_tick = pdMS_TO_TICKS(500);
    if(last_network_time + del_tick > xTaskGetTickCount()){
      vTaskDelayUntil(&last_network_time, del_tick); 
    }
  }  
  vTaskDelete(NULL);
}
