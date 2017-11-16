#include <Wire.h>
#include <SPI.h>
#include <ESP8266WiFi.h>  
#include <WiFiClient.h>  
#include <ThingSpeak.h>  
#include "SH1106.h"
#include "SH1106Ui.h"
const char* ssid = "testwifi";  
const char* password = "12345678";  
WiFiClient client;  
unsigned long myChannelNumber =  359593;  
const char * myWriteAPIKey = "X4G7UOIVL2JV3BAD";  


/* Hardware Wemos D1 mini SPI pins
 D5 GPIO14   CLK         - D0 pin OLED display
 D6 GPIO12   MISO (DIN)  - not connected
 D7 GPIO13   MOSI (DOUT) - D1 pin OLED display
 D1 GPIO5    RST or RES        - RST pin OLED display
 D2 GPIO4    DC          - DC pin OLED
 D8 GPIO15   CS / SS     - CS pin OLED display
*/
int sensorValue = 0;  // Analoge battery A0
char Str6[15];
char StrPWR[15];
char Status[15]= {"[x]"};
char StatusWifi[64]= {"      www.smartpepper.ga"};
int wakeSensor=0; // Wake IR sensor D0
int wakeSatus=0;
int Phase1V =0;
int Phase2V=0;
int Phase3V=0 ;
int Phase1I =0;
int Phase2I=0;
int Phase3I=0 ;
int Phase1P = 0;
int Phase2P=  0;
int Phase3P=  0;
int pwrTotal[42];
int n=0;
int i=0;
unsigned long previousMillis = 0;  
const long interval = 15000;           // interval at which to upload (milliseconds)



// Pin definitions for SPI

// Node MCU
#define OLED_RESET  D1   // RESET
#define OLED_DC     D2   // Data/Command
#define OLED_CS     D8   // Chip select

SH1106 display(true, OLED_RESET, OLED_DC, OLED_CS); // FOR SPI

SH1106Ui ui     ( &display );

bool msOverlay(SH1106 *display, SH1106UiState* state) {

 

  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
   // battery voltage
  sensorValue = analogRead(A0);
  display->fillRect(1 , 3 , 3, 3);
  display->drawRect(3 , 0 ,18 ,9);
 // Serial.println(sensorValue);
  sensorValue = sensorValue*0.0287164;
  //Serial.println(sensorValue);
  display->fillRect(3 ,1 , sensorValue, 8);
  sensorValue = sensorValue*5.5555555;
  String str;
  str=String(sensorValue);
  str.concat('%');
  str.toCharArray(Str6,15);
  display->drawString(24, 0, Str6);
  
  str=String(pwrTotal[n]);
  str.concat('W');
  str.toCharArray(StrPWR,15);
  display->drawString(50, 0,StrPWR);   //CurrentPowerHeaded
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128, 0,Status);   //status

  
  

  return true;
}


bool drawFrame1(SH1106 *display, SH1106UiState* state, int x, int y) {
   display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 +x , 10 + y, "");

  display->setFont(ArialMT_Plain_16);
  display->drawString(0 +x, 20 + y, "   Pepper 1.0");
 
  display->setFont(ArialMT_Plain_10);
  display->drawString(0 +x, 44 + y, StatusWifi);
  return false;
}

