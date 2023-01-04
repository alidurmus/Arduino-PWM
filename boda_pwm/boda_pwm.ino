/*
ilk karşılama ekranı gelsin 3 sn beklesin
ana menu gelsin ( Sweep Test, Range Test) OK tuşuna basılırsa test ekranı gelsin
Sweep Mode Test
daha sonra Sweep mode ekranı gelsin 
Sweep başlatmak için enter tuşuna basılsın iptal etmek için cancel tuşuna basılsın

Range Test
 kademe test ekranına gelsin 
 test ekranında yukarı aşağı tuşu kademeleri değiştirir
 sağ sol tuşu value değerini değiştirir
test sırasında ana menuye geçmek için cancel tuşuna 

*/



#include <LiquidCrystal.h>
const int rs = 12, en = 10, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int pwm_pin = 9;

const byte rightButtonPin = 11;
const byte leftButtonPin = 8;
const byte upButtonPin = 6;
const byte downButtonPin = 7;
const byte selectButtonPin = 13;
//const char cancelButtonPin = A0; //a0
int count = 0;

// kademeye göre bit değeri
int Range_0 = 0;
int Range_10 = 25;
int Range_20 = 51;
int Range_30 = 76;
int Range_40 = 102;
int Range_50 = 127;
int Range_60 = 153;
int Range_70 = 178;
int Range_80 = 204;
int Range_90 = 229;
int Range_100 = 255;


// kademelere göre voltaj değeri
float Range_0_DC = 0.83;//166 
float Range_10_DC = 1.03;//206
float Range_20_DC = 1.23;//247
float Range_30_DC = 1.42;//285
float Range_40_DC = 1.62;//324
float Range_50_DC = 1.80;//362
float Range_60_DC = 2.00;//400
float Range_70_DC = 2.18;//437
float Range_80_DC = 2.37;//476
float Range_90_DC = 2.57;//514
float Range_100_DC = 2.75;//551

// 5V referans alınarak 255 bit karşılığı
// 255bit x 0,83 / 5V
int Range_0_PWM = 42;//0.83;//166 
int Range_10_PWM = 52;//1.03;//206
int Range_20_PWM = 63;//1.23;//247
int Range_30_PWM = 72;//1.42;//285
int Range_40_PWM = 83;//1.62;//324
int Range_50_PWM = 92;//1.80;//362
int Range_60_PWM = 102;//2.00;//400
int Range_70_PWM = 111;//2.18;//437
int Range_80_PWM = 121;//2.37;//476
int Range_90_PWM = 131;//2.57;//514
int Range_100_PWM = 140;//2.75;//551

// kademelerin değer karşılık 
int Range_0_VALUE = 166;
int Range_10_VALUE = 206;
int Range_20_VALUE = 247;
int Range_30_VALUE = 285;
int Range_40_VALUE = 324;
int Range_50_VALUE = 362;
int Range_60_VALUE = 400;
int Range_70_VALUE = 437;
int Range_80_VALUE = 476;
int Range_90_VALUE = 514;
int Range_100_VALUE = 551;

char key = 'a';
int menuDurum = 0;
int menu = 1;
int menuSize = 3;

int tolerance = 5; // yüzde tolerans değeri
int deviation = 5; //

void setup() {
  Serial.begin(9600);
  pinMode(pwm_pin, OUTPUT);
  //pinMode(A0, INPUT_PULLUP); 
  
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(selectButtonPin, INPUT_PULLUP);
  pinMode(A0, INPUT_PULLUP);
  lcd.begin(20, 4);
  lcd.print(" EMERGENCY");
  lcd.setCursor(0,1);
  lcd.print(" POWER UNIT ");
  lcd.setCursor(0,2);
  lcd.print(" EPU ");
  
  delay(3000);
  lcd.clear();// clearing the LCD
  
  updateMenu();
}

void loop() {
  Serial.println(menu);
  if (digitalRead(upButtonPin) == LOW){
    menu--;
    updateMenu();
    delay(1000);
    
  }
  if (digitalRead(downButtonPin) == LOW){
    menu++;
    updateMenu();
    delay(1000);
   
  }
  if (digitalRead(selectButtonPin) == LOW){
    executeAction();
    updateMenu();
    delay(1000);
    
  }
}

void updateMenu() {
 if(menu > menuSize){
    menu = menuSize;
 }else if(menu < 1){
  menu = 1;
 }else{
   switch (menu) {    
    case 1:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(">SWEEP TEST");
      lcd.setCursor(0, 1);
      lcd.print(" RANGE TEST");
      lcd.setCursor(0, 2);
      lcd.print(" AUTO TEST");
      break;
    case 2:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" SWEEP TEST");
      lcd.setCursor(0, 1);
      lcd.print(">RANGE TEST");
      lcd.setCursor(0, 2);
      lcd.print(" AUTO TEST");
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" SWEEP TEST");
      lcd.setCursor(0, 1);
      lcd.print(" RANGE TEST");
      lcd.setCursor(0, 2);
      lcd.print(">AUTO TEST");
      break;
  }
 }
}

void executeAction() {
  switch (menu) {
    case 1:
      sweepTest();
      break;
    case 2:
      manualTest();
      break;
    case 3:
      autoTest();
      break;
  }
}

