#include <Arduino.h>
#include <user_interface.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
//#include <NTPClient.h>



#define GPIO_PIN 4 // GPIO4/D2 
//apparently only D4 and D5 don't send a short pulse on reset
#define ANALOG_PIN A0


#define RTC_BASE 65
#define SLEEP_COUNTS_FOR_WATER 24 // water every 24 hours
const unsigned long WIFI_TIMEOUT = 15000; // 15 seconds

const char* SERVER_URL = "http://192.168.178.68/receive_data.php";
const char* CONFIG_SERVER_URL = "http://192.168.178.68/configuration.php";

WiFiClient wifiClient; // Create a WiFiClient object

int ScheduleHour = 8;
int ScheduleMinutes = 0;
int SprinkleTimeDecSeconds = 20;
unsigned long timezone_seconds_offset = 3600; //by default 1 hr = CET

WiFiUDP ntpUDP;
//NTPClient timeClient(ntpUDP);
//const char* ntpServer = "pool.ntp.org";
const char* ntpServer = "time.google.com";
//const long gmtOffset_sec = 3600; // GMT offset in seconds (CET is UTC+1)

int sprinkle_action_trigger = 0;
double wait_amount=0;

//functions defintions
void connectToWiFi(); 
void writetoDB(int humidity, int counter);
void readConfigurationFromDB();
unsigned long getNextRecurrenceTime();
unsigned long getTimeDifference(unsigned long targetTime);

uint32_t sleepCount;


