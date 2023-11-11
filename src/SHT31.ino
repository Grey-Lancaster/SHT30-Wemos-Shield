/*************************************************** 
  Example for the SHT31-D Humidity & Temp Sensor with Web Server

  Designed specifically to work with the SHT31-D sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  This example integrates a web server to display sensor data and auto-refreshes every 5 seconds.
 ***************************************************/

#include <ESP8266WiFi.h>  // Use <WiFi.h> for ESP32
#include <ESP8266WebServer.h>  // Use <WebServer.h> for ESP32
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

// Replace with your network credentials
const char* ssid = "shop2";
const char* password = "mine0313";

ESP8266WebServer server(80);  // Use WebServer server(80) for ESP32

// Sensor setup
bool enableHeater = false;
uint8_t loopCnt = 0;
Adafruit_SHT31 sht31 = Adafruit_SHT31();

void handleRoot() {
  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  float tF = ((t * 9.0) / 5.0) + 32 - 3;  // Adjusted Fahrenheit value

  String html = "<html><head><meta http-equiv='refresh' content='5'></head><body>";
  html += "<h1>SHT31 Sensor Data At the Grey Farm</h1>";
  html += "<p>Temperature: " + String(tF) + " &deg;F</p>";
  html += "<p>Humidity: " + String(h) + " %</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(9600);
  // Initialize the WiFi and web server
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);  // Define the handling function for the root path
  server.begin();

  if (!sht31.begin(0x45)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  Serial.print("Heater Enabled State: ");
  if (sht31.isHeaterEnabled())
    Serial.println("ENABLED");
  else
    Serial.println("DISABLED");
}

void loop() {
  server.handleClient();

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();

  if (loopCnt >= 10) {
    enableHeater = !enableHeater;
    sht31.heater(enableHeater);
    loopCnt = 0;
  }
  loopCnt++;
}
