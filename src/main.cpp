#include <Arduino.h>
#include <user_interface.h>

#define GPIO_PIN 4 // GPIO4/D2 
//apparently only D4 and D5 don't send a short pulse on reset
#define ANALOG_PIN A0


#define RTC_BASE 65
#define SLEEP_COUNTS_FOR_WATER 24 // water every 24 hours



uint32_t sleepCount;


void setup() {
  Serial.begin(74880);
  //delay(1000);


  system_rtc_mem_read(RTC_BASE, &sleepCount, 4); // read counter
  sleepCount++;
  if (sleepCount >= SLEEP_COUNTS_FOR_WATER) {
    pinMode(GPIO_PIN, OUTPUT);
    //do watering and reset
    // Activate GPIO2
    Serial.println("Activate pump");
    digitalWrite(GPIO_PIN, HIGH);
    delay(2500); // Wait for 2.5 seconds
    // Deactivate GPIO2
    Serial.println("Deactivate pump");
    digitalWrite(GPIO_PIN, LOW);
     
    //reset the memory
    sleepCount = 0;
  }
  // store the counter
  system_rtc_mem_write(RTC_BASE, &sleepCount, 4); // write counter
  
  Serial.println(sleepCount);


  //Serial.println("Going to sleep");
  //delay(1000); // Wait for 1 second
  //delay(600000); // Wait for 10 minutes
  //ESP.deepSleep(86400e6); //deepsleep 1 day (86400 seconds - 86400 x 10e6 microseconds)
  //this doesnt work as it;s too long. 
  //Serial.println(ESP.deepSleepMax());
  yield();
  //Serial.end();
  Serial.flush();  
  ESP.deepSleep (3600e6); //1 hour
  //ESP.deepSleep(2e6); //deepsleep 2 seconds (86400 seconds - 86400 x 10e6 microseconds)
  delay(10);


}

void loop() {

  //int sensorValue = analogRead(ANALOG_PIN);

  // Print the sensor value to the serial monitor
  //Serial.print("light: ");
  //Serial.println(sensorValue);
  
}