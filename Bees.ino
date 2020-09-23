#include "Bees.h"

/*
 * 
 * I2c SDA port to GPIO0 and SCL port to GPIO2.
 * 
 * 
 */
//////////////////////////////////////////////////////////////////////////////SETUP///////////////////////////////////////////////////////////////////////
void setup()
{

    // setup debugging interface
    Serial.begin(9600);
    Serial.println("BEE DEBUG test!");

    Serial.println("Turning on the 3.3 Power rail\n");  

    //turn on 3.v power rail
    pinMode(POWER_3_3_SWITCHED, OUTPUT);
    digitalWrite(POWER_3_3_SWITCHED, HIGH);

    delay(100);// wait for everything to come up

  //////////////////////////////////////////////////////////////////////I2C/////////////////////////////////////////////////////////
    
    //wrap into function

    //add all known sensor types to the Known Sensor vector
    // The null sensor must always be the last sensor
    KnownSensors.push_back(Test_BME280.clone());
    KnownSensors.push_back(Test_HDC.clone());
    KnownSensors.push_back(Test_ADS.clone());
    KnownSensors.push_back(Test_Null.clone());
    
    //  Serial.println("End of transistor\n");  

    //KnownSensors is globally scoped. We need to fix this. 

    Wire.begin(SCL_MASTER,SDA_MASTER);  // we need to initialize the wire lib at this level as all of the libs have a wire.begin. 

    delay(100); // wait 100 milliseconds
    
    FoundSensors = Scan_I2C_Sensors(8, KnownSensors);
    
    if (!LORA.init())
    {
      Serial.println("init failed");
    }
   //delay(5000); // wait 10 seconds to connect to an AP

    // This shit should work even if the thing does not connect
    // pass the client name to the MQTT client
   
}
//////////////////////////////////////////////////////////////////////////////LOOP!///////////////////////////////////////////////////////////////////////

