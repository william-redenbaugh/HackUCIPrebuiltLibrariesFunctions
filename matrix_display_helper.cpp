/*
Author: William Redenbaugh
Last Edit Date: 1/15/2020
Notes:
Matrix helper file to deal with animation implementation.
*/

#include "matrix_display_helper.h"

void MatrixHelper::begin(RGB64x32MatrixPanel_I2S_DMA *dma_display){
  this->dma_display = dma_display; 
  this->use_matrix = xSemaphoreCreateMutex();
}


void MatrixHelper::fill_topdown(uint32_t interval_ms, uint16_t col){
  for(uint8_t x = 0; x < 64; x++){
    for(uint8_t y = 0; y < 32; y++){
      this->dma_display->drawPixel(x, y, col);
      delay(interval_ms);
    }  
  }
}

void MatrixHelper::fill_leftright(uint32_t interval_ms, uint16_t col){
  for(uint8_t y = 0; y < 32; y++){
    for(uint8_t x = 0; x < 64; x++){
      this->dma_display->drawPixel(x, y, col);
      delay(interval_ms);
    }  
  }
}

void MatrixHelper::show_time(uint8_t hour, uint8_t minute, uint8_t second, uint16_t col){
  xSemaphoreTake(this->use_matrix, portMAX_DELAY);
  // CLEAR OLD TIME BEGIN // 
  // First we gotta clear out the old numbers 
  // that need to be changed 
  this->dma_display->setCursor(9, 3);
  this->dma_display->setTextColor(0x00);
    
  // Clear out the old hour
  if(this->prev_hour != hour){
    if(this->prev_hour < 10){
      this->dma_display->print("0");
    }
    
    this->dma_display->print(prev_hour);
    this->prev_hour = hour; 
  }
  // otherwise move two spaces to the left
  else{
    this->dma_display->print("  "); 
  }  

  // Since it's clear anyway, may as well print it out 
  this->dma_display->print(":");
  
  if(this->prev_minute != minute){
    if(this->prev_minute < 10){
      this->dma_display->print("0");
    }
    this->dma_display->print(prev_minute);
    this->prev_minute = minute; 
  }
  // otherwise move two spaces to the left
  else{
    this->dma_display->print("  ");
  }
  
  // Since it's clear anyway, may as well print it out 
  this->dma_display->print(":");
  
  if(this->prev_second != second){
    if(this->prev_second < 10){
      this->dma_display->print("0");
    }
    this->dma_display->print(prev_second);
    this->prev_second = second; 
  }
  // otherwise move two spaces to the left
  else{
    this->dma_display->print("  ");
  }
  // CLEAR OLD TIME END // 
  
  // PRINT NEW TIME BEGIN // 
  this->dma_display->setCursor(9, 3);
  this->dma_display->setTextColor(col);
  if(hour < 10){
    this->dma_display->print("0");
  }
  this->dma_display->print(hour);
    
  if(alternate_dot){
    this->dma_display->print(":");
  }
  else{
    this->dma_display->print(" ");
  }
  if(minute < 10){
    this->dma_display->print("0");
  }
  this->dma_display->print(minute);
  
  if(alternate_dot){
    this->dma_display->print(":");
  }
  else{
    this->dma_display->print(" ");
  }
  if(second < 10){
    this->dma_display->print("0");
  }
  this->dma_display->print(second);

  alternate_dot = !alternate_dot;
  // PRINT NEW TIME END // 

  xSemaphoreGive(this->use_matrix);
}

void MatrixHelper::show_date(uint8_t month, uint8_t day, uint16_t col){
  // Setting up the new date and time!
  String date_time = "";
  Serial.print("Month:");
  Serial.println(month);
  switch((month)){
  case(0):
    date_time = "Jan";
    break;
  case(1):
    date_time = "Feb";
    break;
  case(2):
    date_time = "Mar";
    break;
  case(3):
    date_time = "Apr";
    break;
  case(4):
    date_time = "May";
    break;
  case(5):
    date_time = "Jun";
    break;
  case(6):
    date_time = "Jul";
    break;
  case(7):
    date_time = "Aug";
    break;
  case(8):
    date_time = "Sep";
    break;
  case(9):
    date_time = "Oct";
    break;
  case(10):
    date_time = "Nov";
    break;
  case(11):
    date_time = "Dec";
    break;
  default:
    date_time = "ERR";
    break;
  }
  date_time = date_time + "/" + String(day);

  xSemaphoreTake(this->use_matrix, portMAX_DELAY);
  // Clear out the previous text. 
  this->dma_display->setCursor(26, 14);
  this->dma_display->setTextColor(0x0000);
  this->dma_display->print(prev_date_time);

  // Set the previous date time. 
  prev_date_time = date_time; 
  
  this->dma_display->setCursor(26, 14);
  this->dma_display->setTextColor(col);
  this->dma_display->print(prev_date_time);
  xSemaphoreGive(this->use_matrix);
}