bool drawFrame2(SH1106 *display, SH1106UiState* state, int x, int y) {


  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
   //  voltage 1
  display->drawRect(50 +x  , 13+y ,75 ,8);
 // Serial.println(sensorValue);
 // Phase1V = Phase1V*0.0287164;
  //Serial.println(Phase1V);
  String str1;
  str1=String(Phase1V);
  str1.concat(" V");
  str1.toCharArray(Str6,17);
  display->drawString(0 +x, 11 + y, "P1 :");
  display->drawString(19 + x, 11 + y, Str6);
  Phase1V = Phase1V*0.3;
  display->fillRect(50 +x  , 14 + y , Phase1V, 7);
   //  voltage 2
  display->drawRect(50 +x  , 24+y ,75 ,8);
 // Serial.println(sensorValue);
 // Phase1V = Phase1V*0.0287164;
  //Serial.println(Phase1V);
  String str2;
  str2=String(Phase2V);
  str2.concat(" V");
  str2.toCharArray(Str6,17);
  display->drawString(0 +x, 22 + y, "P2 :");
  display->drawString(19 + x, 22 + y, Str6);
  Phase2V = Phase2V*0.3;
  display->fillRect(50 +x  , 25 + y , Phase2V, 7);
   //  voltage 3
  display->drawRect(50 +x  , 35+y ,75 ,8);
 // Serial.println(sensorValue);
 // Phase3V = Phase3V*0.0287164;
  //Serial.println(Phase1V);
  String str3;
  str3=String(Phase3V);
  str3.concat(" V");
  str3.toCharArray(Str6,17);
  display->drawString(0 +x, 33 + y, "P3 :");
  display->drawString(19 + x, 33 + y, Str6);
  Phase3V = Phase3V*0.3;
  display->fillRect(50 +x  , 36 + y , Phase3V, 7);
 display->drawString(0 +x, 44+ y, "Spannung(Voltage/Phase)");
 return false;
}

