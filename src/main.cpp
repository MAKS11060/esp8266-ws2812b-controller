#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <FastLED.h>

#include "./config.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

CRGB leds[NUM_LEDS];

uint32_t timer_led;
byte counter;

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else {  // U_SPIFFS
      type = "filesystem";
    }

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using
    // SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println();
  Serial.println("ESP8266 Chip Info:");
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId());
  Serial.print("Flash Chip ID: ");
  Serial.println(ESP.getFlashChipId());
  Serial.print("Flash Chip Size: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("Flash Chip Real Size: ");
  Serial.println(ESP.getFlashChipRealSize());
  Serial.print("Flash Chip Speed: ");
  Serial.println(ESP.getFlashChipSpeed());
  Serial.print("Cycle Count: ");
  Serial.println(ESP.getCycleCount());
  Serial.print("Free Heap: ");
  Serial.println(ESP.getFreeHeap());
  Serial.print("Sketch Size: ");
  Serial.println(ESP.getSketchSize());
  Serial.print("Free Sketch Space: ");
  Serial.println(ESP.getFreeSketchSpace());
  Serial.print("Sketch MD5: ");
  Serial.println(ESP.getSketchMD5());
  Serial.print("Reset Reason: ");
  Serial.println(ESP.getResetReason());

  // Setup LED
  FastLED.addLeds<WS2812B, PIN, GRB>(leds, NUM_LEDS)
      .setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(128);
}

void loop() {
  ArduinoOTA.handle();

  if (millis() - timer_led >= 400) {
    timer_led = millis();

    for (int i = 0; i < NUM_LEDS; i++) {
    //   leds[i] = CHSV(counter + i * 2, 255, 255);
      leds[i] = 0xffffff;
    }
    // counter++;

    FastLED.show();
  }
}
