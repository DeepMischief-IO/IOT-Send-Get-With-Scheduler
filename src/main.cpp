#include <Arduino.h>
#include <WiFiManager.h>
#include <TaskScheduler.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

Scheduler runner;
WiFiManager wifiManager; 
WiFiClient client;

void blinkLed();
void sendData();
void getData();

//for task
Task t1(500,TASK_FOREVER,&blinkLed);      //500ms LED
Task t2(30000,TASK_FOREVER,&sendData); //send data 30 seconds
Task t3(45000,TASK_FOREVER,&getData);   //get data 45 seconds

void setup() {
  Serial.begin(115200);

  pinMode(LED_BUILTIN,OUTPUT);
  // wifiManager.resetSettings();//if you want to reset the save wifi info
  wifiManager.autoConnect("CONNECT WIFI","12341234");

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WIFI");

    //add the task to the scheduler
    // runner.addTask(t1);
    runner.addTask(t2);
    runner.addTask(t3);

    // Enable tasks
    // t1.enable();
    digitalWrite(LED_BUILTIN,LOW);
    t2.enable();
    t3.enable();
  }else{
    
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("Failed to connect to wifi");
  }
  

}

void loop() {
  runner.execute();
}
// Taks to blink led
void blinkLed(){
  digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
}

// Task to send data
void sendData(){
  if (WiFi.status() == WL_CONNECTED)
  { 
    Serial.println("Sending Data..");

    HTTPClient http;
  
    http.begin(client,"http://jsonplaceholder.typicode.com/posts");

    // Specify content type header
    http.addHeader("Content-Type","application/json");
    //Prepare JSON payload
    String httpRequestData= "{\"title\":\"foo\",\"body\":\"bar\",\"userId\":1}";
    // Send the request 
    int httpCode = http.POST(httpRequestData);

    // Check the returning code
    if(httpCode>0){
      String payload =http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    }else
    {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
  
}void getData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    Serial.println("Getting Data");
    http.begin(client,"http://jsonplaceholder.typicode.com/posts/1");

    // Send the request
    int httpCode = http.GET();

    // Check the returning code
    if (httpCode > 0) {
      // Get the request response payload
      String payload = http.getString();
      Serial.println(httpCode);
      Serial.println(payload);
    } else {
      Serial.println("Error on HTTP request");
    }

 
    http.end();
  }
}