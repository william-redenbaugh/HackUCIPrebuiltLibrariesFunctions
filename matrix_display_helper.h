#ifndef _MATRIX_DISPLAY_HELPER_H
#define _MATRIX_DISPLAY_HELPER_H

/*
Author: William Redenbaugh
Last Edit Date: 1/15/2020
Notes:
Matrix helper file to deal with animation class and definitions. 
*/

#include "Arduino.h"
#include "wifi_setup.h"
#include "mqtt_task.h"
#include "WiFiTimeModule.h"
#include <ESP32-RGB64x32MatrixPanel-I2S-DMA.h>

class MatrixHelper{
  public: 
    void begin(RGB64x32MatrixPanel_I2S_DMA *dma_display);

    // Basic Fill animations. 
    void fill_topdown(uint32_t delay_ms, uint16_t col);
    void fill_leftright(uint32_t delay_ms, uint16_t col);

    // Basic Fade fill animations
    void fade_topdown(uint32_t delay_ms, uint16_t col);
    void fade_leftright(uint32_t delay_ms, uint16_t col);

    void top_line_fill_random(void);

    // Time set animation. 
    void show_time(uint8_t hour, uint8_t minute, uint8_t second, uint16_t col);
    void show_date(uint8_t month, uint8_t day, uint16_t col);
    void reset_time_date(void);

    // Temp set animation
    void show_temp(uint8_t temp, uint16_t col);
    void show_temp_colored(uint8_t temp);

    uint16_t random_light_col(void);

    // Semaphore/Mutex Acess to the LED matrix .
    void get_matrix(void);
    void release_matrix(void);

    // Using the messages. 
    void show_messages(void);
    void show_message(String str, uint16_t *col);
    
  private: 
    // So we can use the dma_display
    RGB64x32MatrixPanel_I2S_DMA *dma_display;


    // TIME VARIABLES BEGIN //
    uint8_t prev_hour = 99;
    uint8_t prev_minute = 99; 
    uint8_t prev_second = 99; 
    uint8_t prev_day = 99; 
    uint8_t prev_month = 99;  
    uint16_t prev_year = 0xFFFF;
    bool alternate_dot = false; 
    String prev_date_time;
    // TIME VARIABLES END //

    // TOP LINE FILL RANDOM ANIMATION BEGIN // 

    // Keeping current location and color. 
    uint8_t top_line_fill_pos = 0; 
    uint16_t top_line_fill_col = 0xFFFF; 
    uint8_t top_line_state = 0; 
    // TOP LINE FILL RANDOM ANIMATION END // 

    // TEMP ANIMATION BEGIN // 
    uint8_t prev_temp_f = 250; 
    // TEMP ANIMATION END // 

    // Matrix Hold Semaphore
    SemaphoreHandle_t use_matrix; 

    // MESSAGE VARIABLES BEGIN // 
    volatile bool messages_present = false; 
    volatile uint16_t message_length = 0;
    volatile uint16_t skip_back_amount = 0; 
    volatile int16_t current_message_spot = 0; 
    uint16_t *msg_col;
    volatile int16_t end_spot = 0;  
    String message;
    // MESSAGES VARIBALES END // 
};

#endif 
