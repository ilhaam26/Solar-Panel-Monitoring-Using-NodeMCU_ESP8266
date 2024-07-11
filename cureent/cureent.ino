
#define VIN A0 // define the Arduino pin A0 as voltage input (V in)
const float VCC   = 5.0; // supply voltage 5V or 3.3V. If using PCB, set to 5V only.
const int model = 0;   // enter the model (see below)
float in_cur = 0;
float cutOffLimit = 0.50;
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "HRES";  //Nama Wifi
const char *password = "PrambananA18"; // pass wifi 

String serverName = "http://plts.pramhres.my.id/kirimcbatt.php";
//String serverName = "http://endpoint.pramhres.my.id/wt/inputCurrent.php";


unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
//unsigned long timerDelay = 30000;
 

float adc_voltage = 0.0;
float in_voltage = 0.0;

int adc_value = 0;

float sensitivity[] ={
          40.0,// for ACS758LCB-050B
          60.0,// for ACS758LCB-050U
          20.0,// for ACS758LCB-100B
          40.0,// for ACS758LCB-100U
          13.3,// for ACS758KCB-150B
          16.7,// for ACS758KCB-150U
          10.0,// for ACS758ECB-200B
          20.0,// for ACS758ECB-200U     
         }; 


float quiescent_Output_voltage [] ={
          0.5,// for ACS758LCB-050B
          0.12,// for ACS758LCB-050U
          0.5,// for ACS758LCB-100B
          0.12,// for ACS758LCB-100U
          0.5,// for ACS758KCB-150B
          0.12,// for ACS758KCB-150U
          0.5,// for ACS758ECB-200B
          0.12,// for ACS758ECB-200U            
          };
const float FACTOR = sensitivity[model]/1000;// set sensitivity for selected model
const float QOV =   quiescent_Output_voltage [model] * VCC;// set quiescent Output voltage for selected model
float voltage;// internal variable for voltage
float cutOff = FACTOR/cutOffLimit;// convert current cut off to mV

void setup() {
    //Robojax.com ACS758 Current Sensor 
    Serial.begin(9600);// initialize serial monitor

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
  //Robojax.com ACS758 Current Sensor 
  float voltage_raw =   (5.0 / 1024.0)* (analogRead(VIN) - 270);// Read the voltage from sensor
  voltage =  voltage_raw - QOV + 0.007 ;// 0.007 is a value to make voltage zero when there is no current
  float current = voltage / FACTOR;
  adc_value = analogRead(VIN);
  Serial.print("A: ");
  Serial.println(adc_value);

    Serial.print("Current: ");
    Serial.print(current,2); // print the current with 2 decimal places
    Serial.println("A");

  delay(1000);
String cur, iddev;

  cur = current,2;
  iddev = "10";

if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "?current="+ cur +"&iddev="+ iddev ;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());

      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        Serial.println(serverPath);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
  
}
