#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include "secrets.h"
#include <myesp.h>

const String ROOT = "/sensors";

const String POOL_LIGHT = "/pool/light";

#define POOL_PIN D1

void setupFirebaseStreaming()
{
  Firebase.stream(ROOT + POOL_LIGHT);
  println("Started streaming on " + ROOT + POOL_LIGHT);

  bool init = Firebase.getBool(ROOT + POOL_LIGHT);
  setLight(POOL_PIN, init);
}

void setup()
{

  setupServices();
  setupFirebaseStreaming();

  pinMode(POOL_PIN, OUTPUT);
}

void loop()
{
  if (loopServices())
  {
    setupFirebaseStreaming();
  }

  if (Firebase.available())
  {
    FirebaseObject event = Firebase.readEvent();

    String eventType = event.getString("type");
    println("type: " + eventType);
    if (eventType == "put")
    {
      String path = event.getString("path");
      println("path: " + path);
      if (path == "/")
      {

        bool light = event.getBool("data");
        print("Light value changed. New value: ");
        println(light);

        setLight(POOL_PIN, light);
      }
    }
  }
}