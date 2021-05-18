#include <LiquidCrystal.h>
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
int relays = 12;
int trigPin = 11;
int echoPin = 10;
int ldr = A0;
int ON1 = 9;
int ON2 = 13;
int OFF = 8;
int distance;
long duration;
bool motor_status = false;
bool emergency = true;
bool on_allowed = true;
bool first = true;

void setup()
{
  lcd.begin(16, 2);
  pinMode(relays, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ON1, OUTPUT);
  pinMode(ON2, OUTPUT);
  pinMode(OFF, OUTPUT);
  digitalWrite(relays, LOW);
  digitalWrite(OFF, HIGH);
}

void loop()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  if (analogRead(ldr) <= 850)
  {
    lcd.print("Motor:");
    if (motor_status == false)
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
  lcd.print(" ");
  lcd.print(analogRead(ldr));

  if (first == false)
  {
    if (analogRead(ldr) <= 850)
    {
      if (distance >= 90 && motor_status == false)
      {
        digitalWrite(ON1, LOW);
        digitalWrite(ON2, LOW);
        digitalWrite(relays, HIGH);
        delay(500);
        digitalWrite(relays, LOW);
        digitalWrite(ON1, HIGH);
        digitalWrite(ON2, HIGH);
        motor_status = true;
      }
      if (distance <= 20 && on_allowed == true)
      {
        digitalWrite(OFF, LOW);
        motor_status = false;
        on_allowed = false;
      }
      if (distance >= 25 && on_allowed == false)
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
