/*
Author: William Redenbaugh
Last Edit Date: 12/19/2020
Notes:
Reads the time from an ntp server implementation
*/

#include "WiFiTimeModule.h"

void TimeModule::begin(int timezone){
  this->timezone = (timezone * 3600);
  configTime(this->timezone, this->dst, "pool.ntp.org");
}

void TimeModule::get(void){
  
	// TIME GETTERS BEGIN //
	struct tm time_info; 
	getLocalTime(&time_info);
	this->seccond = time_info.tm_sec;
	this->minute = time_info.tm_min;
	this->hour = time_info.tm_hour;
	this->day = time_info.tm_mday;
	this->month = time_info.tm_mon; 	
	this->year = time_info.tm_year; 
	// TIME GETTERS END //
}

bool TimeModule::get_blocking(uint32_t timeout_ms){
	long last_millis = millis();
	
	struct tm time_info; 
	bool got_time = getLocalTime(&time_info);
	while(!got_time){
		if(last_millis + timeout_ms < millis()){
			return false; 
			break;  
		}
		delay(70);
		got_time = getLocalTime(&time_info);
	}

  // TIME GETTERS BEGIN //
	this->seccond = time_info.tm_sec;
	this->minute = time_info.tm_min;
	this->hour = time_info.tm_hour;
	this->day = time_info.tm_mday;
	this->month = time_info.tm_mon; 
	this->year = time_info.tm_year; 
  // TIME GETTERS END //

  // LETTEM KNO WE GOT THE GOODS //
  return true; 
}
