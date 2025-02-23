#include <BH1750.h>

#include <Adafruit_GFX.h>    
#include <Adafruit_ST7735.h> 
#include <Adafruit_ST7789.h> 
#include <SPI.h>
#include <Wire.h>
#define TFT_CS        13
#define TFT_RST        4 
#define TFT_DC         2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  Serial.begin(9600);
tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

}

void loop() {
  
  uint8_t AQI = 5;
  uint16_t TVOC = 4480;
  uint16_t ECO2 = 1622;
  float dustDensity = 1000;
  float temp = 36;
  float humidity = 60;
  
  tft.fillScreen(ST77XX_BLACK);
  tvocLCD(TVOC);
  eco2LCD(ECO2);
  pmLCD(dustDensity);
  tempAndHumLCD(temp, humidity);
  aqiLCD(AQI);
  delay(2000);
}
void tvocLCD(int TVOC){
  tft.fillCircle(30, 30, 30, ST7735_YELLOW);
  tft.setCursor(8, 15);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  tft.print("TVOC");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  if(TVOC >= 10 && TVOC < 100){
    tft.setCursor(20, 35);
  }else if(TVOC >= 100 && TVOC < 1000){
    tft.setCursor(15, 35);
  }else if(TVOC > 1000){
    tft.setCursor(8, 35);
  }else tft.setCursor(25, 35);
  tft.print(TVOC);
}
void eco2LCD(int ECO2){
  tft.fillCircle(92, 60, 30, ST7735_GREEN);
  tft.setCursor(70, 45);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  tft.print("ECO2");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  if(ECO2 >= 100 && ECO2 < 1000){
    tft.setCursor(77, 65);
  }else if(ECO2 > 1000){
    tft.setCursor(65, 65);
  }
  tft.print(ECO2);
}
void pmLCD(float dustDensity){
  tft.fillCircle(30, 95, 30, ST7735_CYAN);
  tft.setCursor(7, 80);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  tft.print("PM2");
  tft.setTextSize(1,2);
  tft.setCursor(40, 82);
  tft.print(".");
  tft.setCursor(46, 80);
  tft.setTextSize(2);
  tft.print("5");
  tft.setTextSize(2);
  tft.setTextColor(ST7735_BLACK);
  if(dustDensity >= 10 && dustDensity < 100){
    tft.setCursor(20, 100);
  }else if(dustDensity >= 100 && dustDensity < 1000){
    tft.setCursor(15, 100);
  }else tft.setCursor(25, 100);
    tft.print(round(dustDensity));
}
void tempAndHumLCD(float temp, float humidity){
  tft.drawRoundRect(64, 95, 63, 30, 10, ST7735_WHITE);
  tft.setCursor(75, 102);
  tft.setTextSize(1,2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(round(temp));
  tft.print("C ");
  tft.print(round(humidity));
  tft.print("%");
}
void aqiLCD(int AQI){
  tft.setTextColor(ST7735_BLACK);
  tft.setTextSize(2);
  tft.setCursor(10, 135);
  switch (AQI) {
    case 0:
tft.fillRoundRect(6, 130, 115, 25, 10, ST7735_GREEN);
      tft.print("Good");
      break;
    case 1: 
      tft.fillRoundRect(6, 130, 115, 25, 10, ST7735_YELLOW);
      tft.print("Moderate");
      break;
    case 2: 
      tft.fillRoundRect(6, 130, 115, 25, 10, ST7735_ORANGE);
      tft.print("Poor");
      break;
    case 3: 
      tft.fillRoundRect(6, 130, 115, 25, 10, ST7735_RED);
      tft.print("Unhealthy");
      break;
    case 4: 
      tft.fillRoundRect(6, 130, 115, 25, 10, ST7735_MAGENTA);
      tft.print("Hazardous");
      break;
    default:
      tft.fillRoundRect(6, 130, 115, 25, 10, ST7735_CYAN);
      tft.print("No Value");
      break;
  }
}