bool drawFrame3(SH1106 *display, SH1106UiState* state, int x, int y) {
 display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
   //  voltage 1
  display->drawRect(50 +x  , 13+y ,75 ,8);
 // Serial.println(sensorValue);
 // Phase1V = Phase1V*0.0287164;
  //Serial.println(Phase1V);
  String str1;
  str1=String(Phase1I);
  str1.concat(" A");
  str1.toCharArray(Str6,17);
  display->drawString(0 +x, 11 + y, "P1 :");
  display->drawString(19 + x, 11 + y, Str6);
  Phase1I = Phase1I*3.75;
  display->fillRect(50 +x  , 14 + y , Phase1I, 7);
   //  voltage 2
  display->drawRect(50 +x  , 24+y ,75 ,8);
 // Serial.println(sensorValue);
 // Phase1V = Phase1V*0.0287164;
  //Serial.println(Phase1V);
  String str2;
  str2=String(Phase2I);
  str2.concat(" A");
  str2.toCharArray(Str6,17);
  display->drawString(0 +x, 22 + y, "P2 :");
  display->drawString(19 + x, 22 + y, Str6);
  Phase2I = Phase2I*3.75;
  display->fillRect(50 +x  , 25 + y , Phase3I, 7);
   //  voltage 3
  display->drawRect(50 +x  , 35+y ,75 ,8);
 // Serial.println(sensorValue);
 // Phase3V = Phase3V*0.0287164;
  //Serial.println(Phase1V);
  String str3;
  str3=String(Phase3I);
  str3.concat(" A");
  str3.toCharArray(Str6,17);
  display->drawString(0 +x, 33 + y, "P3 :");
  display->drawString(19 + x, 33 + y, Str6);
  Phase3I = Phase3I*3.75;
  display->fillRect(50 +x  , 36 + y , Phase3I, 7);
 display->drawString(0 +x, 44+ y, "     Strom(Current/Phase)");
 return false; 
}
bool drawFrame4(SH1106 *display, SH1106UiState* state, int x, int y) {
   display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->fillRect(0 +x  , 50 + y , 128, 1);
 
  pwrTotal[n] = pwrTotal[n]*0.003333333;
  display->drawRect( x + 3*n  , 50 -pwrTotal[n] , 3 , pwrTotal[n]);

  /*  pwrTotal[n-1] = pwrTotal[n-1]*0.003333333;
  display->drawRect( x + 3*(n-1)  , 50 -pwrTotal[n-1] , 3 , pwrTotal[n-1]);

  pwrTotal[n-2] = pwrTotal[n-2]*0.003333333;
  display->drawRect( x + 3*(n-2)  , 50 -pwrTotal[n-2] , 3 , pwrTotal[n-2]);
  pwrTotal[n-3] = pwrTotal[n-3]*0.003333333;
  display->drawRect( x + 3*(n-3)  , 50 -pwrTotal[n-3] , 3 , pwrTotal[n-3]);
 pwrTotal[n-4] = pwrTotal[n-4]*0.003333333;
  display->drawRect( x + 3*(n-4)  , 50 -pwrTotal[n-4] , 3 , pwrTotal[n-4]);
  pwrTotal[n-5] = pwrTotal[n-5]*0.003333333;
  display->drawRect( x + 3*(n-5)  , 50 -pwrTotal[n-5] , 3 , pwrTotal[n-5]);
  pwrTotal[n-6] = pwrTotal[n-6]*0.003333333;
  display->drawRect( x + 3*(n-6)  , 50 -pwrTotal[n-6] , 3 , pwrTotal[n-6]);
 pwrTotal[n-7] = pwrTotal[n-7]*0.003333333;
  display->drawRect( x + 3*(n-7)  , 50 -pwrTotal[n-7] , 3 , pwrTotal[n-7]);
  pwrTotal[n-8] = pwrTotal[n-8]*0.003333333;
  display->drawRect( x + 3*(n-8)  , 50 -pwrTotal[n-8] , 3 , pwrTotal[n-8]);
  pwrTotal[n-9] = pwrTotal[n-9]*0.003333333;
  display->drawRect( x + 3*(n-9)  , 50 -pwrTotal[n-9] , 3 , pwrTotal[n-9]);
  pwrTotal[n-10] = pwrTotal[n-10]*0.003333333;
  display->drawRect( x + 3*(n-10)  , 50 -pwrTotal[n-10] , 3 , pwrTotal[n-10]);
  pwrTotal[n-11] = pwrTotal[n-11]*0.003333333;
  display->drawRect( x + 3*(n-11)  , 50 -pwrTotal[n-11] , 3 , pwrTotal[n-11]);
  pwrTotal[n-12] = pwrTotal[n-12]*0.003333333;
  display->drawRect( x + 3*(n-13)  , 50 -pwrTotal[n-13] , 3 , pwrTotal[n-13]);
  pwrTotal[n-14] = pwrTotal[n-14]*0.003333333;
  display->drawRect( x + 3*(n-14)  , 50 -pwrTotal[n-14] , 3 , pwrTotal[n-14]);
  pwrTotal[n-15] = pwrTotal[n-15]*0.003333333;
  display->drawRect( x + 3*(n-15)  , 50 -pwrTotal[n-15] , 3 , pwrTotal[n-15]);
  pwrTotal[n-16] = pwrTotal[n-16]*0.003333333;
  display->drawRect( x + 3*(n-16)  , 50 -pwrTotal[n-16] , 3 , pwrTotal[n-16]);
  pwrTotal[n-17] = pwrTotal[n-17]*0.003333333;
  display->drawRect( x + 3*(n-17)  , 50 -pwrTotal[n-17] , 3 , pwrTotal[n-17]);
  pwrTotal[n-18] = pwrTotal[n-18]*0.003333333;
  display->drawRect( x + 3*(n-18)  , 50 -pwrTotal[n-18] , 3 , pwrTotal[n-18]);
  pwrTotal[n-19] = pwrTotal[n-19]*0.003333333;
  display->drawRect( x + 3*(n-19)  , 50 -pwrTotal[n-19] , 3 , pwrTotal[n-19]); */
  display->drawString(0 +x, 50+ y, "     (Power/Time)");
  return false;
}
bool drawFrame5(SH1106 *display, SH1106UiState* state, int x, int y) {
   display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  display->drawStringMaxWidth(0 + x, 10 + y, 128, "Pepper is a smart energy monitoring device that's capable of accurately monitor your energy usage.");
  return false;
}
int frameCount = 5;
bool (*frames[])(SH1106 *display, SH1106UiState* state, int x, int y) = { drawFrame1, drawFrame2, drawFrame3, drawFrame4 ,drawFrame5};

