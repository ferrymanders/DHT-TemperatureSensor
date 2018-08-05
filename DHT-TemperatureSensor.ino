/*
 * DHT-TemperatureSensor
 * By : Ferry Manders
 * https://github.com/ferrymanders
 * 
 * Requirements :
 * - DHT-sensor-library : https://github.com/adafruit/DHT-sensor-library
 * 
 */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Below are my custom settings, if you want to use this script remove the next include
#include <WiFiSettings.h>

// Uncomment lines below to setup wifi
// const char* ssid     = "SSID";       // WiFi SSID
// const char* password = "WiFiPassword";  // WiFi Password


#define DHTTYPE DHT11 // Set DHT Type : DHT11, DHT21 or DHT22
#define DHTPIN  D4 // Set which pin the DHT module is connected to, you can usually find this on the board
 
// ***** Do not edit below this line ***************

ESP8266WebServer server(80);
DHT dht(DHTPIN, DHTTYPE, 11);
 
float humidity, temp_f, temp_c;
String webString="";     // String to display

unsigned long previousMillis = 0;
const long interval = 2000;

void setup(void)
{
  Serial.begin(115200);  // Serial connection from ESP-01 via 3.3v console cable

  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.print("\n\r \n\rWorking to connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  dht.begin();
   
  server.on("/", [](){
    getTemperature();
    webString = "{\"temp\":\"" + String(temp_c,2) + "\",\"humidity\":\"" + String(humidity,2) + "\"}";
    server.send(200, "application/json", webString);
  });

  server.begin();
  Serial.println("HTTP server started");
}
 
void loop(void)
{
  server.handleClient();
} 
 
void getTemperature() {
  unsigned long currentMillis = millis(); 
  if(currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;   
    humidity = dht.readHumidity();
    temp_f = dht.readTemperature(true);
    temp_c = dht.readTemperature();
    if (isnan(humidity) || isnan(temp_f) || isnan(temp_c)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}
