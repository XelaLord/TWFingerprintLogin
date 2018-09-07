#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

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
    Serial.print("E1;");
  } else {
    Serial.print("E0;");
    while (1) { delay(1); }
  }

  while (! Serial.available());
  if (Serial.readStringUntil(";") == "1;") {
    digitalWrite(Green, HIGH); delay(250); digitalWrite(Green, LOW);
  } else {
    digitalWrite(Red, HIGH); delay(250); digitalWrite(Red, LOW);
  } 
}

void loop()
{
  while (Serial.available()) Serial.read();
  while (! Serial.available());

  int id = readnumber();
  if (getFingerprintEnroll(id) != 1) {    //Scanner exited with an error, immediate fail
    finger.deleteModel(id);
    digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW); delay(100); digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);
  } else {                                          //Scanner exited without error, conditionall success
    while (! Serial.available());
    if (Serial.readStringUntil(";") == "1;") {
      digitalWrite(Green, HIGH); delay(100); digitalWrite(Green, LOW); delay(100); digitalWrite(Green, HIGH); delay(100); digitalWrite(Green, LOW);
    } else {
      finger.deleteModel(id);
      digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW); delay(100); digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);
    }
  }
}

int getFingerprintEnroll(int id) {

  int p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p != FINGERPRINT_NOFINGER) {
      Serial.print(p);
      Serial.print(";");
      if (p != FINGERPRINT_OK) {
        return 0;
      }
    }
  }
  p = finger.image2Tz(1);
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }
  p = confirmUnique();
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }

  //Fingerprint #1 is good!
  
  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("0;");
  
  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p != FINGERPRINT_NOFINGER) {
      Serial.print(p);
      Serial.print(";");
      if (p != FINGERPRINT_OK) {
        return 0;
      }
    }
  }
  p = finger.image2Tz(2);
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }
  p = confirmUnique();
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }
  p=finger.createModel();
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }
  p=finger.storeModel(id);
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }

  //Fingerprint #2 is good

  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.print("0;");

  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    if (p != FINGERPRINT_NOFINGER) {
      Serial.print(p);
      Serial.print(";");
      if (p != FINGERPRINT_OK) {
        return 0;
      }
    }
  }
  p = finger.image2Tz(1);
  Serial.print(p);
  Serial.print(";");
  if (p != 0) {
    return 0;
  }
  p = finger.fingerFastSearch();
  if (finger.fingerID == id) {  //All is good
    Serial.print("0;");
  } else if (p == 0) {          //Already enrolled somewhere else
    Serial.print("8;");
    return 0;
  } else {                      //No matching print found, prints don't match
    Serial.print("10;");
    return 0;
  }

  //Fingerprint #3 is good
  //If it gets here all has gone smoothly
  return 1;
}

int readnumber(void) {
  int num = -1;
  
  while (num == -1) {
    while (! Serial.available());
    num = Serial.parseInt();
  }
  return num;
}

int confirmUnique() {
  int q = finger.fingerFastSearch();
  if (q == FINGERPRINT_OK) {
    return 8;   //Not unique, fail
  } else {
    return 0;   //Unique, pass
  }
}
