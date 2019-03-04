#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include "secrets.h"
#include <myesp.h>

const String ROOT = "/sensors";

const String POOL = "/pool";

const String POOL_LIGHT = "/light";
const String POOL_PUMP = "/pump";

#define POOL_LIGHT_PIN D1
#define POOL_PUMP_PIN D2


void setupFirebaseStreaming() {
  Firebase.stream(ROOT + POOL);
  println("Started streaming on " + ROOT + POOL);

  bool light = Firebase.getBool(ROOT + POOL + POOL_LIGHT);
  setLight(POOL_LIGHT_PIN, light);

  bool pump = Firebase.getBool(ROOT + POOL + POOL_PUMP);
  setLight(POOL_PUMP_PIN, pump);
}

void setup() {

  setupServices();
  setupFirebaseStreaming();

  pinMode(POOL_LIGHT_PIN, OUTPUT);
  pinMode(POOL_PUMP_PIN, OUTPUT);

  setLight(POOL_LIGHT_PIN, false);
  setLight(POOL_PUMP_PIN, false);
}

unsigned long lastTimeAvailableCalled = -1;
unsigned long MAX_ELLAPSED_TIME_BETWEEN_AVAILABLE_CALLS = 10 * 60 * 1000; // 10 mins in millis

void loop() {
  if (loopServices()) {
    setupFirebaseStreaming();
  }

  unsigned long currentTime = millis();
  unsigned long ellapsed = (currentTime - lastTimeAvailableCalled) / 1000;

  if (MAX_ELLAPSED_TIME_BETWEEN_AVAILABLE_CALLS < ellapsed) {
    reset();
  }
  
  if (Firebase.available()) {
    print("Time elapsed since last Firebase.available: ");
    print(ellapsed);
    println("s");
    
    lastTimeAvailableCalled = currentTime;
    FirebaseObject event = Firebase.readEvent();

    String eventType = event.getString("type");
    String path = event.getString("path");

    println("");
    println("");
    println("TYPE: " + eventType);
    println("PATH: " + path);

    JsonVariant v = event.getJsonVariant("data");
    String data = v.as<String>();
    println("DATA: " + data);

    if (eventType == "put" && path != "/") {
      if (path == POOL_LIGHT) {
        setLight(POOL_LIGHT_PIN, data == "true");
      }
      if (path == POOL_PUMP) {
        setLight(POOL_PUMP_PIN, data == "true");
      }
    } else {
      if (eventType == "put" || eventType == "patch") {
        String root = "";
        if (path == "/") {
          // prefix with /data
          root = "/data";
        }
        
        bool light = event.getBool(root + POOL_LIGHT);
        if (!event.failed()) {
          print("Light value changed. New value: ");
          println(light);
  
          setLight(POOL_LIGHT_PIN, light);
        }
  
        bool pump = event.getBool(root + POOL_PUMP);
        if (!event.failed()) {
          print("Pump value changed. New value: ");
          println(pump);
  
          setLight(POOL_PUMP_PIN, pump);
        }
      }
    }
  }
}
