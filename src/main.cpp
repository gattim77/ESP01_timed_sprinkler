#include <Arduino.h>

#define GPIO_PIN 2 // GPIO2 is connected to pin 2 on ESP-01



void setup() {
  pinMode(GPIO_PIN, OUTPUT);
  
}

void loop() {
// Activate GPIO2
  Serial.print("Activate pump");
  digitalWrite(GPIO_PIN, HIGH);
  delay(4000); // Wait for 4 seconds
  // Deactivate GPIO2
  Serial.print("Deactivate pump");
  digitalWrite(GPIO_PIN, LOW);
  Serial.print("sleep");
  delay(600000); // Wait for 10 minutes
  
  Serial.print("*");

}