#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int fingerReturn;
int Green = 8;
int Yellow = 9;
int Red = 10;
int Button = 12;

void setup()
{
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Yellow, OUTPUT);
  pinMode(Button, INPUT);
  
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.print("L1;");
  } else {
    Serial.print("L0;");
    while (1) { delay(1); }
  }

  while (! Serial.available());
    if (Serial.readStringUntil(";") == "1;") {
      digitalWrite(Green, HIGH); delay(250); digitalWrite(Green, LOW);
    } else {
      digitalWrite(Red, HIGH); delay(250); digitalWrite(Red, LOW);
    }
}

void loop()                    // run over and over again
{
  if (digitalRead(Button) == 0) {
    do {
      fingerReturn = getFingerprintIDez();
      digitalWrite(Yellow, LOW);
      digitalWrite(Red, LOW);
      
      if (fingerReturn >= 0) {
        digitalWrite(Yellow, HIGH);
      } else if (fingerReturn == -1) {
        digitalWrite(Red, HIGH);
      }
      
      delay(5);
    } while (fingerReturn < 0);
    
    while (! Serial.available());
    
    if (Serial.readStringUntil(";") == "1;") {
      digitalWrite(Yellow, LOW);
      digitalWrite(Green, HIGH);
      delay(200);
      digitalWrite(Green, LOW);
    } else {
      digitalWrite(Yellow, LOW);
      digitalWrite(Red, HIGH);
      delay(200);
      digitalWrite(Red, LOW);
    }
  }
}

int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -2;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -2;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) {
    return -1;
  }
  
  // found a match!
  Serial.print(finger.fingerID);Serial.print(";");
  return finger.fingerID; 
}
