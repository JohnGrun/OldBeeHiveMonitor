#ifndef BEES_H
#define BEES_H


#include <PubSubClient.h>
//#include <ESP8266WiFi.h>

//Used for scanning the sensors 
#include <vector> // used for the auto detection 
#include <SPI.h>  // Arduino 
#include <Wire.h>
#include <ArduinoJson.h>
#include <Sensor_Wrapper.h>
#include <Adafruit_HDC1000.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1015.h>
#include <Sensor_Null.h> // null sensor type
#include <RH_RF95.h> // Lora radio

RH_RF95 LORA;

// CLASS STRUCTURE

//BEES.ino
//    |
//    Sensors   --Temperature sensors, weight, light, noise, etc.
//    Comms    -- Radio Wifi Bluetooth
//    Messaging -- Pub Sub 

// may be unused 
struct PubSubInfo {
  String BeeHiveID;
  String pubMessage;
  String pubSubscribe;
}PubSubInfo_t;

enum Return_Codes_Type {
  Success = 666,
  Failure = -666,
  Connected = 1000,
  READ_FAILURE = -9999 
};

///////////////////////////////////////////////////////////////////////////Sensors/////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////Temperature / humidity / pressure //////////////////////////////////////////////////////////////////////

//I2c SDA port to GPIO0 and SCL port to GPIO2.orginally 
//I2c sensors
int TCAADDR = 0x70;  //multiplexer  // should be a seperate class 
int SDA_MASTER = 2; // pin number see datasheet
int SCL_MASTER = 4; // pin number see datasheet
int POWER_3_3_SWITCHED = 5;

std::vector<std::unique_ptr<Sensor_Wrapper>> KnownSensors;
std::vector<std::unique_ptr<Sensor_Wrapper>> FoundSensors;

Adafruit_BME280 Test_BME280; // I2C
Adafruit_HDC1000 Test_HDC; // I2C
Adafruit_ADS1115 Test_ADS;  // I2C -- Use this for the 16-bit version //
Sensor_Null Test_Null; 

void tcaselect(uint8_t i); //sensor multiplexer
std::vector<std::unique_ptr<Sensor_Wrapper>> Scan_I2C_Sensors(int MaxIndex, std::vector<std::unique_ptr<Sensor_Wrapper>> &KnownSensors);
String Message_Builder_JSON(std::unique_ptr<Sensor_Wrapper> &Sensor);

//Wifi
//////////////////////////////////////////////////////////////////////////////WIFI//////////////////////////////////////////////////////////////////////
// These are the SSID and PASSWORD to connect to your Wifi Network
//  put details appropriate for your network between the quote marks,
//  eg  #define ESP8266_SSID "YOUR_SSID"

// make into struct wifi
const char* ssid = "WIFI-SSID";
const char* password = "WIFI_PASSWORD";

WiFiClient Wificlient;

int Initialize_Wifi(void);

int Wifi_Connection_Manager();
 
//////////////////////////////////////////////////////////////////////////////MQTT///////////////////////////////////////////////////////////////////////
// Update these with values suitable for your network.
char* server = "www.example.com";  

uint16_t mqtt_port = 0000;

PubSubClient client(Wificlient, server);
struct PubSubInfo PubSubData;

int Initialize_MQTT(String clientName);
int MQTT_Connection_Setup(PubSubClient PubSub_type, struct PubSubInfo PubSubInfo_Type);
void MQTTcallback(char* topic, byte* payload, unsigned int length);
String macToStr(const uint8_t* mac);

////////////////////////////////////////////////////////////////////////////////Message builder ///////////////////////////////////////////////////////////////////////
String Generate_ClientName();
///////////////////////////////////////////////////////////////////////////COMMS/////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////Function prototypes///////////////////////////////////////////////////////////////////////


#endif /* BEES_H */


