#include <WiFiNINA.h>
#include <Wire.h>
#include <BH1750.h>

char wifiSSID[] = "Ag"; 
char wifiPass[] = "Football";

WiFiClient wifiClient;

BH1750 sensor; // Instance of the BH1750 light sensor library
int ledPin = 3;
String httpRequestData = "?val1=57&val2=25"; 
char serverName[] = "maker.ifttt.com"; // IFTTT server
String webhookURL = "https://maker.ifttt.com/trigger/Lightsensor/with/key/bJ-MCeoICdbnuZYyKy2MDfZwsY_8bGQaALNEmvxrjWN"; // Webhook URL

void setup() 
{
  Serial.begin(9600); 
  Serial.println("Starting setup...");
  
  pinMode(ledPin, OUTPUT);
  
  Wire.begin();
  if (sensor.begin()) {
    Serial.println("Light sensor initialized successfully");
  } else {
    Serial.println("Failed to initialize light sensor");
  }
  
  // Connect to WiFi network
  initializeWiFi();
}

void loop() {
  Serial.println("Entering loop...");

  float lightLevel = sensor.readLightLevel();
  Serial.print("Light Level: ");
  Serial.println(lightLevel);
  
  if (lightLevel >= 360) {
    sendNotification("Daylight");
  } else if (lightLevel < 80) {
    sendNotification("Nighttime");
  }
  delay(1000); // Pause for a while before reading the light level again
}

void initializeWiFi() {
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(wifiSSID, wifiPass);
    delay(5000); // Retry every 5 seconds
    Serial.print(".");
  }
  Serial.println("\nSuccessfully connected to WiFi");
}

void sendNotification(String event) 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
    initializeWiFi();
  }
  
  Serial.print("Sending notification for event: ");
  Serial.println(event);
  
  if (wifiClient.connect(serverName, 80)) {
    wifiClient.println("GET " + webhookURL + httpRequestData + " HTTP/1.1");
    wifiClient.println("Host: " + String(serverName));
    wifiClient.println("Connection: close");
    wifiClient.println();
    delay(500);
    wifiClient.stop();
    Serial.println("Notification sent successfully");
  } else {
    Serial.println("Failed to connect to server");
  }
}