#include <Wire.h>
int currentPins[3] = {A0,A1,A2};              //Assign phase CT inputs to analog pins 
int voltagePins[3] = {A3,A4,A5};              //Assign phase VT inputs to analog pins 
float calib[3] = {1,1,1}; 
float calibv[3] = {204.6,204.6,204.6}; 
float kilos[3];
int Totalkilos;
unsigned long startMillis[3]; 
unsigned long endMillis[3]; 
float RMSCurrent[3];
float RMSVoltage[3];
//double RMSCurrent[3];
float RMSPower[3]; 
float peakPower[3]; 

//unsigned long myChannelNumber = 359593;
//const char * myWriteAPIKey = "X4G7UOIVL2JV3BAD";

void setup() {
   Serial.begin(9600);  
   Wire.begin(8);                /* join i2c bus with address 8 */
   Wire.onRequest(requestEvent); /* register request event */
  pinMode(currentPins[0], INPUT);
  pinMode(currentPins[1], INPUT);
  pinMode(currentPins[2], INPUT);
  pinMode(voltagePins[0], INPUT);
  pinMode(voltagePins[1], INPUT);
  pinMode(voltagePins[2], INPUT);
}
void loop() {
 
  readPhase(); 
 // uploadThingSpeak(); 
  
 /// readPhase(); 
//  displayPeakPower (); 
//  delay(3000); 
   }
void readPhase ()      //Method to read information from CTs 
{ 
  for(int i=0;i<=2;i++) 
  { 
    startMillis[i]= millis(); 
    int current = 0; 
    int maxCurrent = 0; 
    int minCurrent = 1000; 
    int voltage = 0; 
    int maxVoltage = 0; 
    int minVoltage = 1000; 
    for (int j=0 ; j<=200 ; j++)  //Monitors and logs the current input for 200 cycles to determine max and min current 
    { 
      current =  analogRead(currentPins[i]);    //Reads current input and records maximum and minimum current 
      voltage =  analogRead(voltagePins[i]);    //Reads current input and records maximum and minimum voltage
      if(current >= maxCurrent) 
        maxCurrent = current; 
      else if(current <= minCurrent) 
        minCurrent = current; 
      if(voltage >= maxVoltage) 
        maxVoltage = voltage; 
      else if(voltage <= minVoltage) 
        minVoltage = voltage; 
    } 
    if (maxCurrent <= 512)  { 
      maxCurrent = 512;
    } 
    RMSCurrent[i] = ((maxCurrent - 512)*0.707)/calib[i];    //Calculates RMS current based on maximum value and scales according to calibration 
    RMSVoltage[i] = ((maxVoltage)*0.707)/calibv[i];    //Calculates RMS voltage based on maximum value and scales according to calibration 
    RMSPower[i] = RMSVoltage[i]*RMSCurrent[i];    //Calculates RMS Power Assuming pf 1.0, change to 110VAC accordingly 
    if (RMSPower[i] > peakPower[i]) 
    { 
      peakPower[i] = RMSPower[i]; 
    } 
    displayCurrent(); 
    displayVoltage();
    displayRMSPower();
    
    endMillis[i]= millis(); 
    unsigned long time = (endMillis[i] - startMillis[i]); 
    kilos[i] = kilos[i] + ((double)RMSPower[i] * ((double)time/60/60/1000000));    //Calculate kilowatt hours used  
  } 
}
void displayCurrent (){
  Serial.print(RMSCurrent[0]);
  Serial.print("A | ");
  Serial.print(RMSCurrent[1]);
  Serial.print("A | ");
  Serial.print(RMSCurrent[2]);
  Serial.print("A | ");
  Serial.println("  ");
}
void displayRMSPower ()
{
  Serial.print(RMSPower[0]);
  Serial.print("VA rms | ");
  Serial.print(RMSPower[1]);
  Serial.print("VA rms | ");
  Serial.print(RMSPower[2]);
  Serial.print("VA rms | Total Pwr(rms) ");
  Totalkilos = kilos[2]+kilos[1]+kilos[0];
  Serial.print(Totalkilos);
  Serial.print("VA | ");
  Serial.println("  ");
} 
void displayVoltage(){
  Serial.print(RMSVoltage[0]);
  Serial.print("V | ");
  Serial.print(RMSVoltage[1]);
  Serial.print("V | ");
  Serial.print(RMSVoltage[2]);
  Serial.print("V | ");
  Serial.println("  ");  
}
void requestEvent() {   /// I2c Communication
 Totalkilos = random(0,250);
 Wire.write(Totalkilos);  /*send string on request */
}

