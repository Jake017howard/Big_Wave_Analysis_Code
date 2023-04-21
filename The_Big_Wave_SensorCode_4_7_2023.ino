#include "Wire.h"
#include <SPI.h>
#include <Adafruit_LSM9DS1.h>
#include <Adafruit_Sensor.h>
#define TCAADDR 0x70
#include <SD.h>
#include <LiquidCrystal.h>
#include "RTClib.h"

//////////////////////////////////////////////////////////////////////
//////////////////////// global parameters ///////////////////////////
//////////////////////////////////////////////////////////////////////

//instantiate objects & global variables
//LCD
const int rs = 12, en = 11, d4 = 36, d5 = 34, d6 = 32, d7 = 30;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//millis setup
unsigned long myTime;

//microSD
const int microSD_CS_pin = 53;
const int microSD_CLK_pin = 52;
const int microSD_DO_pin = 50;
const int microSD_DI_pin = 51;
File sensorData;
char filename[ ] = "BOI.CSV";
//clear file

//RTC
RTC_DS3231 rtc;

//Accelerometers
// assign names to each sensor
Adafruit_LSM9DS1 lsm7 = Adafruit_LSM9DS1(7);
Adafruit_LSM9DS1 lsm6 = Adafruit_LSM9DS1(6);
Adafruit_LSM9DS1 lsm5 = Adafruit_LSM9DS1(5);
Adafruit_LSM9DS1 lsm4 = Adafruit_LSM9DS1(4);
Adafruit_LSM9DS1 lsm1 = Adafruit_LSM9DS1(1);

//////////////////////////////////////////////////////////////////////
///////////////////////// setup function /////////////////////////////
//////////////////////////////////////////////////////////////////////

//Setup Sensors
void setupSensor()
{
  //lsm7-4 are the data-retriving sensors
  lsm7.setupAccel(lsm7.LSM9DS1_ACCELRANGE_2G);

  lsm6.setupAccel(lsm6.LSM9DS1_ACCELRANGE_2G);

  lsm5.setupAccel(lsm5.LSM9DS1_ACCELRANGE_2G);

  lsm4.setupAccel(lsm4.LSM9DS1_ACCELRANGE_2G);

  //lsm1 is the control sensor
  lsm1.setupAccel(lsm1.LSM9DS1_ACCELRANGE_2G);

  Serial.print("\n Accelerometers intitiation ready!");
}


  

//Setup Multiplexer
void tcaselect(uint8_t i) {
  if (i > 7) return;
 
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();
}

  
//Overall Setup
void setup() {
  // put your setup code here, to run once:
  while (!Serial);
  delay(1000);

  Wire.begin();
  
  Serial.begin(9600);
  // Serial.println("\nTCAScanner ready!");



  //Initialize Sensors...
  tcaselect(7);
  if(!lsm7.begin()){
    Serial.print("lsm7 failed");
    lcd.print("lsm7 failed");
    while(1);
  }
  tcaselect(6);
  if(!lsm6.begin()){
    Serial.print("lsm6 failed");
    lcd.print("lsm6 failed");
    while(1);
  }
  tcaselect(5);
  if(!lsm5.begin()){
    Serial.print("lsm5 failed");
    lcd.print("lsm5 failed");
    while(1);
  }
  tcaselect(4);
  if(!lsm4.begin()){
    Serial.print("lsm4 failed");
    lcd.print("lsm4 failed");
    while(1);
  }
  tcaselect(1);
  if(!lsm1.begin()){
    Serial.print("lsm1 failed");
    lcd.print("lsm1 failed");
    while(1);
  }

  pinMode(microSD_CS_pin, OUTPUT);
  if (!SD.begin(microSD_CS_pin)) {
    Serial.println("Uh oh... microSD initialization failed!");
    lcd.print("microSD failed");
    while (1);
  }  
      Serial.println("\n done");
      lcd.print("Setup Complete");
      delay(500);

  if(SD.exists(filename)){
  SD.remove(filename);
  delay(100);
   }
    // wait for real time clock to be ready
  while(! rtc.begin()) {}

  // the following line sets the RTC to the date & time this sketch was compiled
//set RTC to the Date and Time that this file was complied
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = rtc.now();

//Time Span args - days, hours, minutes, seconds. 7 is a fine tune amount, can be adjusted
  DateTime future (now + TimeSpan(0, 0, 0, 7));


//set the RTC with explicti date & time
  rtc.adjust(DateTime(future.year(), future.month(), future.day(), future.hour(), future.minute(), future.second()));

  Serial.println("check done");
  
  
}

//////////////////////////////////////////////////////////////////////
//////////////////////////// loop function ///////////////////////////
//////////////////////////////////////////////////////////////////////

void loop() {
  sensors_event_t a, m, g, temp;
  sensorData = SD.open(filename, FILE_WRITE);

  //// Calibration Loop ////
  int yuh = 0;
  lcd.clear();
  lcd.print ( "Calibrating...");
  while (yuh < 100){

    tcaselect(7);
    lsm7.read();
    lsm7.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(6);
    lsm6.read();
    lsm6.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(5);
    lsm5.read();
    lsm5.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(4);
    lsm4.read();
    lsm4.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(1);
    lsm1.read();
    lsm1.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    DateTime now = rtc.now();
    sensorData.print(now.second());
    sensorData.print(",");
    myTime = millis();
    sensorData.print(myTime);

    sensorData.print("\n");
    
    // Below delay is for testing only
    delay(5);
    yuh += 1;
    }

  //// Data Loop ////
  int j=0;
  lcd.clear();
  lcd.print("Taking Data");
  while(j<900){
    
    tcaselect(7);
    lsm7.read();
    lsm7.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(6);
    lsm6.read();
    lsm6.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(5);
    lsm5.read();
    lsm5.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(4);
    lsm4.read();
    lsm4.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    tcaselect(1);
    lsm1.read();
    lsm1.getEvent(&a, &m, &g, &temp);
    sensorData.print(a.acceleration.x);
    sensorData.print(",");
    sensorData.print(a.acceleration.y);
    sensorData.print(",");
    sensorData.print(a.acceleration.z);
    sensorData.print(",");

    DateTime now = rtc.now();
    sensorData.print(now.second());
    sensorData.print(",");
    myTime = millis();
    sensorData.print(myTime);

    sensorData.print("\n");

    // Below delay is for testing only
    delay(5);
    j += 1;

  }
  sensorData.close();
  lcd.clear();
  delay(1000);
  int k = 1;
  while(k < 2);
    Serial.print("Done writing");
    lcd.print("All Done!");
    delay(10000);
  
return;
}
