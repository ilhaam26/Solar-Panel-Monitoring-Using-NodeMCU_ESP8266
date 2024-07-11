#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const int relay1 = 16;
const int relay2 = 5;
const int relay3 = 4;

String response;
const char *ssid = "HRES";  //Nama Wifi
const char *password = "PrambananA18"; // pass wifi 

String serverName = "http://plts.pramhres.my.id/switching.php";

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);// initialize serial monitor

pinMode(relay1, OUTPUT);
pinMode(relay2, OUTPUT);
pinMode(relay3, OUTPUT);

digitalWrite(relay1, HIGH);
digitalWrite(relay2, HIGH);
digitalWrite(relay3, HIGH);
     WiFi.mode(WIFI_OFF);        
  delay(1000);
  WiFi.mode(WIFI_STA); 
  
  WiFi.begin(ssid, password); 
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
}

void loop() {
  // put your main code here, to run repeatedly:
if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverName.c_str());
  
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        response = http.getString();
        Serial.println(response);
        Serial.println(serverName);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
     
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    delay(1000);
    if(response >= "1" && response < "2"){
      digitalWrite(relay1, LOW);
      digitalWrite(relay2, HIGH);   
      digitalWrite(relay3, HIGH); 
      Serial.println("relay 1 aktif");
    } 
    else if(response >= "2" && response < "3"){
      digitalWrite(relay2, LOW);
      digitalWrite(relay1, HIGH);   
      digitalWrite(relay3, HIGH); 
      Serial.println("relay 2 aktif");
    } else if(response >= "3"){
      digitalWrite(relay3, LOW);
      digitalWrite(relay2, HIGH);   
      digitalWrite(relay1, HIGH); 
      Serial.println("relay 3 aktif");
    }
              
  }