void setup() {
  Serial.begin(74880);
  //delay(1000);

  connectToWiFi();


  readConfigurationFromDB();
  

  //Initialize NTP client
  //timeClient.begin();
  //timeClient.setPoolServer(ntpServer);
  //timeClient.setTimeOffset(gmtOffset_sec);

  // Update time from NTP server
  //timeClient.update();
  //unsigned long epochTime = timeClient.getEpochTime();
  
  // Set timezone and start NTP
  //configTime(1, 3600, ntpServer);
  //Serial.println(ntpServer);

  // Wait for time to be synchronized
  while (time(nullptr) < 1000000000UL) {
    delay(1000);
    Serial.println("Waiting for time synchronization...");
    Serial.print("Current time: ");
    Serial.println(time(nullptr));
    Serial.println(now());
  }

  setTime(time(nullptr) + timezone_seconds_offset); //adding +2:00 for timezone and daylight saving

  //system_rtc_mem_read(RTC_BASE, &sleepCount, 4); // read counter
  //sleepCount++;
  
  // Calculate next recurrence time
  unsigned long nextRecurrenceTime = getNextRecurrenceTime();

  // Calculate time difference in seconds
  unsigned long timeDifference = getTimeDifference(nextRecurrenceTime);
  Serial.print("time difference: ");
  Serial.println(timeDifference);
  // if we are within 10 seconds from the next iteration the sprinkler is activated
  //+- 10 seconds
  if (timeDifference<10 || timeDifference>86390) { 
    sprinkle_action_trigger = 1;
    wait_amount = 7200e6; //wait two hours
    writetoDB (1,wait_amount/1e6);
  } else {
    // these will progressivley shorten the timer to get better precision
    switch (timeDifference) {
      case 0 ... 60: // Less than or equal to 900 seconds
        // Perform action for less than or equal to 900 seconds
        wait_amount = (timeDifference-5) *1e6;
        break;      
      case 61 ... 200: // Less than or equal to 900 seconds
        // Perform action for less than or equal to 900 seconds
        wait_amount = 55e6;
        break;      
      case 201 ... 450: // Less than or equal to 900 seconds
        // Perform action for less than or equal to 900 seconds
        wait_amount = 170e6;
        break;      
      case 451 ... 900: // Less than or equal to 900 seconds
        // Perform action for less than or equal to 900 seconds
        wait_amount = 420e6;
        break;
      case 901 ... 1800: // Between 901 and 1800 seconds
        // Perform action for between 901 and 1800 seconds
        wait_amount = 870e6;
        break;
      case 1801 ... 3600: // Between 1801 and 3600 seconds
        // Perform action for between 1801 and 3600 seconds
        wait_amount = 1770e6;
        break;
      case 3601 ... 10800: // Between 3601 and 10800 seconds
        // Perform action for between 3601 and 10800 seconds
        wait_amount = 3570e6;
        break;
      default:
        // Perform default action if timeDifference doesn't match any case
        wait_amount = 7200e6; //wait two hours
        break;
    }
    writetoDB (0,wait_amount/1e6);
  }


  
  
  //if (sleepCount > SLEEP_COUNTS_FOR_WATER) {
  //  writetoDB (1,sleepCount);
  //} else {
  //  writetoDB (0,sleepCount);
  //}


  if (sleepCount > SLEEP_COUNTS_FOR_WATER) {
    pinMode(GPIO_PIN, OUTPUT);
    //do watering and reset
    // Activate GPIO2
    Serial.println("Activate pump");
    digitalWrite(GPIO_PIN, HIGH);
    delay(SprinkleTimeDecSeconds*10); // Wait (time is in decimal seconds)
    // Deactivate GPIO2
    Serial.println("Deactivate pump");
    digitalWrite(GPIO_PIN, LOW);
     
    //reset the memory
    //sleepCount = 0;
  }
  // store the counter
  //system_rtc_mem_write(RTC_BASE, &sleepCount, 4); // write counter
  
  //Serial.println(sleepCount);


  //ESP.deepSleep(86400e6); //deepsleep 1 day (86400 seconds - 86400 x 10e6 microseconds)
  //this doesnt work as it;s too long. 
  //Serial.println(ESP.deepSleepMax());
  yield();
  //Serial.end();
  Serial.flush();  
  //ESP.deepSleep (3600e6); //1 hour
  ESP.deepSleep (wait_amount); //the amount set above
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


void readConfigurationFromDB() {
HTTPClient http;

  // Make GET request to API
  http.begin(wifiClient, CONFIG_SERVER_URL);
  int httpCode = http.GET();

  if (httpCode > 0) {
    // Parse JSON response
    String payload = http.getString();
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    Serial.print("payload: ");
    Serial.println(payload);
    // Extract configuration values
    ScheduleHour = doc["schedule_hour"];
    ScheduleMinutes = doc["schedule_minute"];
    SprinkleTimeDecSeconds = doc["sprinkle_decseconds"];
    timezone_seconds_offset = doc["timezone_offset"];
    // Print configuration values
    Serial.print("Schedule_hour: ");
    Serial.println(ScheduleHour);
    Serial.print("Schedule_Minute: ");
    Serial.println(ScheduleMinutes);
    Serial.print("Sprinlkle_decSeconds: ");
    Serial.println(SprinkleTimeDecSeconds);
    Serial.print("timezone_seconds_offset: ");
    Serial.println(timezone_seconds_offset);

  } else {
    Serial.print("Error reading configuration. HTTP code: ");
    Serial.println(httpCode);
  }

  http.end();


}

// Function to calculate next recurrence time
unsigned long getNextRecurrenceTime() {
  // Get current time in epoch format
  unsigned long currentTime = now();

  // Get the current date and time components
  int currentHour = hour(currentTime);
  int currentMinute = minute(currentTime);
  int currentSecond = second(currentTime);

  Serial.printf("%02d:%02d:%02d - %20d\n", currentHour, currentMinute, currentSecond, currentTime);

  // Calculate the next recurrence time for today
  unsigned long targetTimeToday = currentTime;
  if (currentHour > ScheduleHour || (currentHour == ScheduleHour && currentMinute >= ScheduleMinutes)) {
    // Target time for today has already passed, calculate for tomorrow
    targetTimeToday += (24 * 3600); // Add one day (24 hours) in seconds
  }
  targetTimeToday -= ((currentHour * 3600) + (currentMinute * 60) + currentSecond); // Subtract current time from the current day

  // Calculate target time for the next recurrence
  unsigned long nextRecurrenceTime = targetTimeToday + (ScheduleHour * 3600) + (ScheduleMinutes * 60);

  return nextRecurrenceTime;
}

// Function to calculate time difference in seconds
unsigned long getTimeDifference(unsigned long targetTime) {
  unsigned long currentTime = now();
  return targetTime > currentTime ? targetTime - currentTime : 0;
}
