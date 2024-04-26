/*************************************************** 
  Example for the SHT31-D Humidity & Temp Sensor with Web Server, mDNS, and Temperature Calibration

  Designed specifically to work with the SHT31-D sensor from Adafruit
  ----> https://www.adafruit.com/products/2857

  This example integrates a web server to display sensor data, auto-refreshes every 60 seconds, shows battery voltage, uses mDNS for easy access, and allows for temperature calibration.
 ***************************************************/

#include <ESP8266WiFi.h>  // Use <WiFi.h> for ESP32
#include <ESP8266WebServer.h>  // Use <WebServer.h> for ESP32
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <ESP8266mDNS.h>  // mDNS library

// Replace with your network credentials
const char* ssid = "xxxx";
const char* password = "xxxx";

ESP8266WebServer server(80);  // Use WebServer server(80) for ESP32

// Sensor setup
bool enableHeater = false;
uint8_t loopCnt = 0;
Adafruit_SHT31 sht31 = Adafruit_SHT31();
float tempCalibration = 0.0;

float readBatteryVoltage() {
  int sensorValue = analogRead(A0);
  float voltage = sensorValue * (4.2 / 1023.0);  // Convert analog reading to voltage
  return voltage;
}

void handleRoot() {
  float t = sht31.readTemperature();
  t += tempCalibration;  // Apply calibration
  float h = sht31.readHumidity();
  float tF = ((t * 9.0) / 5.0) + 32 - 3;  // Adjusted Fahrenheit value
  float batteryVoltage = readBatteryVoltage();

  String html = "<html><head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>";
  //.String html = "<html><head><meta http-equiv='refresh' content='60'></head><body>";
  html += "<meta http-equiv='refresh' content='60'></head><body>";
  html += "<h1>SHT31 Sensor Data</h1>";
  html += "<form action='/calibrate' method='POST'>";
  html += "<label for='calibration'>Temperature Calibration (°F):</label>";
  html += "<input type='number' step='0.01' name='calibration' value='0'>";
  html += "<input type='submit' value='Calibrate'>";
  html += "</form>";
  html += "<p>Temperature: " + String(tF) + " &deg;F</p>";
  html += "<p>Humidity: " + String(h) + " %</p>";
  html += "<p>Battery Voltage: " + String(batteryVoltage) + " V</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleCalibration() {
  if (server.hasArg("calibration")) {
    tempCalibration = server.arg("calibration").toFloat();
     String html = "<html><head><meta http-equiv='Content-Type' content='text/html; charset=UTF-8'>";
    server.send(200, "text/plain", "Calibration set to: " + server.arg("calibration") + " °F");
  } else {
    server.send(500, "text/plain", "Invalid request");
  }
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
  Serial.println("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  // Start mDNS with the hostname "esp8266"
  if (MDNS.begin("budro")) {
    Serial.println("mDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/calibrate", HTTP_POST, handleCalibration);
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
  MDNS.update();  // Keep the mDNS responder updated
  // Loop code if needed
  // ...
}
