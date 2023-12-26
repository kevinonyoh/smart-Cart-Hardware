#define DEBUG_ESP_HTTP_CLIENT
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "kevin's laptop";
const char* password = "hellokevin";
const char* serverUrl = "https://smartcart-0ayg.onrender.com/health-check";

void setup() {
 Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
     if (WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client; // Create a WiFiClient to pass to HTTPClient
    
    client.setInsecure();  // Disable SSL verification (for testing only)


    HTTPClient http;

    // Specify the server and resource
    http.begin(client, serverUrl);  // Use the updated begin method

    // Make a GET request
    int httpCode = http.GET();

    // Check for a successful request
    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("HTTP GET Request Successful!");
      Serial.println("Response: " + payload);
    } else {
      Serial.println("HTTP GET Request Failed. Error code: " + String(httpCode));
      Serial.print("Error detail: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }

    // Close the connection
    http.end();
  }

  delay(2000);   
   }

