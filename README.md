This software is part of a ESP8266 bee hive monitoring project I created years ago. 
The orginal project read in sensor data over SPI and I2C then packaged into a json format and sent it to a Mongodb base over WIFI via the MQTT protocol. 
This project was later adapted to use LORA instead of wiFi. 
Another prototype version was created on a different platform which used LORAWAN.

This repo also contains code to make a common interface for several sensors availiable from Adafruit. 
The supported sensors are the ADS1X15, BME280, and the HDC1000.
The software is plug and play with the aforementioned Adafruit sensors. 

I am uploading this project because others may find this code useful.
