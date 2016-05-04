#include <SoftwareSerial.h>
#include "ESP8266WiFiUNO.h"

extern SoftwareSerial esp8266wifi;
int relayPin[] = {4, 5, 6, 7};
ESP8266WiFiUNO myWifi;


void setup()
{
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  Serial.println("Setup Begins!");
  digitalWrite(13, HIGH);
  for (int i = 0; i < 6; i++)
    pinMode(relayPin[i], OUTPUT);
  for (int i = 0; i < 6; i++)
    digitalWrite(relayPin[i], LOW);
  Serial.println("Init!");
  myWifi.init();
  myWifi.setCWMODE(2);
  myWifi.setIPOfSoftAP("192.168.50.1");
  myWifi.setSoftAPParameters("SMART_HOME", "sadat2017", "5", "3");
  Serial.println("Starting Server!");
  myWifi.deleteServer();
  myWifi.startServer("1", "7200");
  Serial.println("Setup Ended!");
  digitalWrite(13, LOW);



}
long start = millis();
void loop()
{
  String response  = "";
  String localScanCode = "";
  while (esp8266wifi.available() > 0)
  {
    //most crucial delay of this part :D
    delay(10);
    char c = esp8266wifi.read();
    response += c;
  }
  int x = response.indexOf(":");
  localScanCode = localScanCode + response[x + 1] + response[x + 2];
  if (localScanCode == "1H")
  {
    digitalWrite(relayPin[0], HIGH);
    myWifi.send(0, "Load1 ON");
    myWifi.send(1, "Load1 ON");
  }
  if (localScanCode == "1L")
  {
    digitalWrite(relayPin[0], LOW);
    myWifi.send(0, "Load1 OFF");
    myWifi.send(1, "Load1 OFF");
  }
  if (localScanCode == "2H")
  {
    digitalWrite(relayPin[1], HIGH);
    myWifi.send(0, "Load2 ON");
    myWifi.send(1, "Load2 ON");
  }
  if (localScanCode == "2L")
  {
    digitalWrite(relayPin[1], LOW);
    myWifi.send(0, "Load2 OFF");
    myWifi.send(1, "Load2 OFF");
  }
  if (localScanCode == "3H")
  {
    digitalWrite(relayPin[2], HIGH);
    myWifi.send(0, "Load3 ON");
    myWifi.send(1, "Load3 ON");
  }
  if (localScanCode == "3L")
  {
    digitalWrite(relayPin[2], LOW);
    myWifi.send(0, "Load3 OFF");
    myWifi.send(1, "Load3 OFF");
  }
  if (localScanCode == "4H")
  {
    digitalWrite(relayPin[3], HIGH);
    myWifi.send(0, "Load4 ON");
    myWifi.send(1, "Load4 ON");
  }
  if (localScanCode == "4L")
  {
    digitalWrite(relayPin[3], LOW);
    myWifi.send(0, "Load4 OFF");
    myWifi.send(1, "Load4 OFF");
  }
  //Turning All ON
  if (localScanCode == "AH")
  {
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(relayPin[i], HIGH);

    }
    myWifi.send(0, "ALL ON");
    myWifi.send(1, "ALL ON");
  }
  //Turning All OFF
  if (localScanCode == "AL")
  {
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(relayPin[i], LOW);
    }
    myWifi.send(0, "ALL OFF");
    myWifi.send(1, "ALL OFF");
  }
  //checking whether the interval(e.g. 8mins) has passed? in order to restart server
  if ((millis() - start) >= 600000)
  {
    start = millis();
    Serial.println("Restarting The Server!");
    digitalWrite(13, HIGH);
    myWifi.init();
    myWifi.deleteServer();
    myWifi.startServer("1", "7200");

    Serial.println("Done!");
    digitalWrite(13, LOW);
  }
}





