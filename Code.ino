#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);    //Declaring the 16x2 LCD
int relays = 12, trigPin = 11, echoPin = 10, ON = 9, OFF = 8, ldr = A0, distance, max = 90, min = 20;    //Declaring all the integer type variables
bool status = false, emergency = true, on_allowed = true, first = true;   //Declaring all the bool type variables 
long duration;    //Declaring long type variable

void setup()
{
  lcd.begin(16, 2);   //Initializing the 16x2 LCD
  pinMode(relays, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ON, OUTPUT);
  pinMode(OFF, OUTPUT);
  digitalWrite(relays, LOW);    //Intial state of the relays which switch ON/OFF the Pump is off to save some power
  digitalWrite(OFF, HIGH);    //Initial state of OFF relay is on so that the pump can be switched on manually
}

void loop()
{
  lcd.clear();    //Clearing the LCD in the beginning of the loop everytime to avoid overwriting on the LCD
  lcd.setCursor(0, 0);
  if (analogRead(ldr) <= 850)   //ldr is the variable that stores the data from photoresistor that is put near the light which is switched on evertime the generator is turned on. This is to avoid switiching on the pump why the generator is on
  {
    lcd.print("Motor:");
    if (status == false)
    {
      lcd.print("OFF");
    }
    else
    {
      lcd.print("ON");
    }
  }
  else
  {
    lcd.print("Generator is on");
  }
  
  //This section is for calculating the distance of the water level in the tank that the ultrasonic sensor reads
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.0343 / 2;
  lcd.setCursor(0, 1);
  lcd.print("Distance:");
  lcd.print(distance);

  if (first == false)   //This is just to let all the sensor values stabalize 
  {
    if (analogRead(ldr) <= 900)   //900 is the minimum value of the ldr when the generators light is on
    {
      if (distance >= max && status == false)   //When the tank is empty and the pump is off, the pump is switched on
      {
        digitalWrite(ON, LOW);
        digitalWrite(relays, HIGH);
        delay(500);
        digitalWrite(relays, LOW);
        digitalWrite(ON, HIGH);
        status = true;
      }
      if (distance <= min && on_allowed == true)    //When the tank has been filled (whether switched on automatically or manually), the pump is switched off
      {
        digitalWrite(OFF, LOW);
        status = false;
        on_allowed = false;
      }
      if (distance >= (min + 5) && on_allowed == false)   //This is to allow the pump to be switched on manually after the tank has been filled so that the tank doesnt overflow
      {
        digitalWrite(OFF, HIGH);
        on_allowed = true;
      }
    }
    else
    {
      on_allowed = false;
    }
  }
  else
  {
    first = false;
  }
  delay(1000);
}
