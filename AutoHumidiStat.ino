/*********************************************************************
  This is an example sketch for our Monochrome Nokia 5110 LCD Displays

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/338

  These displays use SPI to communicate, 4 or 5 pins are required to
  interface

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <stdlib.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 9
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
char caOutTemp[16] = {"OutTemp: 123456"};
char Humidity[16] = {"Humidit: 123456"};
char ServoStr[16] = {"SetServ: 123456"};
Servo myservo;  // create servo object to control a servo

int potpin = 0;  // analog pin used to connect the potentiometer
int val;    // variable to read the value from the analog pin

// Software SPI (slower updates, more flexible pin options):
// pin 7 - Serial clock out (SCLK)
// pin 6 - Serial data out (DIN)
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
//Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);

// Hardware SPI (faster, but must use certain hardware pins):
// SCK is LCD serial clock (SCLK) - this is pin 13 on Arduino Uno
// MOSI is LCD DIN - this is pin 11 on an Arduino Uno
// pin 5 - Data/Command select (D/C)
// pin 4 - LCD chip select (CS)
// pin 3 - LCD reset (RST)
Adafruit_PCD8544 display = Adafruit_PCD8544(5, 4, 3);
// Note with hardware SPI MISO and SS pins aren't used but will still be read
// and written to during SPI transfer.  Be careful sharing these pins!

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

void setup()   {
  myservo.attach(0);  // attaches the servo on pin 2 to the servo object

  display.begin();
  // init done

  // you can change the contrast around to adapt the display
  // for the best viewing!
  display.setContrast(16);

  display.display(); // show splashscreen
  delay(500);
  display.clearDisplay();   // clears the screen and buffer

  // write outdoor temperature
  testdrawchar();
  display.display();
  delay(500);
  display.clearDisplay();
}


void loop()
{
  
  int numberOfDevices;              // Number of temperature devices found
  DeviceAddress tempDeviceAddress;  // We'll use this variable to store a found device address

  // For testing purposes, reset the bus every loop so we can see if any devices appear or fall off
  sensors.begin();
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();

  Serial.print(" Parasite:");
  if (sensors.isParasitePowerMode()) Serial.print("ON ");
  else Serial.print("OFF ");

  Serial.print("Count:");
  Serial.print(numberOfDevices, DEC);
  // report parasite power requirements

  sensors.requestTemperatures(); // Send the command to get temperatures
  sensors.getAddress(tempDeviceAddress, 0);
  float fOutTempC = sensors.getTempC(tempDeviceAddress);
  dtostrf(fOutTempC, 5, 1, caOutTemp + 8);
  float SetHumidity = 0.785714*fOutTempC+39.2857;
  
  if (SetHumidity > 40){
    SetHumidity = 40;
  }
  dtostrf(SetHumidity, 5, 1, Humidity + 8);
  float ServoSet = -0.0333333 * SetHumidity * SetHumidity - 3.9359 * SetHumidity + 233.654;
  dtostrf(ServoSet, 5, 1, ServoStr + 8);
  // Loop through each device, print out temperature data
  for (int i = 0; i < numberOfDevices; i++)
  {
    // Search the wire for address
    if (sensors.getAddress(tempDeviceAddress, i))
    {
      // Output the device ID
      Serial.print(" #");
      Serial.print(i, DEC);
      Serial.print("=");
      float tempC = sensors.getTempC(tempDeviceAddress);
      Serial.print(tempC); // Converts tempC to Fahrenheit
    }
    //else ghost device! Check your power requirements and cabling

  }
  display.clearDisplay();
  DisplayTextAtCoords(0, 0,caOutTemp);
  DisplayTextAtCoords(0, 20,Humidity);
  DisplayTextAtCoords(0, 40,ServoStr);
  
  Serial.println("");

  //delay(500)
  //myservo.write((int)ServoSet);
  myservo.write((int)ServoSet);
}

void DisplayTextAtCoords(uint8_t iX, uint8_t iY, char * pcText) {
  display.setTextSize(1);
  display.setTextColor(BLACK);
  display.setCursor(iX, iY);

  display.println(pcText);
  display.display();
}


