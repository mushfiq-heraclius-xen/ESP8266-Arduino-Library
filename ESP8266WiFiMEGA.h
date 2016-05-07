#include <String.h>

#define baudrate 9600
#define INDEX_SIZE 512

class ESP8266WiFiMEGA
{
	public:
	char atRxBuffer[INDEX_SIZE];
	
	uint8_t confirmAtCommand(char *searchString, unsigned long timeOut);
	String getWiFiReply(unsigned long timeOut);
	boolean init(void);
	void setCWMODE(int mode);
	int getCWMODE(void);
	void setCIPMUX(int n);
	int getCIPMUX(void);
	void setCIPMODE(int n);
	int getCIPMODE(void);
	int getCIPSTATUS(void);
	boolean startServer(String port,String timeOut);
	boolean deleteServer(void);
	boolean startSingleClient(String type,String addr,String port);
	boolean startMultipleClient(String id,String type,String addr,String port);
	boolean deleteAllClients(void);
	boolean send(String id,String s);
	boolean send(String s);
	void restart(void);
	boolean setSoftAPParameters(String ssid,String pwd,String ch,String ecn);
	String getSoftAPParameters(void);
	boolean setIPOfStation(String ip);
	String getIPOfStation(void);
	boolean setIPOfSoftAP(String ip);
	String getIPOfSoftAP(void);
	boolean joinAP(String ssid, String pwd);
	String getAPParameters(void);
	boolean disconnectFromAP(void);
	String getListOfAvailableAP(void);
	String getListOfClientsConnectedToSoftAP(void);
	
	String getLocalIP(void);
};

/*************************************************************  
Procedure to search data returned from AT commands.
ARGUMENTS
  searchString  pointer to string to be searched
  timeOut     timeout in milliseconds
  
RETURN:
  0       searchString found successfully
  1       ERROR string encountered, potential problem
  2       Buffer full, searchString not found
  3       Timeout reached before searchString found
**************************************************************/

