#include <WiFi.h> 
#include <HTTPClient.h>  
#include <SPI.h>
#include "RF24.h"
//#include "LowPower.h"
//#include <ArduinoLowPower.h>

const char* ssid = "xxxxx"; 
const char* password = "xxxxxxxxx";

//Your Domain name with URL path or IP address with path 
String serverName = "https://abc.com/xyz/api/table.php";


float rr[3];
void  postToServer(float rr[3]);

RF24 radio(4, 5);
const byte address[6] = "00001";

void setup() 
{
  Serial.begin(115200);
  
  WiFi.begin(ssid, password); 
  Serial.println("Connecting"); 
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500); 
    Serial.print("."); 
  } 
  Serial.println(""); 
  Serial.print("Connected to WiFi network with IP Address: "); 
  Serial.println(WiFi.localIP());

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.startListening();
  
}
 
void loop()
{ 
 while(radio.available()){
   radio.read(rr, sizeof(rr));
   Serial.println("Sensor1:");
   Serial.println(rr[0]);
   Serial.println("Sensor2:");
   Serial.println(rr[1]);
   Serial.println("Sensor3:");
   Serial.println(rr[2]);
    
    postToServer(rr); // function to post the values 
    delay(300000); //post the values every three minutes.
  }

//  radio.powerDown();
//  MCUCR |= (3 << 5); //set both BODS and BODSE at the same time
//  MCUCR = (MCUCR & ~(1 << 5)) | (1 << 6); //then set the BODS bit and clear the BODSE bit at the same time
//  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF); // sleep for 4 seconds with all IOs OFF
//LowPower.deepSleep(60000);
}

void  postToServer(float rr[3])
{ 
   
    //Check WiFi connection status 
    if(WiFi.status()== WL_CONNECTED){ 
      HTTPClient http; 
         
      // Your Domain name with URL path or IP address with path 
      http.begin(serverName); 
      
      http.addHeader("Content-Type", "application/json");
      
      // Send HTTP POST request 
      // can use for loop to post the three values 
      int httpResponseCode1 = http.POST("{\"sensor_id\":\"S1\",\"value\":\""+String(rr[0])+"\"}"); 
      if (httpResponseCode1>0) { 
        Serial.print("HTTP Response code: "); 
        Serial.println(httpResponseCode1); 
        String payload = http.getString(); 
        Serial.println(payload); 
      } 
      else { 
        Serial.print("Error code: "); 
        Serial.println(httpResponseCode1); 
      }
      delay(2000);
      int httpResponseCode2 = http.POST("{\"sensor_id\":\"S2\",\"value\":\""+String(rr[1])+"\"}");
      if (httpResponseCode2>0) { 
        Serial.print("HTTP Response code: "); 
        Serial.println(httpResponseCode2); 
        String payload = http.getString(); 
        Serial.println(payload); 
      } 
      else { 
        Serial.print("Error code: "); 
        Serial.println(httpResponseCode2); 
      }
      delay(2000);
      int httpResponseCode3 = http.POST("{\"sensor_id\":\"S3\",\"value\":\""+String(rr[2])+"\"}");
      if (httpResponseCode1>0) { 
        Serial.print("HTTP Response code: "); 
        Serial.println(httpResponseCode3); 
        String payload = http.getString(); 
        Serial.println(payload); 
      } 
      else { 
        Serial.print("Error code: "); 
        Serial.println(httpResponseCode3); 
      }  
      // Free resources 
      http.end(); 
    } 
    else { 
      Serial.println("WiFi Disconnected"); 
    } 
}
