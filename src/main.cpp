#include <Arduino.h>

#define GPIO_PIN 13 // GPIO13/D7 is connected to pin 13 on ESP12
#define ANALOG_PIN A0


void setup() {
  //Serial.begin(9600);
  pinMode(GPIO_PIN, OUTPUT);
}

void loop() {

int sensorValue = analogRead(ANALOG_PIN);

  // Print the sensor value to the serial monitor
  //Serial.print("light: ");
  //Serial.println(sensorValue);

  // Activate GPIO2
  Serial.println("Activate pump");
  digitalWrite(GPIO_PIN, HIGH);
  delay(4000); // Wait for 4 seconds
  // Deactivate GPIO2
  Serial.println("Deactivate pump");
  digitalWrite(GPIO_PIN, LOW);
  Serial.println("sleep");
  delay(1000); // Wait for 1 second
  //delay(600000); // Wait for 10 minutes
  
}