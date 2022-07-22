#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <M5StickCPlus.h>
void setup() {
  // put your setup code here, to run once:
  m5.begin();
  M5.Lcd.print("Hello World");
  pinMode(10,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
    digitalWrite(10,HIGH);
    delay(1000);
    digitalWrite(10,LOW);
    delay(1000);
}