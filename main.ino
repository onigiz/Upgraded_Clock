#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <virtuabotixRTC.h>

#define CLK_PIN 6
#define DATA_PIN 7
#define RST_PIN 8
#define buzzer1 13
#define buzzer2 5
int month = 0;

virtuabotixRTC myRTC(CLK_PIN, DATA_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 20, 4);

char *months[12] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int alarm_hours = 1;
int alarm_minutes = 00;
int alarm_type = 3;

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
  lcd.init();
  lcd.backlight();
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);

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
  delay(100);
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  delay(100);
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

  show_date();
  show_hour();


  //alarm modların toplam delay'i 1000'e eşit olmazsa saniye yazdırımında atlamalar meydana geliyor
  if (alarm_hours == myRTC.hours && alarm_minutes == myRTC.minutes)
    alarm_call(alarm_type);  
}