void loop()
{ 
      /*
      Init=Initialize_Wifi();
      Wifi_Connection_Manager();
      Serial.print("The value of the Initialize_Wifi() is: ");
      Serial.print(Init);
      Serial.print("\n");

      Serial.println("\n");  
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    */

    /*
    // SENSOR UUID BASED OFF OF MAC ADDRESS.
    String PubSubName = Generate_ClientName();
    Serial.println(PubSubName);

   // This will be removed in the next version
   Initialize_MQTT(PubSubName);
  
  //////////////////////////////////////////////////////////////////////////////Wireless ///////////////////////////////////////////////////////////////////////
         //Serial.println("\n");  
        //Serial.println("IP address: ");
  
     if (Wifi_Connection_Manager()== Success)
     {
  
        Serial.println("\n");  
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
  
        

            for(int SensorIndex = 0; SensorIndex < NumSensors; SensorIndex++)
            {
              tcaselect(SensorIndex);
              PubSubData.pubMessage = Message_Builder_JSON(FoundSensors[SensorIndex]);
           
              MQTT_Connection_Setup(client,PubSubData);
              if(client.connected())
              {
                client.loop();
              }

              //may not need this anymore
              //delay(1000); // do to duplicate timestamp issue in mysql 
            }
           int NumSensors = FoundSensors.size();
            Serial.print("\n");
            Serial.print("***********************************************************************************");
            Serial.print("\n");
            Serial.print("Data Sent\n");
            
            Serial.print("\n"); 
            
            Serial.print("Data Received");
            Serial.print("\n");
  
     }
     else 
     {
        Serial.println("WIFI FAILED MQTT DATA NOT SENT");  
      
     }

     */
     String RadioMessage;
     int NumSensors = FoundSensors.size();
      // NEW LORA RADIO!
       for(int SensorIndex = 0; SensorIndex < NumSensors; SensorIndex++)
       {
              
              tcaselect(SensorIndex);
              RadioMessage = RadioMessage + Message_Builder_JSON(FoundSensors[SensorIndex]); // all sennsors summed.
              //RadioSend(RadioMessage);

       }

        Serial.println(RadioMessage);
        Serial.println("Sending to rf95_server");
    // Send a message to rf95_server
    //uint8_t data[] = "Hello World!";
    //rf95.send(data, sizeof(data));
    LORA.send(RadioMessage.c_str(), sizeof(RadioMessage.c_str()));
    
    //rf95.waitPacketSent();
     LORA.waitPacketSent();
     
    // Now wait for a reply
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
  
    if (LORA.waitAvailableTimeout(300))
    { 
      // Should be a reply message for us now   
      if (LORA.recv(buf, &len))
     {
        Serial.print("got reply: ");
        Serial.println((char*)buf);
  //      Serial.print("RSSI: ");
  //      Serial.println(LORA.lastRssi(), DEC);    
      }
      else
      {
        Serial.println("recv failed");
      }
    }
    else
    {
      Serial.println("No reply, is LORA running?");
    }
    //delay(400);
   
     //turn off the power rail as soon as possible
    digitalWrite(POWER_3_3_SWITCHED, LOW);
  
    pinMode(SCL_MASTER, OUTPUT);
    digitalWrite(SCL_MASTER, LOW);

    pinMode(SDA_MASTER, OUTPUT);
    digitalWrite(SDA_MASTER, LOW);
    
    Serial.println("Entering Deep Sleep mode");
    delay(500);   
    ESP.deepSleep(900000000, WAKE_RF_DEFAULT); // Sleep for 15 minutes time in micro seconds
    //ESP.deepSleep(30000000, WAKE_RF_DEFAULT); // Sleep for 15 minutes time in micro seconds
    delay(1000); // reports every 60min

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////Start Functions ////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

String Generate_ClientName()
{ // this works quite well

  String Name;
  Name += "BeeHive";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Name += macToStr(mac);

  return Name;
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

/*
int Initialize_Wifi(void)
{
  // Set the baud rate, this depends on your ESP8266's previous setup
  // as it remembers the rate.  9600 is the default for recent firmware
  // and is the recommended speed since we will be using SoftwareSerial
  // to communicate with it.

  int status;
  if (WiFi.status() != WL_CONNECTED) {
    status = WiFi.begin(ssid, password);  
  }
  
  if (status == WL_CONNECTED )
  {
    return Success;
  }
  else
  {
    return Failure;
  }
}

 int Wifi_Connection_Manager()
 {

      // Connect to the given Wifi network as a station (as opposed to an 
  // Access Point running it's own Wifi network).
  //
  // See how &Serial is passed, the library will print status and debug
  // information there when it is connecting. 
  //
  // Note that this will not return until it has successfully connected,
  // it will just keep retrying.  I like to keep things simple!  

//////////////////////////////////////////////////////////////////////////////WIFI CONNECTION MANAGER///////////////////////////////////////////////////////////////////////
  Serial.print("####################################WIFI CONNECTING################################################\n");
  Serial.print("WILL ATTEMPT TO CONNECT 10 TIMES BEFORE SLEEPING\n");
  int Count=0;
  int Init=0;
  
     while ((WiFi.status() != WL_CONNECTED)) 
     {
      Init=Initialize_Wifi();
      Serial.print("The value of the Initialize_Wifi() is: ");
      Serial.print(Init);
      Serial.print("\n");
      Count = Count + 1;  // fuck the ++ operator 
      delay(100);
      Serial.print(".");

      if(Count >=15)
      {
        int status = WiFi.status() ;
        // 15 connections attempts have failed.
          Serial.print("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!WIFI FAILURE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
          Serial.print("Failure is: ");
          
          if(status == WL_IDLE_STATUS )
          {
            Serial.print("WL_IDLE_STATUS ");
          }
          //WL_NO_SSID_AVAIL:
          if(status == WL_NO_SSID_AVAIL )
          {
            Serial.print("WL_NO_SSID_AVAIL ");
          }
          if(status == WL_SCAN_COMPLETED )
          {
            Serial.print("WL_SCAN_COMPLETED ");
          }
          if(status == WL_CONNECT_FAILED )
          {
            Serial.print("WL_CONNECT_FAILED ");
          }
          if(status == WL_CONNECTION_LOST )
          {
            Serial.print("WL_CONNECTION_LOST ");
          }
          if(status == WL_DISCONNECTED )
          {
            Serial.print("WL_DISCONNECTED ");
          }
          Serial.print("\n");
        return Failure;
        
      }
        
    }

  // we have connected to the WIFI
  Serial.print("###################################### WIFI CONNECTION SUCCESSFUL ############################\n");
  return Success; 

 }

int Initialize_MQTT(String clientName)
{
  //////////////////////////////////////////////////////////////////////////////PUBSUB CONNECTION MANAGER///////////////////////////////////////////////////////////////////////
  // Generate client name based on MAC address and last 8 bits of microsecond counter
  // pubsub // setup for this magical MQTT service.
   
  Serial.println("MQTT INITIALIZATION");
  //strncpy(PubSubData.BeeHiveID, clientName.c_str(),sizeof(PubSubData.BeeHiveID));
  PubSubData.BeeHiveID = clientName;
  
  //strncpy(PubSubData.pubSubscribe,"/Commands",sizeof(PubSubData.pubSubscribe));
  PubSubData.pubSubscribe = "/Commands";
  return Success;
}

int MQTT_Connection_Setup(PubSubClient PubSub_type, struct PubSubInfo PubSubInfo_Type)
{
 
        // clientID, username, MD5 encoded password
        // clientid, username, password

       if(!PubSub_type.connected())
       {
           //if(PubSub_type.connect(PubSubInfo_Type.BeeHiveID, "BeeMonitor", "CandyMan"))
          if(PubSub_type.connect(MQTT::Connect(PubSubInfo_Type.BeeHiveID).set_auth("BeeMonitor", "CandyMan")))
           {
            Serial.println("MQTT Connection Established");
            if(PubSub_type.publish("Here",PubSubInfo_Type.pubMessage))
            {
              delay(200);

              Serial.println("MQTT Message Published!");
            }
            else
            {
              Serial.println("MQTT Message Publish Failure!!!!!");
            }

            PubSub_type.subscribe(PubSubInfo_Type.pubSubscribe);
            Serial.println("MQTT loop");
            //PubSub_type.loop();  // call the magical loop function

            client.loop();                        
            Serial.println("MQTT loop end");

            return Success;
           }
           else 
           {
            //client.loop();
            Serial.println("MQTT Connection FAILED");
            return Failure;
           }

        return Connected;   
       }
        else
        {
          PubSub_type.publish("Here",PubSubInfo_Type.pubMessage);
          client.loop();
          return Success;   // return a sussess and return
        }
  
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
*/

String Message_Builder_JSON(std::unique_ptr<Sensor_Wrapper> &Sensor)
{
    //int JsonBufferSize = 0;
    //int SensorIndex = 0;
    int NumSensors = FoundSensors.size();
    float SensorTempValue = 0;
    const int JsonBufferSize = 900; // needs to be dynamic somehow.......
     char buffer[JsonBufferSize + 2]; // scales with the buffer size and null char
    int SensorTempType = 0; // stores a temp for a the sensor type
    String SensorSensorTempName;
    
    Serial.println("Inside of the generic Message builder");
    
    StaticJsonBuffer<JsonBufferSize> jsonBuffer; // 
    // per sensor version
    JsonObject& root = jsonBuffer.createObject();
    root["ID"] = Generate_ClientName(); // main id 
  
    root["Sensor"] = Sensor->getSensor().name; 
    //for loop to get all sensor reading. each sensor may have multiple types.

      for( int FunctionIndex =0; FunctionIndex < Sensor->getSensor().SupportedFunctions.size(); FunctionIndex++)
      { // loop past all sensor reading types 
        
        String F1(FunctionIndex);
        JsonObject& SensorJsonFunction = root.createNestedObject("F" + F1);

        //get the sensor value                        
        Serial.println("Supported Functon Names");
        SensorTempType = Sensor->getSensor().SupportedFunctions[FunctionIndex];
        
        //SensorTypeName = Sensor[SensorIndex]->SupportedFunctionName[SensorTempType];
        SensorJsonFunction["Type"] = Sensor->SupportedFunctionName[SensorTempType];

        Serial.println(" Get value");
        //SensorTempValue = Sensor[SensorIndex]->getValue(SensorTempType); // get the sensor type
        SensorJsonFunction["Value"] = Sensor->getValue(SensorTempType);
        // get the function name
        
      }//for( int i =0; i < Sensor[SensorIndex]->getSensor().SupportedFunctions.size(); i++)
  //}//for(int SensorIndex = 0; SensorIndex < NumSensors; SensorIndex++)

  root.printTo(buffer, sizeof(buffer));

  String outstring(buffer);
  Serial.println("Message_Builder_JSON: " + outstring);
  
  return outstring;
  
}

void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

std::vector<std::unique_ptr<Sensor_Wrapper>> Scan_I2C_Sensors(int MaxIndex, std::vector<std::unique_ptr<Sensor_Wrapper>> &TestSensors)
{// scans the i2c bus

  // pass a vector of possible sensors into function and compare
      std::vector<std::unique_ptr<Sensor_Wrapper>> I2C_Sensors;
      
      // KnownSensors.at(x);
      
      int NumberofKnownSensors = TestSensors.size(); // x must be less then

      // Test each device
      int select_index =0;
      int LoopCounter=0;
      
      for(select_index=0; select_index< MaxIndex; select_index++) // this is the number of I2C devices that can be connected to the I2C Mux
      {
        tcaselect(select_index);
        Serial.print("Scanning at index : ");
        Serial.print(select_index);
        Serial.println("");
        
        for(LoopCounter=0; LoopCounter < NumberofKnownSensors; LoopCounter++)
        {
          //delay(1000); // added for debugging 

          Serial.print("The value of the returned object is : ");
          Serial.print(TestSensors[LoopCounter]->begin());
          Serial.println("");
          
           if(TestSensors[LoopCounter]->begin())  // when possible replace with KnownSensors.at()
           { 
               I2C_Sensors.push_back(TestSensors[LoopCounter]->clone());  // when possible replace with KnownSensors.at()
               Serial.print("Found sensor at I2C Mux index of : ");
               Serial.print(select_index);
               Serial.println("");
               Serial.print("Sensor Type is : ");
               Serial.println(TestSensors[LoopCounter]->getSensor().name);
               break; // leave the loop
               //Serial.println("");
           }

           // otherwise add a NULL type sensor at a given index. 
        }
       
      } //for(select_index=0; select_index< MaxIndex; select_index++)

      return I2C_Sensors;
    }
    
