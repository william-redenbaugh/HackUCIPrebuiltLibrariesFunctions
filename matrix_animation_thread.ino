/*
Author: William Redenbaugh
Last Edit Date: 1/15/2020
Notes:
Matrix thread function. 
*/

void matrix_animation_task(void *parameters){  
  // Used for time interval purposes so we can keep as many things 
  // in the same thread as possible. 
  
  uint16_t ticker = 0; 
  
  // Last wakeup time, so we can delay a certain amount of time
  // regardless of actual time required. 
  TickType_t last_matrix_time;
  
  // Stop throwing a dam warning!
  (void*)parameters; 

  // Thread stuff. 
  for(;;){
    last_matrix_time = xTaskGetTickCount();

    // Ticker. 
    ticker++; 
  
    // Things we want to happen once a second. 
    if(ticker % 50 == 0){
      seconds_to_minute++; 
      
      if(seconds_to_minute == 60)
        seconds_to_minute = ntp_time_module.seccond; 
      
      // If we want to show time,, updated by the second. 
      if(show_time){
        matrix_helper.show_time(ntp_time_module.hour, ntp_time_module.minute, seconds_to_minute, main_col);
        matrix_helper.show_date(ntp_time_module.month, ntp_time_module.day, main_col);
      }
    }
  
    // Things we want to happen twice a second. 
    if(ticker % 25 == 0){
      // Runs through cloud stuff. 
      cloud_commands();
    }
  
    // Stuff that happens 50x a second. 
    if(show_top_line_fill){
      matrix_helper.top_line_fill_random();
    }

    if(show_messages){
      matrix_helper.show_messages();
    }
  
    // Every minute. 
    if(seconds_to_minute == 0){
      if(show_temp)
        matrix_helper.show_temp_colored(uint8_t(weather.tempurature_f));
    }
  
    // Reset ticker at 1 seconds. 
    if (ticker == 1000){
      seconds_to_minute++;
      ticker = 0; 
    }
    
    if(seconds_to_minute == 60){
      // No need to update the time even to this point.. 
      seconds_to_minute = 0; 
    }
    // 20 ms tick, allowing for up to 50fps animations. 
    TickType_t del_tick = pdMS_TO_TICKS(20);
    if(last_matrix_time + del_tick > xTaskGetTickCount()){
      vTaskDelayUntil(&last_matrix_time, del_tick); 
    }  
  }
  vTaskDelete(NULL);
}
