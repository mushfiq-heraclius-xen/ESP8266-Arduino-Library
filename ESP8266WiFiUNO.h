#include <SoftwareSerial.h>
#include <String.h>
SoftwareSerial esp8266wifi(2,3);
#define baudrate 9600
#define INDEX_SIZE 512

class ESP8266WiFiUNO
{
	public:
	char atRxBuffer[INDEX_SIZE];
	
	boolean init(void);
	uint8_t confirmAtCommand(char *searchString, unsigned long timeOut);
	String getWiFiReply(unsigned long timeOut);
	void setCWMODE(int mode);
	int getCWMODE(void);
	void setCIPMUX(int n);
	int getCIPMUX(void);
	void setCIPMODE(int n);
	int getCIPMODE(void);
	int getCIPSTATUS(void);
	void startServer(int port,int timeOut);
	void deleteServer(void);
	void startSingleClient(String type,String addr,String port);
	void startMultipleClient(int id,String type,String addr,String port);
	void deleteAllClients(void);
	void send(int id,String s);
	void send(String s);
	void restart(void);
	void setSoftAPParameters(String ssid,String pwd,int ch,int ecn);
	String getSoftAPParameters(void);
	void setIPOfStation(String ip);
	String getIPOfStation(void);
	void setIPOfSoftAP(String ip);
	String getIPOfSoftAP(void);
	void joinAP(String ssid, String pwd);
	String getAPParameters(void);
	void disconnectFromAP(void);
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
boolean ESP8266WiFiUNO::init(void)
{
	esp8266wifi.begin(baudrate);
	esp8266wifi.println("AT");
	int x = confirmAtCommand("OK",1200);
	if(x==0)
		return true;
	else
		return false;
}
uint8_t ESP8266WiFiUNO::confirmAtCommand(char *searchString, unsigned long timeOut)
{
  uint8_t index = 0;
  unsigned long tOut = millis();
  while((millis() - tOut) <= timeOut)
  {
    if (esp8266wifi.available())
    {
      atRxBuffer[index] = esp8266wifi.read();
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
String ESP8266WiFiUNO::getWiFiReply(unsigned long timeOut)
{
	String str = "";
	char c; 
	unsigned long tOut = millis();
    while((millis() - tOut) <= timeOut)
	{
		if (esp8266wifi.available())
		{
			c = esp8266wifi.read();
			str+=c;
		}
	}
	return str;
}
void ESP8266WiFiUNO::setCWMODE(int mode)
{
	if(mode==1)
	{
		esp8266wifi.println("AT+CWMODE=1");
		confirmAtCommand("OK",1200);
	}		
	if(mode==2)
	{
		esp8266wifi.println("AT+CWMODE=2");
		confirmAtCommand("OK",1200);
	}	
	if(mode==3)
	{
		esp8266wifi.println("AT+CWMODE=3");
		confirmAtCommand("OK",1200);
	}			
}
int ESP8266WiFiUNO::getCWMODE(void)
{
	String reply ="";
	String t = "";
	esp8266wifi.println("AT+CWMODE?");
	reply = getWiFiReply(2000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
}
void ESP8266WiFiUNO::setCIPMUX(int n)
{
	if(n==0)
	{
		esp8266wifi.println("AT+CIPMUX=0");
		confirmAtCommand("OK",1200);		
	}
	if(n==1)
	{
		esp8266wifi.println("AT+CIPMUX=1");
		confirmAtCommand("OK",1200);
	}
		
}
int ESP8266WiFiUNO::getCIPMUX(void)
{
	String reply = "";
	String t = "" ;
	esp8266wifi.println("AT+CIPMUX?");
	reply = getWiFiReply(2000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
	
}
void ESP8266WiFiUNO::setCIPMODE(int n)
{
	if(n==0)
	{	
		esp8266wifi.println("AT+CIPMODE=0");
		confirmAtCommand("OK",1200);	
		
	}
	if(n==1)
	{
		esp8266wifi.println("AT+CIPMODE=1");
		confirmAtCommand("OK",1200);	
	}
}
int ESP8266WiFiUNO::getCIPMODE(void)
{
	String reply = "";
	String t = "" ;
	esp8266wifi.println("AT+CIPMODE?");
	reply =getWiFiReply(2000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
	
}
int ESP8266WiFiUNO::getCIPSTATUS(void)
{
	String reply = "";
	String t = "" ;
	esp8266wifi.println("AT+CIPSTATUS");
	reply =getWiFiReply(2000);
	int x = reply.indexOf(":");
	t += reply[x+1];
	return atoi(t.c_str());
	
}
void ESP8266WiFiUNO::startServer(int port,int timeOut)
{
	setCIPMUX(1);
	confirmAtCommand("OK",1200);
	esp8266wifi.println("AT+CIPSERVER=1,"+port);
	confirmAtCommand("OK",1200);
	esp8266wifi.println("AT+CIPSTO="+timeOut);
	confirmAtCommand("OK",1200);
}
void ESP8266WiFiUNO::deleteServer(void)
{
	esp8266wifi.println("AT+CIPSERVER=0");
	confirmAtCommand("OK",1200);
	restart();
}

void ESP8266WiFiUNO::startSingleClient(String type,String addr,String port)
{
	setCIPMUX(0);
	confirmAtCommand("OK",2000);
	String s ="";
	s = s + "AT+CIPSTART=" + "\"" + type + "\"" + "," + "\"" + addr + "\"" + "," +"\""+ port+"\"";
	esp8266wifi.println(s);
	confirmAtCommand("OK",2000);
}
void ESP8266WiFiUNO::startMultipleClient(int id,String type,String addr,String port)
{
	setCIPMUX(1);
	confirmAtCommand("OK",2000);
	String s ="";
	s = s + "AT+CIPSTART=" + id + "," + "\"" + type + "\"" + "," + "\"" + addr+ "\"" + "," +"\""+port+"\"";
	esp8266wifi.println(s);
	confirmAtCommand("OK",2000);
}
void ESP8266WiFiUNO::deleteAllClients(void)
{
	int x =getCIPMUX();
	if(x==0)
		esp8266wifi.println("AT+CIPCLOSE");
	else
		esp8266wifi.println("AT+CIPCLOSE=5");
	confirmAtCommand("OK",2000);
}
void ESP8266WiFiUNO::send(int id,String s)
{
	int len = s.length();
	String temp = "";
	temp = temp + "AT+CIPSEND=" + id + "," + len;
	esp8266wifi.println(temp);
	delay(100);
	esp8266wifi.println(s);
	delay(100);
}
void ESP8266WiFiUNO::send(String s)
{
	int len = s.length();
	
	String temp = "";
	temp = temp + "AT+CIPSEND=" + len;
	esp8266wifi.println(temp);
	delay(100);
	esp8266wifi.println(s);
	delay(100);
}


void ESP8266WiFiUNO::restart(void)
{
	esp8266wifi.println("AT+RST");
	delay(7000);		
	
}

void ESP8266WiFiUNO::setSoftAPParameters(String ssid,String pwd,int ch,int ecn)
{
	String s = "";
	s = s + "\"" + ssid + "\"" + "," + "\"" + pwd + "\"" + "," + ch + "," + ecn ;
	esp8266wifi.print("AT+CWSAP=");
	esp8266wifi.println(s);
	confirmAtCommand("OK",3000);
}
String ESP8266WiFiUNO::getSoftAPParameters(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CWSAP?");
	reply = getWiFiReply(3000);
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
void ESP8266WiFiUNO::setIPOfStation(String ip)
{
	String s = "AT+CIPSTA=";
	s = s + "\"" + ip + "\"";
	esp8266wifi.println(s);
	confirmAtCommand("OK",2000);
}
String ESP8266WiFiUNO::getIPOfStation(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CIPSTA?");
	reply = getWiFiReply(2000);
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
void ESP8266WiFiUNO::setIPOfSoftAP(String ip)
{
	String s = "AT+CIPAP=";
	s+="\""+ip+"\"";
	esp8266wifi.println(s);
	confirmAtCommand("OK",2000);
}
String ESP8266WiFiUNO::getIPOfSoftAP(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CIPAP?");
	reply = getWiFiReply(2000);
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
void ESP8266WiFiUNO::joinAP(String ssid, String pwd)
{
	String s = "AT+CWJAP=" ;
	s+="\""+ssid+"\""+","+"\""+pwd+"\"";
	esp8266wifi.println(s);
	confirmAtCommand("OK",2000);
}
String ESP8266WiFiUNO::getAPParameters(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CWJAP?");
	reply = getWiFiReply(2000);
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
void ESP8266WiFiUNO::disconnectFromAP(void)
{
	esp8266wifi.println("AT+CWQAP");
	confirmAtCommand("OK",2000);
}
String ESP8266WiFiUNO::getListOfAvailableAP(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CWLAP");
	reply = getWiFiReply(7000);
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
String ESP8266WiFiUNO::getListOfClientsConnectedToSoftAP(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CWLIF");
	reply = getWiFiReply(4000);
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
String ESP8266WiFiUNO::getLocalIP(void)
{
	String reply = "";
	String t = "";
	esp8266wifi.println("AT+CIFSR");
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