void sweepTest() {
  bool durum = true;
  int rangeAuto  = 0;
  Serial.println("SWEEP TEST BEGIN");
  delay(100);
  while(durum)
  {
    for (int i = 0; i <= 6; i++) {     
      analogWrite(pwm_pin, testRange(rangeAuto));
      Serial.println(testRange(rangeAuto));

      lcd.clear();
      lcd.setCursor(0, 0);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print("SWEEP TEST");

      lcd.setCursor(0, 1);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print("Range");
      lcd.setCursor(10, 1);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print(rangeAuto);
      
      lcd.setCursor(0, 2);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print("Value");
      lcd.setCursor(10, 2);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print(testValue(rangeAuto));
      delay(5000);

      if(rangeAuto == 0){
        rangeAuto = 100;
      }else{
        rangeAuto = 0;
      }
      
      if(digitalRead(A0) == LOW)
      {
        analogWrite(pwm_pin, 0);
        durum= false;
        Serial.println("RANGE TEST OUT");
      }
    }
    analogWrite(pwm_pin, 0);
    durum= false;
    Serial.println("RANGE TEST OUT");
    
    delay(100);
  }
}

void manualTest() {
  bool durum = true;
  int range = 0;
  lcd.clear();
  Serial.println("RANGE TEST BEGIN");
  while(durum)
  { 
   

    if (digitalRead(upButtonPin) == LOW){
      range += 10;
      if(range > 100){
        range = 100;
      }      
      delay(100);      
    }
    if (digitalRead(downButtonPin) == LOW){
      range -= 10;
      if(range < 0){
        range = 0;
      }   
      delay(100);    
    }

    if (digitalRead(rightButtonPin) == LOW){
      deviation++;   
      range = range + deviation;
      Serial.println(deviation);       
      delay(100);      
    }
    if (digitalRead(leftButtonPin) == LOW){
       deviation--;    
       range = range + deviation;  
       Serial.println(deviation);  
      delay(100);    
    }

    

    analogWrite(pwm_pin, testRange(range));
    Serial.println(testRange(range));
    lcd.clear();
    lcd.setCursor(0, 0);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print("RANGE TEST");

    lcd.setCursor(0, 2);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print("Range");
    lcd.setCursor(10, 2);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print(range);

    lcd.setCursor(0, 1);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print("Value");
    lcd.setCursor(10, 1);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print(testValue(range));

    lcd.setCursor(0, 3);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print("Deviation");
    lcd.setCursor(10, 3);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
    lcd.print(deviation);

    delay(500);

    if(digitalRead(A0) == LOW)
    {
      analogWrite(pwm_pin, 0);
      durum= false;
      Serial.println("AUTO TEST OUT");
    }
  }
}

void autoTest() {
  bool durum = true;
  int rangeAuto  = 0;
  Serial.println("AUTO TEST BEGIN");
  delay(100);
  while(durum)
  { 
    for (int i = 0; i <= 100; i+=10) {
      analogWrite(pwm_pin, testRange(i));
      Serial.println(testRange(i));

      lcd.clear();
      lcd.setCursor(0, 0);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print("AUTO TEST");

      lcd.setCursor(0, 2);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print("Range");
      lcd.setCursor(10, 2);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print(i);
      
      lcd.setCursor(0, 1);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
      lcd.print("Value");
      lcd.setCursor(10, 1);    // ekranın alt satırına yazı yazdırmak için cursor konumlandırılır.
     
      lcd.print(testValue(i));
      delay(10000);

      if(digitalRead(A0) == LOW)
      {
        analogWrite(pwm_pin, 0);
        durum= false;
        Serial.println("AUTO TEST OUT");
      }
    }
    analogWrite(pwm_pin, 0);
    durum= false;
    Serial.println("AUTO TEST OUT");
    delay(100);
  }
}

int testRange(int range) {
  int returnValue = 0;
  
 switch (range) {
    case 0:
      return  Range_0_PWM;
      break;
    case 10:
      return Range_10_PWM;
      break;
    case 20:
      return Range_20_PWM;
      break;
    case 30:
      return  Range_30_PWM;
      break;
    case 40:
      return  Range_40_PWM; 
        break;
    case 50:
      return  Range_50_PWM;
        break;
    case 60:
      return  Range_60_PWM;
        break;
    case 70:
      return  Range_70_PWM;
        break;
    case 80:
      return Range_80_PWM;
        break;
    case 90:
      return  Range_90_PWM;
        break;
    case 100:
      return  Range_100_PWM;
        break;
  }
}

int testValue(int range) {
  int returnValue = 0;
  
 switch (range) {
    case 0:
      return  Range_0_VALUE;
      break;
    case 10:
      return Range_10_VALUE;
      break;
    case 20:
      return Range_20_VALUE;
      break;
    case 30:
      return  Range_30_VALUE;
      break;
    case 40:
      return  Range_40_VALUE; 
        break;
    case 50:
      return  Range_50_VALUE;
        break;
    case 60:
      return  Range_60_VALUE;
        break;
    case 70:
      return  Range_70_VALUE;
        break;
    case 80:
      return Range_80_VALUE;
        break;
    case 90:
      return  Range_90_VALUE;
        break;
    case 100:
      return  Range_100_VALUE;
        break;
  }
}

