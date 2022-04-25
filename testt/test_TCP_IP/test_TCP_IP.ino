String ptr,webpage="";                                   //String variable to store characters
int i=0,k=0,x=0;                                         //integer variables
String readString;                                   //using readString feature to read characters                       

boolean No_IP=false;                                 //boolean variables 
String IP="";                                         //String variable to store data
char temp1='0';                                      //character variable

void connect_wifi(String cmd, int t)                  //This function is for connecting ESP8266 with wifi network by using AT commands
{
  int temp=0,i=0;
  while(1)
  {
    Serial.println(cmd);                  //Sends to serial monitor
    Serial2.println(cmd);                 //sends to ESP8266 via serial communication
    while(Serial2.available())
    {
      if(Serial2.find("OK"))
      i=8;
    }
    delay(t);
    if(i>5)
    break;
    i++;
  }
  if(i==8)
  Serial.println("OK");
  else
  Serial.println("Error");
}
void wifi_init()                                //This function contains AT commands that passes to connect_wifi()
{
      connect_wifi("AT",100);                   //Sends AT command with time(Command for Achknowledgement)
      connect_wifi("AT+CWMODE=3",100);          //Sends AT command with time (For setting mode of Wifi)
      connect_wifi("AT+CWQAP",100);            //Sends AT command with time (for Quit AP)
      connect_wifi("AT+RST",5000);             //Sends AT command with time (For RESETTING WIFI)
      check4IP(5000);
      if(!No_IP)
      {
        
        Serial.println("Connecting Wifi....");
        connect_wifi("AT+CWJAP=\"TPYP_2.4G\",\"0918898250\"",7000);         //provide your WiFi username and password here
   
      }
      else
        {
        }
      Serial.println("Wifi Connected"); 
      get_ip();
      connect_wifi("AT+CIFSR=1",100);    
      connect_wifi("AT+CIPSTART="TCP","127.0.0.1",8080");
      connect_wifi("AT+CIPMUX=1",100);
}
void setup() 
{
  
   Serial.begin(115200);                //begins serial monitor with baud rate 9600
   Serial2.begin(115200);               //begins serial communication with esp8266 with baud rate 9600 (Change according to your esp8266 module)
   wifi_init();
   Serial.println("System Ready..");
}
void loop() 
{
  k=0;
  
  Serial.println("Please Refresh your Page");
  while(k<1000)
  {
    k++;
   while(Serial2.available())
   {
    if(Serial2.find("0,CONNECT"))
    {
      Serial.println("Start Printing");
      Send();
      Serial.println("Done Printing");

    }
  }
  delay(1);
 }
unsigned int l=webPage.length();
}
void Send_int(String link_id,vari_int)
{
connect_wifi("AT+CIPSEND="+link_id+",1");
Serial.println(vari_int);
Serial2.println(vari_int);
while(Serial2.available())
      {
       
        if(Serial2.find("OK"))
        {
          ii=11;
          break;
        }
      }
      if(ii==11)
      break;
      delay(100);
Serial2.println("AT+CIPCLOSE=0");  
     }

void Send_float(String link_id,vari_float)
{

connect_wifi("AT+CIPSEND="+link_id+",4");
Serial.println(vari_float);
Serial2.println(vari_float);
while(Serial2.available())
      {
       
        if(Serial2.find("OK"))
        {
          ii=11;
          break;
        }
      }
      if(ii==11)
      break;
      delay(100);
Serial2.println("AT+CIPCLOSE=0");  
}
void Send_string(String link_id,vari_string)
{

connect_wifi("AT+CIPSEND="+link_id+",4");
Serial.println(vari_string);
Serial2.println(vari_string);
while(Serial2.available())
      {
       
        if(Serial2.find("OK"))
        {
          ii=11;
          break;
        }
      }
      if(ii==11)
      break;
      delay(100);
Serial2.println("AT+CIPCLOSE=0");  
}