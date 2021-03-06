/*
* Copyright (C) 2008 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software< /span>
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/* Update by K. Townsend (Adafruit Industries) for lighter typedefs, and
 * extended sensor support to include color, voltage and current */
 
#ifndef SENSOR_WRAPPER_H
#define SENSOR_WRAPPER_H

#include <vector> // used to describe the features of a sensor
#include <memory>
//#include <string>

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

/* Intentionally modeled after sensors.h in the Android API:
 * https://github.com/android/platform_hardware_libhardware/blob/master/include/hardware/sensors.h */

/* Constants */
#define SENSORS_GRAVITY_EARTH             (9.80665F)              /**< Earth's gravity in m/s^2 */
#define SENSORS_GRAVITY_MOON              (1.6F)                  /**< The moon's gravity in m/s^2 */
#define SENSORS_GRAVITY_SUN               (275.0F)                /**< The sun's gravity in m/s^2 */
#define SENSORS_GRAVITY_STANDARD          (SENSORS_GRAVITY_EARTH)
#define SENSORS_MAGFIELD_EARTH_MAX        (60.0F)                 /**< Maximum magnetic field on Earth's surface */
#define SENSORS_MAGFIELD_EARTH_MIN        (30.0F)                 /**< Minimum magnetic field on Earth's surface */
#define SENSORS_PRESSURE_SEALEVELHPA      (1013.25F)              /**< Average sea level pressure is 1013.25 hPa */
#define SENSORS_DPS_TO_RADS               (0.017453293F)          /**< Degrees/s to rad/s multiplier */
#define SENSORS_GAUSS_TO_MICROTESLA       (100)                   /**< Gauss to micro-Tesla multiplier */

/** Sensor types */
typedef enum
{
  SENSOR_TYPE_NULL 					= (0),
  SENSOR_TYPE_ACCELEROMETER         = (1),   /**< Gravity + linear acceleration */
  SENSOR_TYPE_MAGNETIC_FIELD        = (2),
  SENSOR_TYPE_ORIENTATION           = (3),
  SENSOR_TYPE_GYROSCOPE             = (4),
  SENSOR_TYPE_LIGHT                 = (5),
  SENSOR_TYPE_PRESSURE              = (6),
  SENSOR_TYPE_UNKNOWN				= (7),
  SENSOR_TYPE_PROXIMITY             = (8),
  SENSOR_TYPE_GRAVITY               = (9),
  SENSOR_TYPE_LINEAR_ACCELERATION   = (10),  /**< Acceleration not including gravity */
  SENSOR_TYPE_ROTATION_VECTOR       = (11),
  SENSOR_TYPE_RELATIVE_HUMIDITY     = (12),
  SENSOR_TYPE_AMBIENT_TEMPERATURE   = (13),
  SENSOR_TYPE_NOT_DEFINED 			= (14),
  SENSOR_TYPE_VOLTAGE               = (15),
  SENSOR_TYPE_CURRENT               = (16),
  SENSOR_TYPE_COLOR                 = (17),
  SENSOR_TYPE_ALTITUDE				= (18),
  SENSOR_TYPE_NOT_SUPPORTED		    = (-99999)
} sensors_type_t;

/** struct sensor_s is used to describe basic information about a specific sensor. */
typedef struct
{
    String name;     	                      /**< sensor name */
    int32_t  version;                         /**< version of the hardware + driver */
    int32_t  sensor_id;                       /**< unique sensor identifier */
    float    max_value;                       /**< maximum value of this sensor's value in SI units */
    float    min_value;                       /**< minimum value of this sensor's value in SI units */
    float    resolution;                      /**< smallest difference between two values reported by this sensor */
    int32_t  min_delay;                       /**< min delay in microseconds between events. zero = not a constant rate */
   	std::vector<int> SupportedFunctions;	  /**< Vector containing the supported functions of the sensor- functions from sensors_type_t */  
	
} sensor_t;

class Sensor_Wrapper {
 
public:

  	// Constructor(s)
  	Sensor_Wrapper() {}
  	virtual ~Sensor_Wrapper() {}

	//virtual bool begin() = 0;	 		/**< is the sensor currently there ? */
	virtual bool begin(uint8_t addr =0) = 0;	 		/**< is the sensor currently there ? */
	virtual void setSensor() = 0; 					/**< Set the sensor information*/
	virtual sensor_t getSensor() = 0;      			 /**< Return the sensor information*/
	virtual float getValue(int Value_Type) = 0;  	/**< Return the value of a given sensor type as passed in by Value_Type see sensors_t*/
	virtual std::unique_ptr<Sensor_Wrapper> clone() = 0;		/**< Return a pointer to an object of the subclass object*/
	//virtual Sensor_Wrapper* clone() = 0;		/**< Return a pointer to an object of the subclass object*/

	sensor_t Sensor_Infomation; 					/**< Sensor type variable to be inherited by all sensor types*/ 
	
	
	std::vector<String> SupportedFunctionName = 
	{ 
		"NULL", 				//SENSOR_TYPE_NULL
		"ACCELEROMETER", 		//SENSOR_TYPE_ACCELEROMETER
		"MAGNETIC_FIELD", 		//SENSOR_TYPE_MAGNETIC_FIELD
		"ORIENTATION", 			//SENSOR_TYPE_ORIENTATION
		"GYROSCOPE", 			//SENSOR_TYPE_GYROSCOPE
		"LIGHT", 				//SENSOR_TYPE_LIGHT
		"PRESSURE",				//SENSOR_TYPE_PRESSURE
		"SENSOR_TYPE_UNKNOWN",	//SENSOR_TYPE_UNKNOWN
		"PROXIMITY", 			//SENSOR_TYPE_PROXIMITY
		"GRAVITY" , 			//SENSOR_TYPE_GRAVITY
		"LINEAR_ACCELERATION", 	//SENSOR_TYPE_LINEAR_ACCELERATION
		"ROTATION_VECTOR", 		//SENSOR_TYPE_ROTATION_VECTOR
		"RELATIVE_HUMIDITY", 	//SENSOR_TYPE_RELATIVE_HUMIDITY
		"AMBIENT_TEMPERATURE",  //SENSOR_TYPE_AMBIENT_TEMPERATURE
		"NOT_DEFINED", 
		"VOLTAGE", 				//SENSOR_TYPE_VOLTAGE
		"CURRENT", 				//SENSOR_TYPE_CURRENT
		"COLOR",				//SENSOR_TYPE_COLOR
		"ALTITUDE",				//SENSOR_TYPE_ALTITUDE
		"NOT_SUPPORTED"
	 }; /**< Sensor type variable to be inherited by all sensor types*/ 
	 
 protected:
	
	
 private:

  //bool _autoRange;
  

};

#endif
