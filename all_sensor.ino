#include <BH1750.h>
#include <Wire.h>
#include "Adafruit_SGP30.h"
#include <Adafruit_Sensor.h>
#include <L298N.h>
#include <GP2Y1010AU0F.h>
#include <Arduino.h>
#include <ESP32Firebase.h>
#include <WiFi.h>
#include <Adafruit_AHTX0.h>

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

  #define TFT_CS         13
  #define TFT_RST         4                                          
  #define TFT_DC         2

// Insert your network credentials
#define _SSID "Wifi"          // Your WiFi SSID
#define _PASSWORD "12345678"      // Your WiFi Password
#define REFERENCE_URL "https://esp8266test-f058e-default-rtdb.firebaseio.com"

Firebase firebase(REFERENCE_URL);

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;


int measurePin = 36;
int ledPin     = 15;
float dustDensity = 0;
const unsigned int IN1 = 26;
const unsigned int IN2 = 27;
const unsigned int IN3 = 34;
const unsigned int IN4 = 35;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Adafruit_AHTX0 aht;
GP2Y1010AU0F dustSensor(ledPin, measurePin); // Construct dust sensor global object
Adafruit_SGP30 sgp;
BH1750 lightMeter;
L298N motor(IN1, IN2);
L298N motor1(IN3, IN4);
uint32_t delayMS;

void setup(){
  Wire.begin(33, 32);
  Serial.begin(115200);
  lightMeter.begin();
  dustSensor.begin();
  lightMeter.configure(BH1750::CONTINUOUS_HIGH_RES_MODE);
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab
  sensor_t sensor;
  if (! sgp.begin()){
    Serial.println("Sensor not found :(");
    while (1);
  }
  Serial.print("Found SGP30 serial #");

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");


  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  // Connect to WiFi
  Serial.println();
  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(_SSID);
  WiFi.begin(_SSID, _PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
}
int counter = 0;
void loop() {
  float randomECValue = randomDecimal(400, 550, 0);
  float randomTValue = randomDecimal(40, 65, 0);

  //bh1750
  float lux = lightMeter.readLightLevel();
  
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
  //sgp30
  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  // Serial.print("TVOC "); Serial.print(randomTValue); Serial.print(" ppb\t");
  // Serial.print("eCO2 "); Serial.print(randomECValue); Serial.println(" ppm");
   Serial.print("TVOC "); Serial.print(sgp.TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(sgp.eCO2); Serial.println(" ppm");

  delay(1000);

  counter++;
  if (counter == 30) {
    counter = 0;

    uint16_t TVOC_base, eCO2_base;