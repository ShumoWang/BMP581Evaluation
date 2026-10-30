/*
Shumo Wang 25-06-2026
This code drives an arduino that uses I2C protocol to communicate with a BMP581 pressure sensor. ODR is setted manually.
The pressure reading is then converted to altitude by international standard barometric formula h = 44330 * (1 - (p/p0)^0.19029). 

Code originated and altered from SparkFun BMP581 Arduino Library Example1_BasicReadingI2C.ino
*/


#include <Wire.h>
#include "SparkFun_BMP581_Arduino_Library.h"
#include <cmath>

// Create a new sensor object
BMP581 pressureSensor;

// I2C address selection
uint8_t i2cAddress = BMP581_I2C_ADDRESS_DEFAULT; // 0x47
//uint8_t i2cAddress = BMP581_I2C_ADDRESS_SECONDARY; // 0x46

// constants defined for international standard barometric formula
const int c1 = 44330;
const float c2 = 0.19029;
float p0 = 101800.0; // data read from ground floor of sensor
const float offset = 0; // offset value between two sensors

// time variable
int time = 0;

void setup()
{
    // Start serial
    Serial.begin(115200);
    Serial.println("BMP581 begin!");

    // Initialize the I2C library
    Wire.begin();

    // Check if sensor is connected and initialize
    // Address is optional (defaults to 0x47)
    while(pressureSensor.beginI2C(i2cAddress) != BMP5_OK)
    {
        // Not connected, inform user
        Serial.println("Error: BMP581 not connected, check wiring and I2C address!");

        // Wait a bit to see if connection is established
        delay(1000);
    }

    Serial.println("BMP581 connected!");

    // Set the ODR frequency to 240 Hz
    pressureSensor.setODRFrequency(BMP5_ODR_30_HZ);
}

void loop()
{
    // wait until the sensor to get a first valid reading. For 128x it is 84.4 ms thus pick maximum waiting time for safety. 
    if(time == 0)
    {
        delay(10);
    }

    // Get 1000 measurements from the sensor
    if (time <= 1000){
        bmp5_sensor_data data = {0,0};
        int8_t err = pressureSensor.getSensorData(&data);

        // Check whether data was acquired successfully
        if(err == BMP5_OK)
        {
            // Acquisistion succeeded, print pressure
            float pressure = data.pressure;

            if(time == 1){
                p0 = pressure;
            }
            else{
                // international standard barometric formula
                float h = c1 * (1 - pow(((pressure + offset) / p0), c2));

                // output data in monitor
                Serial.println("Time: ");
                Serial.println(time);
                Serial.print("Pressure (Pa): ");
                Serial.println(pressure);
                Serial.print("Reference Pressure Used (Pa): ");
                Serial.println(p0);
                Serial.print("Offset Pressure (Pa): ");
                Serial.println(offset);
                Serial.print("Altitude (m): ");
                Serial.println(h);
                Serial.println("\n");
            }
        }
        else
        {
            // Acquisition failed, most likely a communication error (code -2)
            Serial.print("Error getting data from sensor! Error code: ");
            Serial.println(err);
        }

        // Required data every second.
        delay(1000);

        // increment time
        time++;
    }
}