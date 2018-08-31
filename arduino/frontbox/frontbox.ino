#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include "secrets.h"
#include <myesp.h>

const String ROOT = "/sensors"; 

const String FRONT_LIGHT = "/front/light";

#define FRONT_PIN D1
#define TREE_PIN D2

#define ON LOW
#define OFF HIGH

void setupFirebaseStreaming() {
  Firebase.stream(ROOT + FRONT_LIGHT);
  println("Started streaming on " + ROOT + FRONT_LIGHT);

  bool init = Firebase.getBool(ROOT + FRONT_LIGHT);
  setLight(FRONT_PIN, init);
  setLight(TREE_PIN, init);
}

void setup() {

  setupServices();
  setupFirebaseStreaming();
  
  pinMode(FRONT_PIN, OUTPUT);
  pinMode(TREE_PIN, OUTPUT);


  setLight(FRONT_PIN, true);
  delay(1000);
  setLight(FRONT_PIN, false);
  delay(1000);
  setLight(FRONT_PIN, true);
  delay(1000);
  setLight(FRONT_PIN, false);

 
} 

void loop() {
  if(loopServices()) {
    setupFirebaseStreaming();
  }
  
  if (Firebase.available()) {
    FirebaseObject event = Firebase.readEvent();
    
    String eventType = event.getString("type");
    println("type: " + eventType);
    if (eventType == "put") {
      String path = event.getString("path");
      println("path: " + path);
      if (path == "/") {  

        bool light = event.getBool("data");
        print("Light value changed. New value: ");
        println(light);

        setLight(FRONT_PIN, light);
        setLight(TREE_PIN, light);
      }
    }
  }
}