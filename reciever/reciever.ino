#include <LiquidCrystal.h>


const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int buzzer = 7;

int val;
int val2;
char reset = 0;
String duration;

int sensorValue = 0; // variable to store the value coming from the sensor
int sensorPin = A0;  // select the input pin for LDR

int ultrasonic_timer = 0;
int fume_timer = 0;
int magnetic_timer = 0;

char last_state = 0;

int mute_timer = 0;
int mute_timer_active = 0;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(8, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(buzzer, OUTPUT);

  Serial.println("Project starting");
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("LiFi Project");
  delay(3000);
  lcd.clear();
  lcd.print("No Data");
}

void loop()
{
  int startTime = millis();
  if (reset)
  {
    lcd.clear();
    lcd.print("No Data");
    reset = 0;
  }
  int val = 0;
  unsigned long previousTime = millis();
  do
  {
    sensorValue = analogRead(sensorPin);

    if (sensorValue > 300)
    {
      val = 1;
    }
    else
    {
      val = 0;
    }
  } while (val != 0);
  unsigned long currentTime = millis();
  unsigned long difference = currentTime - previousTime;
  if (difference > 2)
  {
    Serial.print("Difference ");
    Serial.println(difference);
    // reset = 1;
  }
  if ((difference >= 40) && (difference <= 60))
  {
    fume_timer = 30000;
  }
  if ((difference >= 140) && (difference <= 160))
  {
    magnetic_timer = 30000;
  }
  if ((difference >= 240) && (difference <= 260))
  {
    ultrasonic_timer = 30000;
  }

  if (ultrasonic_timer > 0 && fume_timer > 0 && magnetic_timer > 0)
  {
    if (last_state != 1)
    {
      lcd.clear();
      lcd.print("Kolo shaghaaaaal");
    }
    last_state = 1;
  }
  else if (ultrasonic_timer > 0 && fume_timer > 0)
  {
    if (last_state != 2)
    {
      lcd.clear();
      lcd.print("ULTRA and FUME");
    }
    last_state = 2;
  }
  else if (fume_timer > 0 && magnetic_timer > 0)
  {
    if (last_state != 3)
    {
      lcd.clear();
      lcd.print("FUME and MAG");
    }
    last_state = 3;
  }
  else if (ultrasonic_timer > 0 && magnetic_timer > 0)
  {
    if (last_state != 4)
    {
      lcd.clear();
      lcd.print("ULTRA and MAG");
    }
    last_state = 4;
  }
  else if (fume_timer > 0)
  {
    if (last_state != 5)
    {
      lcd.clear();
      lcd.print("Fume");
    }
    last_state = 5;
  }
  else if (magnetic_timer > 0)
  {
    if (last_state != 6)
    {
      lcd.clear();
      lcd.print("Magneitc");
    }
    last_state = 6;
  }
  else if (ultrasonic_timer > 0)
  {
    if (last_state != 7)
    {
      lcd.clear();
      lcd.print("Ultrasonic");
    }
    last_state = 7;
  }
  else
  {
    if (last_state != 0)
    {
      lcd.clear();
      Serial.println("Resetting lcd");
      reset = 1;
    }
    last_state = 0;
  }

  int endTime = millis();

  int timeDifference = endTime - startTime;
  if (fume_timer > 0)
  {
    fume_timer -= timeDifference;
  }
  if (magnetic_timer > 0)
  {
    magnetic_timer -= timeDifference;
  }
  if (ultrasonic_timer > 0)
  {
    ultrasonic_timer -= timeDifference;
  }


  /*Mute section*/
  int muteButton   = digitalRead(8);

  if (last_state == 0)
  {
    noTone(buzzer);
  }
  else
  {
    tone(buzzer, 1000);
  }

  if (mute_timer <= 0)
  {
    if (mute_timer_active == 1)
    {
      Serial.println("Setting last state");
      Serial.println(last_state);
      last_state = 0;
      reset = 1;
      ultrasonic_timer = 0;
      fume_timer = 0;
      magnetic_timer = 0;
      mute_timer_active = 0;
    }
    if (!muteButton)
    {
      mute_timer = 5000;
      mute_timer_active = 1;
      Serial.println("Setting timer");
    }
    
  }
  else
  {
    mute_timer -= timeDifference;
  }
}
