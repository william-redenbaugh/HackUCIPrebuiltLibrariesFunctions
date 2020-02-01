/*
Author: William Redenbaugh
Last Edit Date: 1/15/2020
Notes:
Interpreting the data coming from the cloud  
*/

void cloud_commands(void){
  // Checking to make sure a request came in from the 
  // MatrixControl MQTT Adafruit IO Feed. 
  if(mqtt_read_task.get_subscription_status(0)){
      String str = (char*)Matrix_Control.lastread;

      // Command system. 
      if(Matrix_Control.lastread[0] == ';'){
        matrix_helper.get_matrix();
        dma_display.fillScreen(0x0000);
        matrix_helper.release_matrix();
        
        if(str.equals(";display_on")){
          show_time = true; 
          show_top_line_fill = true; 
          show_temp = true; 
          show_messages = true; 
          
          matrix_helper.show_temp_colored(uint8_t(weather.tempurature_f));
          matrix_helper.show_time(ntp_time_module.hour, ntp_time_module.minute, seconds_to_minute, main_col);
          matrix_helper.show_date(ntp_time_module.month, ntp_time_module.day, main_col);
 
        }
        else if(str.equals(";display_off")){
          show_time = false; 
          show_temp = false; 
          show_top_line_fill = false; 
          show_messages = false; 

          matrix_helper.get_matrix();
          dma_display.fillScreen(0x0000);
          matrix_helper.release_matrix();
        }
        else if(str.equals(";clock_on")){
          show_time = true; 
        }
        else if(str.equals(";clock_off")){
          show_time = false;   

          matrix_helper.get_matrix();
          dma_display.fillScreen(0x0000);
          matrix_helper.release_matrix();
        }
        else if(str.equals(";clock_toggle")){
          show_time = !show_time;  
        }
      }

      if(Matrix_Control.lastread[0] == 'c'){
        matrix_helper.get_matrix();
        dma_display.fillScreen(0x0000);
        matrix_helper.release_matrix();
        
        if(str.equals("cred")){
          main_col = dma_display.color444(255, 0, 0);
        }
        else if(str.equals("cgreen")){
          main_col = dma_display.color444(0, 255, 0);
        }
        else if(str.equals("cblue")){
          main_col = dma_display.color444(0, 0, 255);
        }
        else if(str.equals("crose")){
          main_col = dma_display.color444(255, 0, 127);
        }
        else if(str.equals("cpink")){
          main_col = dma_display.color444(255, 182, 194);
        }
        else if(str.equals("cpurple")){
          main_col = dma_display.color444(240, 0, 240);
        }
        else if(str.equals("ccyan")){
          main_col = dma_display.color444(0, 255, 255);
        }
        else if(str.equals("cwhite")){
          main_col = dma_display.color444(250, 250, 250);
        }
        else if(str.equals("cindigo")){
          main_col = dma_display.color444(140, 0, 250);
        }

        matrix_helper.show_temp_colored(uint8_t(weather.tempurature_f));
        matrix_helper.show_time(ntp_time_module.hour, ntp_time_module.minute, seconds_to_minute, main_col);
        matrix_helper.show_date(ntp_time_module.month, ntp_time_module.day, main_col);
      
      }

      if(Matrix_Control.lastread[0] == 'm'){
        String msg = (char*)Matrix_Control.lastread;
        msg.remove(0, 1);

        // Clear out the bottom of the matrix. 
        dma_display.fillRect(0, 24, 63, 32, 0x0000);
        matrix_helper.show_message(msg, &main_col);
      }
    }  
}
