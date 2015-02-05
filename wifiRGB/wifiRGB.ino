

#define SSID       "netcore"
#define PASSWORD   "987656789"


#include "uartWIFI.h"
#include <SoftwareSerial.h>
WIFI wifi;

extern int chlID;	//client id(0-4)


void setRBG(char *buf)
{
    char r[3],g[3],b[3];
    r[0]=buf[3];r[1]=buf[4];
    g[0]=buf[5];g[1]=buf[6];
    b[0]=buf[7];b[1]=buf[8];
    
    int cr = (int) strtol( &r[0], NULL, 16);
    int cg = (int) strtol( &g[0], NULL, 16);
    int cb = (int) strtol( &b[0], NULL, 16);
    
    analogWrite(11,cr);
    analogWrite(9,cg);
    analogWrite(10,cb);    // 打印对应的数据到串口
    
    DebugSerial.println(r);
    DebugSerial.println(g);
    DebugSerial.println(b);

    DebugSerial.println(cr);
    DebugSerial.println(cg);
    DebugSerial.println(cb);


}


void setup()
{

  
  wifi.begin();
  bool b = wifi.Initialize(STA, SSID, PASSWORD);
  if(!b)
  {
    DebugSerial.println("Init error");
  }
  delay(8000);  //make sure the module can have enough time to get an IP address 
  String ipstring  = wifi.showIP();
  DebugSerial.println(ipstring);		//show the ip address of module
  
  delay(5000);
  wifi.confMux(1);
  delay(100);
  if(wifi.confServer(1,8088))
	DebugSerial.println("Server is set up");
 
  //FOR RGB
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  
  //FOR Relay
  pinMode(12,OUTPUT);

	

}
void loop()
{
 
  String recive;
  char buf[100];
   
  //RGB 

  
  int iLen = wifi.ReceiveMessage(buf);
  int lineCodeIndex=-1;
  if(iLen > 0)
  {
    
    lineCodeIndex=String(buf).indexOf(char(13));
    recive=String(buf).substring(0,lineCodeIndex);
    wifi.Send(chlID,recive);
    
    //setRBG(buf);

    if (strcmp(buf,"OPEN")==0)
    {
      digitalWrite(12,LOW);
    }
    
    if (strcmp(buf,"CLOSE")==0)
    {
      digitalWrite(12,HIGH);
    }


    if (strcmp(buf, "#") == 0)
    {      
      wifi.Send(chlID,"This is RGB");
    }
    
    
  }
}


