//Interfacing ESP8266 Wi-Fi with STM32F103C8
//CIRCUIT DIGEST
//NOTE: Serial is serial monitor with baud rate(9600)
//NOTE: Serial2 (TX2, RX2)is connected with ESP8266(RX,TX)respectively with baud rate (9600)
#include "time_date.h"
String ptr,webpage="";                                   //String variable to store characters
int i=0,k=0,x=0;                                         //integer variables
String readString;                                   //using readString feature to read characters                       

boolean No_IP=false;                                 //boolean variables 
String IP="";                                         //String variable to store data
char temp1='0';                                      //character variable

String name="<p>Circui
t Digest</p><p>A community of electrical and electronics students, engineers and makers</p>";   //String with html notations
String data="<p>Data Received Successfully.....</p>";     //String with html 
//LDR module
#define LDRpin PA4
int LDRValue = 0;
String Status,T_s,PIR_s = "Dont know" ;
//TEMP module
#define ADC_VREF_mV    5000.0 // in millivolt
#define ADC_RESOLUTION 1024.0
#define PIN_LM35 PA5
float TempC,tempF ;
float Amp,humi = 0.2;

     
void check4IP(int t1)                                     //A function to check ip of ESP8266 
{
  int t2=millis();
  while(t2+t1>millis())
  {
    while(Serial2.available()>0)
    {
      if(Serial2.find("WIFI GOT IP"))
      {
        No_IP=true;
      }
    }
  }
}

void get_ip()                                           //After cheacking ip ,this is a function to get IP address
{
  IP="";
  char ch=0;
  while(1)
  {
    Serial2.println("AT+CIFSR");                   //GET IP AT COMMAND
    while(Serial2.available()>0)        
    {
      if(Serial2.find("STAIP,"))                   //This finds the STAIP that is the STATIC IP ADDRESS of ESP8266
      {
        delay(1000);
        Serial.print("IP Address:");
        while(Serial2.available()>0)
        {
          ch=Serial2.read();                      //Serial2 reads from ESP8266
          if(ch=='+')
          break;
          IP+=ch;
        }
      }
      if(ch=='+')
      break;
    }
    if(ch=='+')
    break;
    delay(1000);
  }
  Serial.print(IP);                                //prints IP address in Serial monitor
  Serial.print("Port:");
  Serial.println(80);
}

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

void temp_mo()
{
  int adcVal = analogRead(PIN_LM35);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in Celsius
  TempC = milliVolt / 100;
  // convert the Celsius to Fahrenheit
//  tempF = tempC * 9 / 5 + 32;
  Serial.print("Temperature: ");
  Serial.print(TempC);   // print the temperature in Celsius
  Serial.print("°C");
//  Serial.print("  ~  "); // separator between Celsius and Fahrenheit
//  Serial.print(tempF);   // print the temperature in Fahrenheit
//  Serial.println("°F");


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
      
      connect_wifi("AT+CIPMUX=1",100);                          //Sends AT command with time (For creating multiple connections)
      connect_wifi("AT+CIPSERVER=1,80",100);                    //Sends AT command with time (For setting up server with port 80)
}

void sendwebdata(String webPage)                          //This function is used to send webpage datas to the localserver
{
    int ii=0;
     while(1)
     {
      unsigned int l=webPage.length();
      Serial.print("AT+CIPSEND=0,");
      Serial2.print("AT+CIPSEND=0,");
      Serial.println(l+2);
      Serial2.println(l+2);
      delay(100);
      Serial.println(webPage);                        //sends webpage data to serial monitor
      Serial2.println(webPage);                       //sends webpage data to serial2 ESP8266
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
     }
}

void setup() 
{
  
   Serial.begin(115200);                //begins serial monitor with baud rate 9600
   Serial2.begin(115200);               //begins serial communication with esp8266 with baud rate 9600 (Change according to your esp8266 module)
   pinMode(PIN_LM35, INPUT);
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
      delay(1000);
    }
  }
  delay(1);
 }
}

void Send()                                        //This function contains data to be sent to local server
{
      temp_mo();
      ldr_check();

      ptr="<h5> STATUS : </h5>";
      ptr+=String(Status);
      sendwebdata(ptr);
//      delay(10);
//      ptr="<h5> Current : </h5>";
//      ptr+=String(Amp);
//      sendwebdata(ptr);
//      delay(10);
//      ptr=="<h5> Temp : </h5>";
//      ptr+=String(TempC);
//      sendwebdata(ptr);
//      delay(10);
//      ptr="<h5> humi : </h5>";
//      ptr+=String(humi);
//      sendwebdata(ptr);
//      delay(10);
//      ptr+="<h5> touch status : </h5>";
//      ptr+=String(T_s);
//      delay(10);
//
//      sendwebdata(ptr);
//
//      ptr="<h5> PIR status : </h5>";
//      ptr+=String(PIR_s);
//      delay(10);
//      sendwebdata(ptr);

//      webpage = "<a href=\"http://circuitdigest.com/\"";
//      webpage+="\">Click Here to get into circuitdigest.com</a>";

      Serial2.println("AT+CIPCLOSE=0");                  //Closes the server connection
}

void ldr_check()
{
   LDRValue = analogRead(LDRpin);
  if(LDRValue<1500){
    Status = "Day";
    }    
    else {
    Status ="night";
    }
  Serial.println(Status);
  }
String HTML(String Status , float Amp,float TempC,float humi,String T_s,String PIR_s) 
  {
    //      webpage = "<a href=\"http://circuitdigest.com/\"";
//      webpage+="\">Click Here to get into circuitdigest.com</a>\n";
//  String ptr = Time_date;
  ptr="<'div class=col-sm-4  col-up-time text-black text-center\n'>";
  ptr+="<h5 class='topic-font'> STATUS : </h5>\n";
  ptr+=String(Status);
  ptr+="</div>\n";
  ptr+="</div>\n";
  ptr+=Show_data;
  ptr+="<h5 class='topic-font'> Current : </h5>\n";
  ptr+=String(Amp);
  ptr+="</div>\n";
  ptr+="<div class='col'>";
  ptr+="<h5 class='topic-font'> Temp : </h5>\n";
  ptr+=String(TempC);
  ptr+="</div>\n";
  ptr+="</div>\n";
  ptr+="<div class='row'>";
  ptr+="<div class='col'>";
  ptr+="<h5 class='topic-font'> humi : </h5>\n";
  ptr+=String(humi);
  ptr+="</div>\n";
  ptr+="<div class='col'>\n";
  ptr+="<h5 class='topic-font'> touch status : </h5>\n";
  ptr+=String(T_s);
  ptr+="</div>\n";
  ptr+="<div class='col'>\n";
  ptr+="<h5 class='topic-font'> PIR status : </h5>\n";
  ptr+=String(PIR_s);
  ptr+="</div>\n";
  ptr+="</div>\n";
  ptr+="</body>\n";
  ptr+="</html>\n";

  return ptr;
}
