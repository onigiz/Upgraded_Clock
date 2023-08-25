#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <virtuabotixRTC.h>

#define CLK_PIN 6
#define DATA_PIN 7
#define RST_PIN 8
#define buzzer1 13
#define buzzer2 5
#define button 2
#define button2 3
int month = 0;
int button_value = 0;
int button_value2 = 0;
int alarm_set = 0;
int message_mode = 1;

virtuabotixRTC myRTC(CLK_PIN, DATA_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

int alarm_hours = 0;
int alarm_minutes = 0;
int alarm_type = 0;

byte customChar_Heart[] = {B00000, B01010, B11111, B11111, B11111, B01110, B00100, B00000};
byte customChar_Smiley[] = {B00000, B00000, B01010, B00000, B10001, B01110, B00000, B00000};
byte customChar_Stars[] = {B00000, B00010, B00111, B00010, B01000, B11100, B01000, B00000};
byte customChar_S[] = {B01111, B10000, B10000, B01110, B00001, B00001, B11110, B00100};
byte customChar_i[] = {B00000, B00000, B01100, B00100, B00100, B00100, B00100, B01110};
byte customChar_g[] = {B01110, B00000, B01111, B10001, B10001, B01111, B00001, B01110};
byte customChar_c[] = {B00000, B01110, B10000, B10000, B10001, B01110, B00100, B00000};
byte customChar_u[] = {B01010, B00000, B10001, B10001, B10001, B10011, B01101, B00000};

void setup() 
{
  Serial.begin(9600);
  //myRTC.setDS1302Time(15, 23, 12, 5, 25, 8, 2023);
  lcd.init();
  lcd.backlight();
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(button, INPUT);
  pinMode(button2, INPUT);

  lcd.createChar(0, customChar_Heart);
  lcd.createChar(1, customChar_Smiley);
  lcd.createChar(2, customChar_Stars);
  lcd.createChar(3, customChar_S);
  lcd.createChar(4, customChar_i);
  lcd.createChar(5, customChar_g);
  lcd.createChar(6, customChar_c);
  lcd.createChar(7, customChar_u);
}

/* ALARM ***************************************************************************************************************************/
void alarm1(int pin1, int pin2)
{
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,HIGH);
  delay(700);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(300);
}

void alarm2(int pin1, int pin2)
{
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,HIGH);
  delay(400);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(200);
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,HIGH);
  delay(200);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(100);
}

void alarm3(int pin1, int pin2)
{
  digitalWrite(pin1,HIGH);
  digitalWrite(pin2,HIGH);
  delay(500);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(500);
}

int alarm_type_choose(int butt)
{
  button_value = digitalRead(butt);
  if(button_value == HIGH)
  {
    digitalWrite(buzzer1, HIGH);
    alarm_type++;
    if (alarm_type < 0 || alarm_type > 3)
      alarm_type = 0;
    delay(100);
    digitalWrite(buzzer1, LOW);
    
    return (alarm_type);
  }   
}


//Burada alarm_type'ı yanlışlıkla 3'ten büyük veya 1'den küçük alırsak diye kontrol yazmam lazım
void alarm_call(int alarm_type)
{
  if (alarm_type == 1)
    alarm1(buzzer1 , buzzer2);
  else if (alarm_type == 2)
    alarm2(buzzer1, buzzer2);
  else if (alarm_type == 3)
    alarm3(buzzer1, buzzer2);
}

void alarm_status(int alarm_type)
{
  lcd.setCursor(0, 2);
  lcd.print("Alarm:");
  if (alarm_type <= 3 && alarm_type > 0)
  {
    if (alarm_hours < 10)
      lcd.print("0");
    lcd.print(alarm_hours);
    lcd.print(":");
    if(alarm_minutes < 10)
      lcd.print("0");
    lcd.print(alarm_minutes);
    lcd.print("(");
    lcd.print(alarm_type);
    lcd.print(")");
  }
  else 
    lcd.print("OFF"); 
}

void alarm_set_init_print()
{
  lcd.init();
  lcd.clear();
  lcd.print("Set alarm: ");
  if (alarm_hours < 10)
    lcd.print("0");
  lcd.print(alarm_hours);
  lcd.print(":");
  if (alarm_minutes < 10)
    lcd.print("0");
  lcd.print(alarm_minutes);
  lcd.setCursor(0, 1);
  alarm_type = 0;
  lcd.print("Alarm Type: ");
  if (alarm_type == 0)
    lcd.print("OFF");
}

