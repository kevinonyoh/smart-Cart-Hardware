#define DEBUG_ESP_HTTP_CLIENT
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>




const char* ssid = "Kevin";
const char* password = "kevinonyoh12";
const char* serverUrl = "https://smartcart-jlz2.onrender.com/product";

int i = 0;
String response ;

void setup() {
 
 
  WiFi.begin(ssid, password);
  Serial.begin(9600);
  Serial.flush();
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);  
    // Serial.println("connecting to wifi");  
  }
  // Serial.println("connected");
 }

void loop() {
     if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client; 
    
    client.setInsecure();  


    HTTPClient http;

    
    http.begin(client, serverUrl);  

    


 if (Serial.available() > 0) {

 
   
   DynamicJsonDocument doc(1024);
    
    String receivedData = Serial.readStringUntil('\n');
      
      i = 1;

      deserializeJson(doc, receivedData);
      

      if(doc.containsKey("data")){
          
       http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(receivedData);
        
          if (httpResponseCode > 0) {
          
           DynamicJsonDocument doc2(1024);
           response = http.getString();
          //  Serial.println(response);
          deserializeJson(doc2, response);
          String message = doc2["message"];
          Serial.println(message);
          i = 1;
        } else {
          Serial.println("network issues");
        }
      
      }
 
   
 } 

  
   http.end();

  }

  delay(2000);   
   }

