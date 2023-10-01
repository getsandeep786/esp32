#include <WiFi.h>
#include "time.h"

const char* ssid       = "your-WIFI-username";
const char* password   = "password";

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 19800; //adjusted this value to IST
const int   daylightOffset_sec = 0; //adjusted this value to IST
static uint8_t RelayPin1 = 23;

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  char timeWeekDay[4];
  strftime(timeWeekDay,4, "%A", &timeinfo);
  Serial.println(timeWeekDay);
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  pinMode(RelayPin1, OUTPUT);
  
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  // Set the Relays GPIOs as output mode
  digitalWrite(RelayPin1, HIGH);
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
  digitalWrite(RelayPin1, HIGH);
}
