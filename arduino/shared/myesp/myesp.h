#ifndef MyESP_h
#define MyESP_h

#include "debug.h"
#include "OTA.h"
#include "light.h"

void (*reset)(void) = 0; // reset function @ address 0

boolean connectToWifi() {
  WiFi.hostname(HOST_NAME);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  short i = 0;
  while (WiFi.status() != WL_CONNECTED && i++ < 50) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to wifi");
    Serial.print("Connect with IP: ");
    Serial.println(WiFi.localIP());
    return false;
  } else {
    Serial.print("Connected to wifi with IP: ");
    Serial.println(WiFi.localIP());
    return true;
  }
}

short firebaseFailures = 0;
const short FIREBASE_RESET_TRIGGER = 5;

boolean connectToFirebase() {
  println("Beginning Firebase connection");

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  if (Firebase.failed()) {
    print("Firebase connection failed: ");
    println(Firebase.error());
    firebaseFailures++;
    if (FIREBASE_RESET_TRIGGER <= firebaseFailures) {
      reset();
    }
    return false;
  } else {
    println("Connected to Firebase!");
    firebaseFailures = 0;
    return true;
  }
}

boolean setupServices() {
  if(connectToWifi()) {
    setupDebug();
    setupOTA();
    return connectToFirebase();
  } else {
    return false;
  }
}

boolean loopServices() {
  if (WiFi.status() != WL_CONNECTED) {
    return setupServices();
  } else {
    boolean connectedToFirebase = false;
    if (Firebase.failed()) {
      connectedToFirebase = connectToFirebase();
    }
    loopOTA();
    return connectedToFirebase;
  } 
}

#endif