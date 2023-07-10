#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include secrets.h


const char* ssid = STASSID;
const char* password = STAPSK;
const char* host = "OTA-LEDS";

int led_pin = LED_BUILTIN;

void setup() {
  Serial.begin(115200);

  pinMode(led_pin, OUTPUT);

  Serial.println("Booting");
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println("Retrying connection...");
  }

  ArduinoOTA.setHostname(host);

  /* configure  OTA server events */
  ArduinoOTA.onStart([]() {
    digitalWrite(led_pin, LOW);  // switch on led
  });

  ArduinoOTA.onEnd([]() {
    for (int i = 0; i < 1024; i++) {
      analogWrite(LED_BUILTIN, i);  //fade-off LED
      delay(2);
    }
  });

  ArduinoOTA.onError([](ota_error_t error) {
    for(int i = 0; i < 5; i++) {
      digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
      delay(1000);                      // wait for a second
      digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
      delay(1000);                      // wait for a second
    }
    (void)error;
    ESP.restart();
  });

  /* setup the OTA server */
  ArduinoOTA.begin();
  Serial.println("Ready");
  digitalWrite(led_pin, HIGH);    /* switch off led */

 }

void loop() {
  ArduinoOTA.handle();
}
