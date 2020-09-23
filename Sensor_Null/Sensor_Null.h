

/*************************************************** 
  This is a library for the NULL Sensor
  These sensors use I2C to communicate, 2 pins are required to  
  interface
	This sensor is fake.
 ****************************************************/
#ifndef SENSOR_NULL_H
#define SENSOR_NULL_H


#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Wire.h"
#include <Sensor_Wrapper.h>

class Sensor_Null : public Sensor_Wrapper 
{
 public:
  Sensor_Null();
  bool begin(uint8_t a = 0x00);
  void setSensor();
  sensor_t getSensor();
  float getValue(int Value_Type); 
  std::unique_ptr<Sensor_Wrapper> clone() override; 
  
 private:

};

#endif
