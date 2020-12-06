#include <Arduino.h>
#include <WiFi.h>
#include <SimpleDHT.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char *ssid = ""; //REPLACE_WITH_YOUR_SSID
const char *password = "thanos@1998"; //REPLACE_WITH_YOUR_PASSWORD

String serverName = "http://192.168.1.106:1880/update-sensor";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// for DHT22,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 4;
SimpleDHT11 dht11(pinDHT11);

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  // start working...
  Serial.println("=================================");
  Serial.println("Sample DHT11...");

  // read without samples.
  // @remark We use read2 to get a float data, such as 10.1*C
  //    if user doesn't care about the accurate data, use read to get a byte data, such as 10*C.
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT11 failed, err=");
    Serial.println(err);
    delay(2000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((float)temperature);
  Serial.print(" *C, ");
  Serial.print((float)humidity);
  Serial.println(" RH%");

  // DHT11 sampling rate is 0.5HZ.

  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay)
  {
    //Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {
      HTTPClient http;

      String serverPath = serverName + temperature;

      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();

      if (httpResponseCode > 0)
      {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      else
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}