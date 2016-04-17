#include <SoftwareSerial.h>
#include "ESP8266WiFiUNO.h"

extern SoftwareSerial esp8266wifi;
int relayPin[]={4,5,6,7,8,9};
ESP8266WiFiUNO myWifi;


void setup() 
{
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
  Serial.begin(9600);
  Serial.println("Setup Begins!");
  digitalWrite(13,HIGH);
  for(int i=0;i<6;i++)
    pinMode(relayPin[i],OUTPUT);
  for(int i=0;i<6;i++)
    digitalWrite(relayPin[i],HIGH);
  Serial.println("Init!");
  myWifi.init();
  myWifi.setCWMODE(2);
  myWifi.setIPOfSoftAP("192.168.50.1");
  myWifi.setSoftAPParameters("ESP8266_SMART_HOME","01710227718","5","3");
  Serial.println("Starting Server!");
  myWifi.deleteServer();
  myWifi.startServer("1","7200");
  Serial.println("Setup Ended!");
  digitalWrite(13,LOW);
  
  
  
}
long start = millis(); 
void loop() 
{
   String response  = "";
   String localScanCode = "";
   while (esp8266wifi.available()>0) 
   {
     //most crucial delay of this part :D 
     delay(10);
     char c = esp8266wifi.read();
     response +=c;
   }
   int x =response.indexOf(":");
   localScanCode = localScanCode+response[x+1]+response[x+2];
   if(localScanCode=="1H")
      digitalWrite(relayPin[0],LOW);
   if(localScanCode=="1L")
      digitalWrite(relayPin[0],HIGH);
   if(localScanCode=="2H")
      digitalWrite(relayPin[1],LOW);
   if(localScanCode=="2L")
      digitalWrite(relayPin[1],HIGH);
   if(localScanCode=="3H")
      digitalWrite(relayPin[2],LOW);
   if(localScanCode=="3L")
      digitalWrite(relayPin[2],HIGH);
   if(localScanCode=="4H")
      digitalWrite(relayPin[3],LOW);
   if(localScanCode=="4L")
      digitalWrite(relayPin[3],HIGH);
   if(localScanCode=="5H")
      digitalWrite(relayPin[4],LOW);
   if(localScanCode=="5L")
      digitalWrite(relayPin[4],HIGH);
   if(localScanCode=="6H")
      digitalWrite(relayPin[5],LOW);
   if(localScanCode=="6L")
      digitalWrite(relayPin[5],HIGH);
   //Turning All ON
   if(localScanCode=="AH")
      for(int i=0;i<6;i++)
        digitalWrite(relayPin[i],LOW);
   //Turning All OFF
   if(localScanCode=="AL")
      for(int i=0;i<6;i++)
        digitalWrite(relayPin[i],HIGH);
      
        
   //checking whether the interval(e.g. 25mins) has passed? in order to restart server
   if((millis()-start)>=1500000)
   {
      start = millis();
      Serial.println("Restarting The Server!");
      digitalWrite(13,HIGH);
      myWifi.init(); 
	  myWifi.deleteServer();
      myWifi.startServer("1","7200");
	    
      Serial.println("Done!");
      digitalWrite(13,LOW);
   }
}


 
   

