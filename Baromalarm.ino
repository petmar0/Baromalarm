/*
 * Baroalarm
 * 05/17/2016, Pete Marchetto, marchetto@umn.edu
 * This program uses the BMP180 pressure and temperature sensor to take readings, then display them on a TM1637 4-digit display module.
 * The program also has the capability of using an alarm if the dP/dt rate is too high.
 * The BMP180 is set up on the SCK and SDA lines, while the display can be used on any two GPIO pins, and the alarm can be used between any GPIO pin and ground.
 */

#include <SFE_BMP180.h>           //Pressure sensor header
#include <Wire.h>                 //I2C header for pressure sensor
#include <Arduino.h>              //??? Supposedly needed for display
#include <TM1637Display.h>        //Display header

int alarm=4;                      //Alarm buzzer pin
int CLK=5;                        //Display clock line
int DIO=6;                        //Display data line
double dPdt=0.1;                  //Set the dP/dt alarm threshold; default is 0.1 mBar/sec
SFE_BMP180 pressure;              //Pressure sensor object
TM1637Display display(CLK, DIO);  //Display object

double T,P;                       //Doubles for storing temp and pressure
int alarmed=false;

void setup() {
  pressure.begin();               //Starting the pressure sensor object
  Serial.begin(9600);             //Starting the serial line for debugging
  pinMode(4, OUTPUT);             //Setting up the pin for the buzzer
  display.setBrightness(0x0f);    //Setting the display brightness
}

void loop() {
  double T1=T;                    //Creating a double to store the last T value in for dT/dt
  double P1=P;                    //Creating a double to store the last P value in for dP/dt
  Serial.print(millis());         //Send the number of msec since launch to the serial terminal
  Serial.print(",");              //Send a comma to the serial terminal
  pressure.getTemperature(T);     //Get temp from pressure sensor
  Serial.print(T);                //Send the temp to the serial terminal
  Serial.print(",");              //Send a comma to the serial terminal
  Serial.print(T-T1);             //Send ∆T to the serial terminal
  Serial.print(",");              //Send a comma to the serial terminal
  display.showNumberDec(T);       //Send the temperature to the display
  delay(2000);                    //Wait 2000 msec (2 sec)
  pressure.getPressure(P,T);      //Get the pressure from the pressure sensor
  Serial.print(P);                //Send the pressure to the serial terminal
  Serial.print(",");              //Send a comma to the serial terminal
  Serial.println(P-P1);           //Send ∆P to the serial terminal
  display.showNumberDec(P);       //Send the pressure to the display
  if(abs(P-P1)>dPdt*10){          //If the dP/dt is greater than dPdt mBar/sec:
    for(int i=0; i<3; i++){       //Go into a for loop for four repetitions
      digitalWrite(alarm, HIGH);  //Beep the buzzer for
      delay(100);                 //100 msec (0.1 sec)
      digitalWrite(alarm, LOW);   //And turn it back off for
      delay(100);                 //100 msec (0.1 sec)
      alarmed++;                  //Add one to the alarm counter
    }                             //Before repeating, then escaping the for loop after four goes
  }                               //And escaping the if statement
  delay(8000);                    //Then wait for 8000 msec (8 sec) before repeating  
}
