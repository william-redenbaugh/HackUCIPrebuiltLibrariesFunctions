void led_task(void *parameters){
  TickType_t last_network_time;

  for(;;){
    // Check latest tick!
    last_network_time = xTaskGetTickCount();

    if(mqtt_read_task.get_subscription_status(0)){
      String str = (char*)HeaatControl.lastread;

      // Otherwise if we are getting a request for a specific color!
      if(HeaatControl.lastread[0] == 'c'){
        if(str.equals("cred")){
          led.fade(255, 0, 0, 800);
        }
        else if(str.equals("cgreen")){
          led.fade(0, 255, 0, 800);
        }
        else if(str.equals("cblue")){
          led.fade(0, 0, 255, 800);
        }
        else if(str.equals("crose")){
          led.fade(255, 0, 127, 800);
        }
        else if(str.equals("cpink")){
          led.fade(255, 182, 194, 800);
        }
        else if(str.equals("cpurple")){
          led.fade(240, 0, 240, 800);
        }
        else if(str.equals("ccyan")){
          led.fade(0, 255, 255, 800);
        }
        else if(str.equals("cwhite")){
          led.fade(250, 250, 250, 800);
        }
        else if(str.equals("cindigo")){
          led.fade(140, 0, 250, 800);
        }
        else if(str.equals("coff")){
          led.fade(0, 0, 0, 800);
        }
      }

      // If we are getting binary information, here it is!
      if(HeaatControl.lastread[0] == 'b'){
        led.fade(HeaatControl.lastread[1], HeaatControl.lastread[2], HeaatControl.lastread[3], 700);
      }
      
    }
    
    // Half second tick total, allowing us to check for networking stuff twice a second at max speed. 
    TickType_t del_tick = pdMS_TO_TICKS(100);
    if(last_network_time + del_tick > xTaskGetTickCount()){
      vTaskDelayUntil(&last_network_time, del_tick); 
    }
  }
  
  vTaskDelete(NULL);  
}
