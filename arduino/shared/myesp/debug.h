#ifndef MyDebug_h
#define MyDebug_h

#include "TelnetStream.h"

void setupDebug() {
  Serial.begin(115200);
  TelnetStream.begin();
}

void print(String p) {
  Serial.print(p);
  TelnetStream.print(p);
}

void println(String p) {
  Serial.println(p);
  TelnetStream.println(p);
}

void print(boolean p) {
  Serial.print(p);
  TelnetStream.print(p);
}

void println(boolean p) {
  Serial.println(p);
  TelnetStream.println(p);
}

#endif