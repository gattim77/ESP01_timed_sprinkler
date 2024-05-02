#include <Arduino.h>
#include <user_interface.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>




#define GPIO_PIN 4 // GPIO4/D2 
//apparently only D4 and D5 don't send a short pulse on reset
#define ANALOG_PIN A0


#define RTC_BASE 65
#define SLEEP_COUNTS_FOR_WATER 24 // water every 24 hours
const unsigned long WIFI_TIMEOUT = 15000; // 15 seconds

const char* SERVER_URL = "http://192.168.178.68/receive_data.php";
WiFiClient wifiClient; // Create a WiFiClient object


//functions defintions
void connectToWiFi(); 
void writetoDB(int humidity, int counter);


uint32_t sleepCount;


void setup() {
  Serial.begin(74880);
  //delay(1000);

  connectToWiFi();

  system_rtc_mem_read(RTC_BASE, &sleepCount, 4); // read counter
  sleepCount++;
  if (sleepCount >= SLEEP_COUNTS_FOR_WATER) {
    writetoDB (1,sleepCount);
  } else {
    writetoDB (0,sleepCount);
  }
  if (sleepCount >= SLEEP_COUNTS_FOR_WATER) {
    pinMode(GPIO_PIN, OUTPUT);
    //do watering and reset
    // Activate GPIO2
    Serial.println("Activate pump");
    digitalWrite(GPIO_PIN, HIGH);
    delay(2000); // Wait for 2 seconds
    // Deactivate GPIO2
    Serial.println("Deactivate pump");
    digitalWrite(GPIO_PIN, LOW);
     
    //reset the memory
    sleepCount = 0;
  }
  // store the counter
  system_rtc_mem_write(RTC_BASE, &sleepCount, 4); // write counter
  
  Serial.println(sleepCount);


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

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(SSID_NAME, SSID_PASSWORD);

  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED) {
    if (millis() - startTime >= WIFI_TIMEOUT) {
      Serial.println("WiFi connection timed out!");
      return;
    }
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void writetoDB(int humidity, int counter){

  // Create JSON payload
  StaticJsonDocument<200> jsonDocument;
  jsonDocument["humidity"] = humidity;
  jsonDocument["action_log"] = counter;

  // Serialize JSON to string
  String jsonString;
  serializeJson(jsonDocument, jsonString);

  // Send HTTP POST request to server
  HTTPClient http;
  http.begin(wifiClient, SERVER_URL);
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(jsonString);

  // Check for successful HTTP POST request
  if (httpResponseCode > 0) {
    Serial.print("HTTP POST request successful, response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("HTTP POST request failed, error code: ");
    Serial.println(httpResponseCode);
  }

  // Free resources
  http.end();


}