void MatrixHelper::show_temp(uint8_t temp, uint16_t col){
  // CLEAR OLD TIME BEGIN // 
  // First we gotta clear out the old numbers 
  // that need to be changed 
  xSemaphoreTake(this->use_matrix, portMAX_DELAY);
  this->dma_display->setCursor(3, 14);
  this->dma_display->setTextColor(0x00);
  if(this->prev_temp_f != temp){
    this->dma_display->print(this->prev_temp_f);
    this->dma_display->print("F");
    this->prev_temp_f = temp; 
  }

  this->dma_display->setCursor(3, 14);
  this->dma_display->setTextColor(col);
  this->dma_display->print(temp);
  this->dma_display->print("F");
  xSemaphoreGive(this->use_matrix);
}

void MatrixHelper::show_temp_colored(uint8_t temp){
  if (temp < 43){
    this->show_temp(temp, this->dma_display->color444(0, 0, 255));
  }  
  else if (temp >= 43 && temp < 50){
    this->show_temp(temp, this->dma_display->color444(10, 10, 255));
  }
  else if (temp >= 50 && temp < 57){
    this->show_temp(temp, this->dma_display->color444(100, 100, 100));
  }
  else if(temp >= 57 && temp < 63){
    this->show_temp(temp, this->dma_display->color444(150, 150, 120));
  }
  else if(temp >= 63 && temp < 70){
    this->show_temp(temp, this->dma_display->color444(170, 170, 10));
  }
  else if(temp >= 70 && temp < 78){
    this->show_temp(temp, this->dma_display->color444(170, 170, 0));
  }
  else if(temp >= 70 && temp < 78){
    this->show_temp(temp, this->dma_display->color444(200, 100, 0));
  }
  else if(temp >= 78 && temp < 85){
    this->show_temp(temp, this->dma_display->color444(230, 70, 0));
  }
  else if(temp >= 85 && temp < 93){
    this->show_temp(temp, this->dma_display->color444(230, 30, 0));
  }
  else if(temp >= 93){
    this->show_temp(temp, this->dma_display->color444(255, 0, 0));
  }
}

void MatrixHelper::reset_time_date(void){
  this->prev_hour = 255; 
  this->prev_minute = 255;   
  this->prev_second = 255; 
  this->prev_day = 255; 
  this->prev_month = 255; 
  this->prev_year = 0xFFFF; 
}

/*
void MatrixHelper::top_line_fill_random(void){
  top_line_fill_pos++;
  
  // If we hit 64 we reset the counter. 
  if(top_line_fill_pos == 64){
    top_line_fill_col = random(50, 65535);
    top_line_fill_pos = 0; 
  }
  
  // Fill in top line pixel at current location. 
  xSemaphoreTake(this->use_matrix, portMAX_DELAY);
  this->dma_display->drawPixel(this->top_line_fill_pos, 0, this->top_line_fill_col);
  xSemaphoreGive(this->use_matrix);
}
*/

