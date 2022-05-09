#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>


//WIFI settings
const char *ssid = "YOUR_WIFI_SSID";
const char *password = "YOUR_WIFI_PASSWORD";


const String crypto_currency="Titano";
const String crypto_currency_api="titano";
const String real_currency_vale="usd";
const String real_currency_symbol="$";
int net_error=0;



void setup() {
  M5.begin();
  M5.Lcd.setRotation(3); 
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextSize(2);
  Serial.begin(115200);
  delay(4000);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(5000);
    Serial.println("Connecting to WiFi..");
  }
 
  Serial.println("Connected to the WiFi network");
 
}
 
void loop() {
  int updates=0;
  if ((WiFi.status() == WL_CONNECTED)) { //Check the current connection status
    
    HTTPClient http;
 
    http.begin("https://api.coingecko.com/api/v3/coins/markets?vs_currency="+real_currency_vale+"&ids="+crypto_currency_api+"&order=market_cap_desc&per_page=100&page=1&sparkline=false"); //Specify the URL
    int httpCode = http.GET();                                        //Make the request
 
    if (httpCode > 0) { //Check for the returning code
      
        String payload = http.getString();
        //Serial.println(httpCode);
        //Serial.println(payload);
      
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);
        if (error) {
          net_error++;
          if (net_error > 4) {
            setup();
            net_error=0;
          }
          return;
        }
          
        
        
        double crypto_value = doc[0]["current_price"];
        double crypto_price_change_percentage_24h = doc[0]["price_change_percentage_24h"];
        int crypto_high_24h = doc[0]["high_24h"];
        int crypto_low_24h = doc[0]["low_24h"];
        String str_crypto_value=String(crypto_value);
        String str_crypto_price_change_percentage_24h="";
        if (crypto_price_change_percentage_24h > 0) {
          str_crypto_price_change_percentage_24h="+"+String(crypto_price_change_percentage_24h);
        } else {
          str_crypto_price_change_percentage_24h=String(crypto_price_change_percentage_24h);
        }
        
        String str_crypto_high_24h=String(crypto_high_24h);
        String str_crypto_low_24h=String(crypto_low_24h);
       
        
       
        if (crypto_price_change_percentage_24h > 0) {
          M5.Lcd.fillScreen(TFT_GREEN);
        }
        if (crypto_price_change_percentage_24h > 20) {
          M5.Lcd.fillScreen(TFT_BLUE);
        }
        if (crypto_price_change_percentage_24h < 0) {
          M5.Lcd.fillScreen(TFT_YELLOW);
        }
        if (crypto_price_change_percentage_24h < -10) {
          M5.Lcd.fillScreen(TFT_RED);
        }
       
        M5.Lcd.setCursor(1, 0);
        M5.Lcd.setTextSize(2);
        M5.Lcd.println(crypto_currency);
        M5.Lcd.setTextSize(3);
        M5.Lcd.println(str_crypto_value+real_currency_symbol);
        M5.Lcd.setCursor(1, 31);
        M5.Lcd.setTextSize(2);
        M5.Lcd.println();
        M5.Lcd.setCursor(1, 43);
        M5.Lcd.println(str_crypto_low_24h);
        M5.Lcd.println(str_crypto_high_24h);
        M5.Lcd.setCursor(65, 50);
        M5.Lcd.setTextSize(3);
        str_crypto_price_change_percentage_24h.remove(5, 1);
        M5.Lcd.println(str_crypto_price_change_percentage_24h);
        
        
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
    
  }
  
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 75);
  while (updates < 100) {
    // do something repetitive 200 times
    M5.Lcd.println("..");
    M5.Lcd.setCursor(updates, 75);
    updates=updates+2;
    delay(1000);
  }
  
  
 
}
