#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#define REED_SWITCH 5 //D1

int status = WL_IDLE_STATUS; //not required.

const char* ssid = "yourssid";
const char* password = "yourpassword";
int doorClosed = 1;

void setup() {
  pinMode(REED_SWITCH, INPUT_PULLUP);

  Serial.begin(115200);

  setupWifi();
   
   //get_http();

}

void setupWifi()
{
     // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
   WiFi.mode(WIFI_STA);
   status = WiFi.begin(ssid, password);    

   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);  

   // Wait for connection
   while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
   }
   Serial.println("Connected to wifi");
}

void loop() {
   if (WiFi.status() != WL_CONNECTED)
   {
     setupWifi();
   }
  // put your main code here, to run repeatedly:
    if ((digitalRead(REED_SWITCH) == HIGH) && (doorClosed == 1))
    {
      Serial.println("DOOR OPEN!!");
      while (get_http(String("DOOR_OPEN_")) != 0);      
      doorClosed = 0;
    } 
    else if ((digitalRead(REED_SWITCH) == LOW) && (doorClosed == 0))
    {
      Serial.println("DOOR CLOSED!!");
      while (get_http(String("DOOR_CLOSED_")) != 0);      
      doorClosed = 1;
    }
    delay(10);


}


int get_http(String state)
{
   HTTPClient http;
   int ret = 0;
   Serial.print("[HTTP] begin...\n");
   // configure ifttt server and url  should be HTTP only..not https!!!  (http://)   
    http.begin("http://maker.ifttt.com/trigger/KAPI/with/key/0adOkL-O9M9SDm5zwynJH"); //HTTP
   //If you have enabled value1 from iftt settings then uncomment below line to send value and comment out above line  
   //http.begin("http://maker.ifttt.com/trigger/door/with/key/your_key_from_Iftt/?value1="+state); //HTTP

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    // httpCode will be negative on error
    if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

      if(httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
        ret = -1;
        Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
        delay(500); // wait for half sec before retry again
    }

    http.end();
    return ret;
}  