void alarm_set_func()
{
  alarm_set_init_print();
  while (button_value2 == 2 || button_value2 == 3)
  {    
    while(button_value2 >= 2 && button_value2 < 5)
    {
      if (button_value2 == 2)
      {
        if(digitalRead(button) == HIGH)
        {
          alarm_hours++;
          if (alarm_hours >= 24)
            alarm_hours = 0;
          lcd.setCursor(11, 0);  
          if (alarm_hours < 10)
            lcd.print("0");
          lcd.print(alarm_hours);
          delay(10);
        }
        if(digitalRead(button2) == HIGH)
        {
          digitalWrite(buzzer1, HIGH);
          delay(100);
          digitalWrite(buzzer1, LOW);
          delay(50);
          button_value2 = 3;
        }
      }


      if(button_value2 == 3)
      {
        if(digitalRead(button) == HIGH)
        {
          alarm_minutes += 5;
          if (alarm_minutes >= 60)
            alarm_minutes = 0;
          lcd.setCursor(14, 0);

          if (alarm_minutes < 10)
            lcd.print("0");
          lcd.print(alarm_minutes);
          delay(10);
        }
        if(digitalRead(button2) == HIGH)
        {
          digitalWrite(buzzer1, HIGH);
          delay(100);
          digitalWrite(buzzer1, LOW);
          delay(50);
          button_value2 = 4;
        }
        
      }
      
      if(button_value2 == 4)
      {

        lcd.setCursor(11, 1);
        if (digitalRead(button) == HIGH)
        {
          alarm_type++;
          if (alarm_type > 3 || alarm_type <= 0)
          {
            alarm_type = 0;
            lcd.print("OFF");
          }
          else
          {
            lcd.print(alarm_type);
            lcd.print("   ");
            int i = 0;
            while(i < 2)
            {
              delay(1000);
              alarm_call(alarm_type);
              i++;
            }
          }
          //Bu loopla senkronizasyon alarm_type döngülerinin toplam 1000ms olması lazım her zaman
          
        }


        if(digitalRead(button2) == HIGH)
        {
          digitalWrite(buzzer1, HIGH);
          delay(100);
          digitalWrite(buzzer1, LOW);
          delay(50);
          button_value2 = 5;
        }
      }


      if (button_value2 >= 5)
      {
        if(digitalRead(button2) == HIGH)
        {
          button_value2 = 6;
          digitalWrite(buzzer1, HIGH);
          delay(100);
          digitalWrite(buzzer1, LOW);
          delay(50);
        }
        if (button_value2 = 6)
        {
        digitalWrite(buzzer1, HIGH);
        delay(250);
        digitalWrite(buzzer1, LOW);
        button_value = 1;
        button_value2 = 0;
        }
      }
      delay(500);  
    }
  }
}

/*  DATE - TIME ********************************************************************************************************************/
void months_func(int month)
{
  if (month == 1)
    lcd.print("Ocak");
  else if (month == 2)
  {
    lcd.write(3);
    lcd.print("ubat");
  }
  else if (month == 3)
    lcd.print("Mart");
  else if (month == 4)
    lcd.print("Nisan");
  else if (month == 5)
  {  
    lcd.print("May");
    lcd.write(4);
    lcd.write('s');
  }
  else if (month == 6)
    lcd.print("Haziran");
  else if (month == 7)
    lcd.print("Temmuz");
  else if (month == 8)
  {
    lcd.write('A'); 
    lcd.write(5);
    lcd.print("ustos");
  }
  else if (month == 9)
  {
    lcd.print("Eyl");
    lcd.write(7);
    lcd.write('l');
  }
  else if (month == 10)
    lcd.print("Ekim");
  else if (month == 11)
  {
    lcd.print("Kas");
    lcd.write(4);
    lcd.write('m');
  }
  else if (month == 12)
  {
    lcd.print("Aral");
    lcd.write(4);
    lcd.write('k');
  }
}

void show_date()
{
  lcd.setCursor(0, 0);
  if (myRTC.dayofmonth < 10)
    lcd.print("0");
  lcd.print(myRTC.dayofmonth);
  lcd.print(" ");
  months_func(myRTC.month);
  lcd.print(" ");
  if (myRTC.year < 10)
    lcd.print("0");
  lcd.print(myRTC.year);
}

void show_hour()
{
  lcd.setCursor(0, 1);
  if (myRTC.hours < 10)
    lcd.print("0");
  lcd.print(myRTC.hours);
  lcd.print(".");
  if (myRTC.minutes < 10)
    lcd.print("0");
  lcd.print(myRTC.minutes);
  lcd.print(".");
  if (myRTC.seconds < 10)
    lcd.print("0");
  lcd.print(myRTC.seconds);
  delay(1000);
}

/*****************************************************************************************************************************************/
void loop() 
{
  myRTC.updateTime();
  
  lcd.clear();
  alarm_type_choose(button);
  show_date();
  alarm_status(alarm_type);
  show_hour();

  //alarm modların toplam delay'i 1000'e eşit olmazsa saniye yazdırımında atlamalar meydana geliyor
  if (alarm_hours == myRTC.hours && alarm_minutes == myRTC.minutes)
    alarm_call(alarm_type);

  if (digitalRead(button2) == HIGH)
    button_value2++;
  if (button_value2 == 2)
  {
    digitalWrite(buzzer1, HIGH);
    delay(250);
    digitalWrite(buzzer1, LOW);
    alarm_set_func();
  }  
}
