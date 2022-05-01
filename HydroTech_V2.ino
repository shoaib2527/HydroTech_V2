#include <Arduino.h>
#include <WiFiManager.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiClient.h>
#include <Arduino_JSON.h>
Ticker ticker;
#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif
int LED = D2;
int MAIN_RELAY = D1;
String ID = "";
// You need to update USER_ID Field
String USER_ID = "47";
JSONVar device = null;
bool isDevice = false;
bool isDeviceConnected = false;
JSONVar service = null;
bool isService = false;
String jsonBuffer;

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(MAIN_RELAY, OUTPUT);
  wifiStuff();
  handleDevice();
}
void handleDevice()
{
  getDevice();
  if (JSON.typeof(device) == "undefined")
  {
    updateConnection(true, 0);
  }
}
long previousCycleMillis = 0;
long currentCycleMillis = 0;
long cycleDurationInSeconds = 0;
long serviceRemainingTimeInSeconds = 0;
long newServiceDuration = 0;
void loop()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    if (isDevice)
    {
      getService();
      if (isService)
      {
        while (isService)
        {
          Serial.println("Service isn't null...");
          Serial.print("Remamining Time: ");
          Serial.println((int)service["serviceInitialDuration"] - (int)service["serviceDuration"]);

          currentCycleMillis = millis();
          if (previousCycleMillis != 0)
          {
            cycleDurationInSeconds = (currentCycleMillis - previousCycleMillis) / 1000;
          }
          previousCycleMillis = currentCycleMillis;
          newServiceDuration = (int)service["serviceDuration"] + cycleDurationInSeconds;
          if (serviceRemainingTimeInSeconds == 0)
            serviceRemainingTimeInSeconds = (int)service["serviceInitialDuration"] - (int)service["serviceDuration"];
          else
            serviceRemainingTimeInSeconds = (int)service["serviceInitialDuration"] - newServiceDuration;
          if (serviceRemainingTimeInSeconds > 3)
          {
            turnMachineOn();
            Serial.println(service);
            String serviceStatus = (const char *)service["status"];
            Serial.println(service["status"]);
            Serial.println(serviceStatus);
            if (serviceStatus == "posted")
              updateService("{\"status\":\"running\",\"serviceDuration\":\"" + (String)newServiceDuration + "\"}");
            else
              updateService("{\"serviceDuration\":\"" + (String)newServiceDuration + "\"}");
            updateConnection(false, cycleDurationInSeconds);
          }
          else
          {
            turnMachineOff();
            updateService("{\"status\":\"completed\",\"serviceDuration\":\"" + (String)newServiceDuration + "\"}");
            clearService();
            updateConnection(false, cycleDurationInSeconds);
            previousCycleMillis = 0;
            currentCycleMillis = 0;
            cycleDurationInSeconds = 0;
            serviceRemainingTimeInSeconds = 0;
            newServiceDuration = 0;
          }
        }
      }
      else
      {
        updateConnection(false, 0);
        previousCycleMillis = 0;
        currentCycleMillis = 0;
        cycleDurationInSeconds = 0;
        serviceRemainingTimeInSeconds = 0;
        newServiceDuration = 0;
        Serial.println("Service is null...");
        turnMachineOff();
      }
    }
    else
    {
      handleDevice();
    }
    delay(1000);
  }
  else
  {
    ESP.reset();
  }
}