bool (*overlays[])(SH1106 *display, SH1106UiState* state)             = { msOverlay };
int overlaysCount = 1;

void setup() {
  ui.setTargetFPS(30);
  ui.setIndicatorPosition(BOTTOM);
  ui.setIndicatorDirection(LEFT_RIGHT);
  ui.setFrameAnimation(SLIDE_LEFT);
  ui.setFrames(frames, frameCount);
  ui.setOverlays(overlays, overlaysCount);
  ui.init();
 // display.flipScreenVertically();
  ui.update();


  
  Serial.begin(115200);
  Serial.setTimeout(2000);
  delay(10);  
  // Connect to WiFi network  
  Serial.println();  
  Serial.println();  
  Serial.print("Connecting to ");  
  Serial.println(ssid);  
  WiFi.begin(ssid, password);  
  ui.update();
  while (WiFi.status() != WL_CONNECTED)  
  {  
   delay(500); 
   Serial.print(".");  
  }  
  Serial.println("");  
  Serial.println("WiFi connected"); 
  Status[0]='[';
  Status[1]='w';
  Status[2]='i';
  Status[3]='f';
  Status[4]='i';
  Status[5]=']';
  Status[6]='\0';
  // Print the IP address  
  Serial.println(WiFi.localIP());  
  ThingSpeak.begin(client);  


  
  
  
  
}

void loop() { 
  Phase1V =  random(0,250);
  Phase2V=  random(0,250); 
  Phase3V=  random(0,250) ;
  Phase1I =  random(0,20);
  Phase2I=  random(0,20); 
  Phase3I=  random(0,20) ;
  Phase1P = Phase1V*Phase1I;
  Phase2P=  Phase2V*Phase2I;
  Phase3P=  Phase3V*Phase3I;
  if(n<42){
    pwrTotal[n]=Phase1P+Phase2P+Phase3P;
    n++;}
  else{
    n=0;}
  while (WiFi.status() != WL_CONNECTED)  
  {  
   Status[0]='[';
  Status[1]='x';
  Status[2]=']';
  Status[3]='\0';
  }    
  
  static boolean data_state = 0; 
  unsigned long currentMillis = millis();
   if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; 
   if(data_state==0)  
{  
   ThingSpeak.writeField(myChannelNumber, 1, pwrTotal[n], myWriteAPIKey);  
   data_state = 0;  
    //Serial.println("uploaded");
  }  /*
  else if(data_state==1)   
  {  
   ThingSpeak.writeField(myChannelNumber, 2,Phase1V , myWriteAPIKey);  
   data_state = 2;  
  }
  else if(data_state==2)   
  {  
   ThingSpeak.writeField(myChannelNumber, 3,Phase2V , myWriteAPIKey);  
   data_state = 3;  
  }  
  else if(data_state==3)   
  {  
   ThingSpeak.writeField(myChannelNumber, 4,Phase3V , myWriteAPIKey);  
   data_state = 4;  
  } 
  else if(data_state==4)   
  {  
   ThingSpeak.writeField(myChannelNumber, 5,Phase1I , myWriteAPIKey);  
   data_state = 5;  
  }  
  else if(data_state==5)   
  {  
   ThingSpeak.writeField(myChannelNumber, 6,Phase2I , myWriteAPIKey);  
   data_state = 6;  
  }  
  else if(data_state==6)   
  {  
   ThingSpeak.writeField(myChannelNumber, 7,Phase3I , myWriteAPIKey);  
   data_state = 0;  
  }  */
     
    
 }   
  
  
 // Serial.println(pwrTotal[n]);
 
  int remainingTimeBudget = ui.update();

  if (remainingTimeBudget > 0) {
     
    // Don't do stuff if you are below your
    // time budget.
    delay(remainingTimeBudget);
     
    }

 

}
