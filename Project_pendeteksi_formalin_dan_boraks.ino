#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define  s0 25
#define s1 26
#define s2 27
#define s3 14
#define  out 12

int dataR = 0;
int dataG = 0;
int dataB = 0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pinButton = 5;
const int pinSensorHCHO = 32;
const int pinLedHijau = 4;
const int pinLedMerah = 2;
const int pinBuzzer = 15;

const int AMBANG_FORMALIN = 550;
const int BATAS_AMAN_FORMALIN = 300;
bool isLockedFormalin = false;
int mode = 0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  
  pinMode(pinButton, INPUT_PULLUP);
  pinMode(pinLedHijau, OUTPUT);
  pinMode(pinLedMerah, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  pinMode(s0,OUTPUT);
  pinMode(s1,OUTPUT);
  pinMode(s2,OUTPUT);
  pinMode(s3,OUTPUT);
  pinMode(out,INPUT);
  
  digitalWrite(s0,HIGH);
  digitalWrite(s1,HIGH);

  lcd.setCursor(4,0);
  lcd.print("BOFORMAN");
  lcd.setCursor(4,1);
  lcd.print("STARTING");
  digitalWrite(pinBuzzer, HIGH);
  delay(200);
  digitalWrite(pinBuzzer, LOW);
  delay(5000);
  lcd.clear();
}

void loop() {
  if (digitalRead(pinButton) == LOW) {
    mode++; 
    if (mode > 1) mode = 0; 
    
    isLockedFormalin = false; 
    digitalWrite(pinBuzzer, LOW);
    digitalWrite(pinLedMerah, LOW);
    digitalWrite(pinLedHijau, LOW);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Pindah Mode...");
    delay(1000); 
    lcd.clear();
  }

  switch (mode) {
    
    case 0: {
      int nilaiHCHO = analogRead(pinSensorHCHO);

      if (!isLockedFormalin) {
        digitalWrite(pinLedHijau, HIGH);
        digitalWrite(pinLedMerah, LOW);
        digitalWrite(pinBuzzer, LOW);

        lcd.setCursor(0, 0);
        lcd.print("Mode: CH2O      ");
        lcd.setCursor(0, 1);
        lcd.print("CLEAR");
        Serial.println(nilaiHCHO);

        if (nilaiHCHO >= AMBANG_FORMALIN) {
          isLockedFormalin = true;
          lcd.clear();
        }
      } 
      else {
        digitalWrite(pinLedHijau, LOW);
        digitalWrite(pinLedMerah, HIGH);
        
        digitalWrite(pinBuzzer, HIGH); delay(200);
        digitalWrite(pinBuzzer, LOW);  delay(200);

        lcd.setCursor(0, 0);
        lcd.print("Mode: CH2O  ");
        lcd.setCursor(0, 1);
        lcd.print("CH2O DETECTED");

        if (nilaiHCHO < BATAS_AMAN_FORMALIN) {
          isLockedFormalin = false;
          lcd.clear();
        }
      }
      break; 
    }

    case 1: {
      digitalWrite(s2,LOW);
      digitalWrite(s3,LOW);
      Serial.print("Red value = ");
      dataR = pulseIn(out,LOW);
      Serial.print(dataR);
      Serial.print("\	");
      delay(20);

      digitalWrite(s2,HIGH);
      digitalWrite(s3,HIGH);
      Serial.print("Green value = ");
      dataG = pulseIn(out,LOW);
      Serial.print(dataG);
      Serial.print("\	");
      delay(20);

      digitalWrite(s2,LOW);
      digitalWrite(s3,HIGH);
      Serial.print("Blue value= ");
      dataB = pulseIn(out,LOW);
      Serial.print(dataB);
      Serial.print("\	");
      delay(20);

      Serial.println();

      if (dataR < 55 and dataG < 65 and dataB < 50){
        if (dataR > 30 and dataG > 40 and dataB > 45){
          digitalWrite(pinLedHijau, LOW);
          digitalWrite(pinLedMerah, HIGH);
          
          digitalWrite(pinBuzzer, HIGH); delay(200);
          digitalWrite(pinBuzzer, LOW);  delay(200);

          lcd.setCursor(0, 0);
          lcd.print("Mode: BORAKS");  
          lcd.setCursor(0, 1);
          lcd.print("BORAKS DETECTED");
        } else {
          lcd.clear();
          digitalWrite(pinLedHijau, HIGH);
          digitalWrite(pinLedMerah, LOW);
          lcd.setCursor(0, 0);
          lcd.print("Mode: BORAKS");
          lcd.setCursor(0, 1);
          lcd.print("CLEAR");
        }
      } else {
        lcd.clear();
        digitalWrite(pinLedHijau, HIGH);
        digitalWrite(pinLedMerah, LOW);
        lcd.setCursor(0, 0);
        lcd.print("Mode: BORAKS");
        lcd.setCursor(0, 1);
        lcd.print("CLEAR");
      }
      break; 
    }
  }
  delay(50); 
}