void MatrixHelper::top_line_fill_random(void){
  
  switch(this->top_line_state){
  case(0):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(this->top_line_fill_pos, 0, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 64){
      top_line_fill_pos = 0; 
      top_line_state = 1;
    }
  break; 
  case(1):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(this->top_line_fill_pos, 1, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 64){
      top_line_fill_pos = 0; 
      top_line_state = 2;
    }
  break;    
  case(2):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(0, 2 + this->top_line_fill_pos, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 22){
      top_line_fill_pos = 0; 
      top_line_state = 3;
    }
  break;
  case(3):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(63, 2 + this->top_line_fill_pos, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 22){
      top_line_fill_pos = 0; 
      top_line_state = 4;
    }
  break;
  case(4):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(1, 2 + this->top_line_fill_pos, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 22){
      top_line_fill_pos = 0; 
      top_line_state = 5;
    }
  break;
  case(5):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(62, 2 + this->top_line_fill_pos, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 22){
      top_line_fill_pos = 0; 
      top_line_state = 6;
    }
  break;
  case(6):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(this->top_line_fill_pos + 2, 11, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 60){
      top_line_fill_pos = 0; 
      top_line_state = 7;
    }
  break;
  case(7):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(this->top_line_fill_pos + 2, 12, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 60){
      top_line_fill_pos = 0; 
      top_line_state = 8;
    }
  break;
  case(8):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(this->top_line_fill_pos + 2, 23, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 60){
      top_line_fill_pos = 0; 
      top_line_state = 9;
    }
  break;
  case(9):
    // Fill in top line pixel at current location. 
    xSemaphoreTake(this->use_matrix, portMAX_DELAY);
    this->dma_display->drawPixel(this->top_line_fill_pos + 2, 22, this->top_line_fill_col);
    xSemaphoreGive(this->use_matrix);

    top_line_fill_pos++;
    // If we hit 64 we reset the counter. 
    if(top_line_fill_pos == 60){
      // Reset everything and set a new color!
      top_line_fill_col = this->random_light_col();
      this->top_line_fill_pos = 0; 
      this->top_line_state = 0; 
    }
  break;
  }
}


void MatrixHelper::show_message(String str, uint16_t *col){
  this->message = str; 
  this->message_length = message.length();
  this->skip_back_amount = message_length * 7; 
  this->current_message_spot = 64; 
  this->msg_col = col; 
  this->end_spot = -1 * int16_t(this->skip_back_amount);
  this->messages_present = true; 
}

void MatrixHelper::show_messages(void){
  if(!this->messages_present)
    return; 
  // Since we are using the resource. 
  xSemaphoreTake(this->use_matrix, portMAX_DELAY);

  // Clear out the old message
  this->dma_display->setCursor(this->current_message_spot, 24);
  this->dma_display->setTextColor(0x0000);
  this->dma_display->print(this->message);

  // Move stuff over and print the real message
  this->current_message_spot--; 
  
  this->dma_display->setCursor(this->current_message_spot, 24);
  this->dma_display->setTextColor(*this->msg_col);
  this->dma_display->print(this->message);  
  
  // Give back whatcha take!
  xSemaphoreGive(this->use_matrix);

  // Restart it back over again
  if(this->current_message_spot == this->end_spot){
    this->current_message_spot = 64;
  }
}

uint16_t MatrixHelper::random_light_col(void){
  uint8_t random_col = random(0, 14);
  switch(random_col){
   // neon green
   case(0):
   return this->dma_display->color444(57, 255, 20);
   break; 
   // indigo
   case(1):
   return this->dma_display->color444(140, 0, 250);
   break; 
   // neon red
   case(3):
   return this->dma_display->color444(255, 7, 58);
   break; 
   // cyan
   case(4):
   return this->dma_display->color444(0, 255, 255);
   break; 
   // yellow
   case(5):
   return this->dma_display->color444(255, 255, 0);
   break;
   // lavendar
   case(6):
   return this->dma_display->color444(220, 220, 250);
   break; 
   // pouder blue
   case(7):
   return this->dma_display->color444(176, 224, 230);
   break;
   // pink
   case(8):
   return this->dma_display->color444(255, 192, 203);
   break;
   // orange
   case(9):
   return this->dma_display->color444(255,165,0);
   break;
   // Lime green
   case(10):
   return this->dma_display->color444(50,205,50);
   break;
   // ultraviolet
   case(12):
   return this->dma_display->color444(175, 150, 250);
   break;
   // turqoise
   case(13):
   return this->dma_display->color444(175,238,238);
   break;
   // peach
   case(14):
   return this->dma_display->color444(255,218,185);
   break;
   // white otherwise
   default:
   return this->dma_display->color444(200, 200, 200);
   break;
  }
}

void MatrixHelper::get_matrix(void){
  xSemaphoreTake(this->use_matrix, portMAX_DELAY);
}

void MatrixHelper::release_matrix(void){
  xSemaphoreGive(this->use_matrix);  
}
