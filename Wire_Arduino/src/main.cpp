#include <Arduino.h>
#include <WiFi.h>
#include <SimpleDHT.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


const char *ssid = "Thanos's WLAN";   //REPLACE_WITH_YOUR_SSID
const char *password = "thanos@1998"; //REPLACE_WITH_YOUR_PASSWORD

String token = "bRPiqP7vWu9XuEHK";
String serverName = "https://api.iot-dashboard-thesis.space/api/metrics/";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// for DHT22,
//      VCC: 5V or 3V
//      GND: GND
//      DATA: 2
int pinDHT11 = 4;
SimpleDHT11 dht11(pinDHT11);

void jsonninator(float temperature, float humidity) {

  String postMessage;

  HTTPClient http;

    http.begin(serverName + token);
    http.addHeader("Content-Type", "application/json");

    const size_t CAPACITY = JSON_OBJECT_SIZE(1);

    StaticJsonDocument<200> doc;

    JsonObject obj = doc.createNestedObject("values");
    obj["temperature"] = temperature;
    obj["humidity"] = humidity;

    serializeJsonPretty(doc, postMessage);

    // serializeJson(doc, Serial);

    int httpCode = http.POST(postMessage);

    Serial.println(postMessage);

    if (httpCode > 0)
    {
      Serial.println();
      Serial.println(httpCode);
      if (httpCode == 201)
      {
        Serial.println("Hooray!");
      }
    }

}

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

  if ((WiFi.status() == WL_CONNECTED))
  {
    
    jsonninator(temperature,humidity);

  }
  delay(2500);
}