uint8_t ESP8266WiFiMEGA::confirmAtCommand(char *searchString, unsigned long timeOut)
{
  uint8_t index = 0;
  unsigned long tOut = millis();
  while((millis() - tOut) <= timeOut)
  {
    if (Serial2.available())
    {
      atRxBuffer[index] = Serial2.read();
      index++;
      atRxBuffer[index] = '\0';
      if(strstr(atRxBuffer,searchString) != NULL)
        return 0;
      if(strstr(atRxBuffer,"ERROR") != NULL)
        return 1;
      if(index >= (INDEX_SIZE - 1))
        return 2; 
    }
  }
  return 3;
}
String ESP8266WiFiMEGA::getWiFiReply(unsigned long timeOut)
{
	String str = "";
	char c; 
	unsigned long tOut = millis();
    while((millis() - tOut) <= timeOut)
	{
		if (Serial2.available())
		{
			c = Serial2.read();
			str+=c;
		}
	}
	return str;
}
boolean ESP8266WiFiMEGA::init(void)
{
	Serial2.begin(baudrate);
	Serial2.println("AT");
	int x = confirmAtCommand("OK",3000);
	if(x==0)
		return true;
	else
		return false;
}
void ESP8266WiFiMEGA::setCWMODE(int mode)
{
	if(mode==1)
	{
		Serial2.println("AT+CWMODE=1");
		confirmAtCommand("OK",3000);
	}		
	if(mode==2)
	{
		Serial2.println("AT+CWMODE=2");
		confirmAtCommand("OK",3000);
	}	
	if(mode==3)
	{
		Serial2.println("AT+CWMODE=3");
		confirmAtCommand("OK",3000);
	}			
}
int ESP8266WiFiMEGA::getCWMODE(void)
{
	String reply ="";
	String t = "";
	Serial2.println("AT+CWMODE?");
	reply = getWiFiReply(3000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
}
void ESP8266WiFiMEGA::setCIPMUX(int n)
{
	if(n==0)
	{
		Serial2.println("AT+CIPMUX=0");
		confirmAtCommand("OK",3000);		
	}
	if(n==1)
	{
		Serial2.println("AT+CIPMUX=1");
		confirmAtCommand("OK",3000);
	}
		
}
int ESP8266WiFiMEGA::getCIPMUX(void)
{
	String reply = "";
	String t = "" ;
	Serial2.println("AT+CIPMUX?");
	reply = getWiFiReply(3000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
	
}
void ESP8266WiFiMEGA::setCIPMODE(int n)
{
	if(n==0)
	{	
		Serial2.println("AT+CIPMODE=0");
		confirmAtCommand("OK",3000);	
		
	}
	if(n==1)
	{
		Serial2.println("AT+CIPMODE=1");
		confirmAtCommand("OK",3000);	
	}
}
int ESP8266WiFiMEGA::getCIPMODE(void)
{
	String reply = "";
	String t = "" ;
	Serial2.println("AT+CIPMODE?");
	reply =getWiFiReply(3000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
	
}
int ESP8266WiFiMEGA::getCIPSTATUS(void)
{
	String reply = "";
	String t = "" ;
	Serial2.println("AT+CIPSTATUS");
	reply =getWiFiReply(3000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
}
boolean ESP8266WiFiMEGA::startServer(String port,String timeOut)
{
	setCIPMUX(1);
	confirmAtCommand("OK",3000);
	Serial2.println("AT+CIPSERVER=1,"+port);
	int x = confirmAtCommand("OK",3000);
	Serial2.println("AT+CIPSTO="+timeOut);
	confirmAtCommand("OK",3000);
	if(x==0)
		return true;
	else
		return false;

}
boolean ESP8266WiFiMEGA::deleteServer(void)
{
	Serial2.println("AT+CIPSERVER=0");
	int x = confirmAtCommand("OK",4000);
	restart();
	if(x==0)
		return true;
	else
		return false;
}

boolean ESP8266WiFiMEGA::startSingleClient(String type,String addr,String port)
{
	setCIPMUX(0);
	confirmAtCommand("OK",2000);
	String s ="";
	s = s + "AT+CIPSTART=" + "\"" + type + "\"" + "," + "\"" + addr + "\"" + "," + port ;
	Serial2.println(s);
	int x = confirmAtCommand("OK",10000);
	if(x==0)
		return true;
	else
		return false;
	
}
boolean ESP8266WiFiMEGA::startMultipleClient(String id,String type,String addr,String port)
{
	setCIPMUX(1);
	confirmAtCommand("OK",2000);
	String s ="";
	s = s + "AT+CIPSTART=" + id + "," + "\"" + type + "\"" + "," + "\"" + addr+ "\"" + "," + port ;
	Serial2.println(s);
	int x = confirmAtCommand("OK",10000);
	if(x==0)
		return true;
	else
		return false;
}
boolean ESP8266WiFiMEGA::deleteAllClients(void)
{
	int n =getCIPMUX();
	if(n==0)
		Serial2.println("AT+CIPCLOSE");
	else
		Serial2.println("AT+CIPCLOSE=5");
	int x = confirmAtCommand("OK",3000);
	if(x==0)
		return true;
	else
		return false;
}
boolean ESP8266WiFiMEGA::send(String id,String s)
{
	int len = s.length();
	String lenstr = String(len);
	String temp = "";
	temp = temp + "AT+CIPSEND=" + id + "," + lenstr;
	Serial2.println(temp);
	delay(100);
	Serial2.println(s);
	int x = confirmAtCommand("SEND OK",2500);
	if(x==0)
		return true;
	else
		return false;
}
boolean ESP8266WiFiMEGA::send(String s)
{
	int len = s.length();
	String lenstr = String(len);
	String temp = "";
	temp = temp + "AT+CIPSEND=" + lenstr;
	Serial2.println(temp);
	delay(100);
	Serial2.println(s);
	int x = confirmAtCommand("SEND OK",2500);
	if(x==0)
		return true;
	else
		return false;
}

void ESP8266WiFiMEGA::restart(void)
{
	Serial2.println("AT+RST");
	delay(10000);	
}

boolean ESP8266WiFiMEGA::setSoftAPParameters(String ssid,String pwd,String ch,String ecn)
{
	String s = "";
	s = "\"" + ssid + "\"" + "," + "\"" + pwd + "\"" + "," + ch + "," + ecn ;
	Serial2.print("AT+CWSAP=");
	Serial2.println(s);
	int x = confirmAtCommand("OK",8000);
	if(x==0)
		return true;
	else
		return false;
}
String ESP8266WiFiMEGA::getSoftAPParameters(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CWSAP?");
	reply = getWiFiReply(5000);
	int x = reply.indexOf(":");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	
	return t;
}
boolean ESP8266WiFiMEGA::setIPOfStation(String ip)
{
	String s = "AT+CIPSTA=";
	s = s + "\"" + ip + "\"";
	Serial2.println(s);
	int x = confirmAtCommand("OK",3000);
	if(x==0)
		return true;
	else
		return false;
}
String ESP8266WiFiMEGA::getIPOfStation(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CIPSTA?");
	reply = getWiFiReply(3000);
	int x = reply.indexOf("?");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	
	return t;
}
boolean ESP8266WiFiMEGA::setIPOfSoftAP(String ip)
{
	String s = "AT+CIPAP=";
	s+="\""+ip+"\"";
	Serial2.println(s);
	int x = confirmAtCommand("OK",3000);
	if(x==0)
		return true;
	else
		return false;
}
String ESP8266WiFiMEGA::getIPOfSoftAP(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CIPAP?");
	reply = getWiFiReply(3000);
	int x = reply.indexOf("?");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	return t;
}
boolean ESP8266WiFiMEGA::joinAP(String ssid, String pwd)
{
	String s = "AT+CWJAP=" ;
	s+="\""+ssid+"\""+","+"\""+pwd+"\"";
	Serial2.println(s);
	int x = confirmAtCommand("OK",15000);
	if(x==0)
		return true;
	else
		return false;
	
}
String ESP8266WiFiMEGA::getAPParameters(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CWJAP?");
	reply = getWiFiReply(5000);
	int x = reply.indexOf(":");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	return t;
}
boolean ESP8266WiFiMEGA::disconnectFromAP(void)
{
	Serial2.println("AT+CWQAP");
	int x = confirmAtCommand("WIFI DISCONNECT",7000);
	if(x==0)
		return true;
	else
		return false;
}
String ESP8266WiFiMEGA::getListOfAvailableAP(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CWLAP");
	reply = getWiFiReply(8000);
	int x = reply.indexOf("P");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	return t;
	
}
String ESP8266WiFiMEGA::getListOfClientsConnectedToSoftAP(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CWLIF");
	reply = getWiFiReply(8000);
	int x = reply.indexOf("F");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	return t;
	
}
String ESP8266WiFiMEGA::getLocalIP(void)
{
	String reply = "";
	String t = "";
	Serial2.println("AT+CIFSR");
	reply= getWiFiReply(3000);
	int x = reply.indexOf("R");
	int y = reply.indexOf("OK");
	int i = x+1;
	while(i<y)
	{
		t+=reply[i];
		i++;
	}
	return t;
}