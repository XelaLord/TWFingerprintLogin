#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int Green = 8;
int White = 9;
int Red = 10;
int Button = 12;

void setup()  
{
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(White, OUTPUT);
  pinMode(Button, INPUT);
  
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.print("R1;");
  } else {
    Serial.print("R0;");
    while (1) { delay(1); }
  }

  while (! Serial.available());
    if (Serial.readStringUntil(";") == "1;") {
      digitalWrite(Green, HIGH); delay(250); digitalWrite(Green, LOW);
    } else {
      digitalWrite(Red, HIGH); delay(250); digitalWrite(Red, LOW);
    }
}

void loop() {
  if (digitalRead(Button) == 0) {
    while (Serial.available()) {
      Serial.read();
    }
    Serial.print(1);

    while (! Serial.available());

    if (Serial.parseInt() == 1) {
      finger.emptyDatabase();
      Serial.print(1);
      for (int i=0;i<5;i++) {
        int d = 50;
        digitalWrite(Green, HIGH); digitalWrite(White, HIGH); digitalWrite(Red, HIGH);
        delay(d);  
        digitalWrite(Green, LOW); digitalWrite(White, LOW); digitalWrite(Red, LOW);
        delay(d);
      }
    }
  }
}
