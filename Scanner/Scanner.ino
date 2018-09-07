#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

int Green = 8;
int White = 9;
int Red = 10;
int Button = 12;
int Mode = -1;

void setup()  
{
  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(White, OUTPUT);
  pinMode(Button, INPUT);
  
  Serial.begin(9600);
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.print("1;");
    char charBuff[10];
    while (!Serial.available());
    Serial.readStringUntil(";").substring(0,1).toCharArray(charBuff, 10);
    switch (charBuff[0]) {
      case 'E':
        digitalWrite(Green, HIGH); delay(250); digitalWrite(Green, LOW);
        ENROLL();
        break;
      case 'S':
        digitalWrite(Green, HIGH); delay(250); digitalWrite(Green, LOW);
        SIGNIN();
        break;
      case 'R':
        digitalWrite(Green, HIGH); delay(250); digitalWrite(Green, LOW);
        RESET();
        break;
      default:
        break;
    }
  } else {    //Password verify failed
    Serial.print("0;");
    digitalWrite(Red, HIGH); delay(250); digitalWrite(Red, LOW);
  }
}

//MAIN LOOPS
void ENROLL() { 
while (1){
  while (Serial.available()) Serial.read();
  while (! Serial.available());

  int id = readnumber();
  if (getFingerprintEnroll(id) != 1) {            //Scanner exited with an error, immediate fail
    finger.deleteModel(id);
    digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW); delay(150); digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);
  } else {                                        //Scanner exited without error, conditionall success
    while (! Serial.available());
    if (Serial.readStringUntil(";") == "1;") {
      digitalWrite(Green, HIGH); delay(100); digitalWrite(Green, LOW); delay(150); digitalWrite(Green, HIGH); delay(100); digitalWrite(Green, LOW);
    } else {
      finger.deleteModel(id);
      digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW); delay(150); digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);
    }
  }
}
}

void SIGNIN() {
int fingerReturn = 0;
while (1) {
  if (digitalRead(Button) == 0) {
    do {
      fingerReturn = getFingerprintIDez();
      digitalWrite(White, LOW);
      digitalWrite(Red, LOW);
      
      if (fingerReturn >= 0) {
        digitalWrite(White, HIGH);
      } else if (fingerReturn == -1) {
        digitalWrite(Red, HIGH);
      }
      
      delay(5);
    } while (fingerReturn < 0);
    
    while (! Serial.available());
    
    if (Serial.readStringUntil(";") == "1;") {
      digitalWrite(White, LOW);
      digitalWrite(Green, HIGH);
      delay(200);
      digitalWrite(Green, LOW);
    } else {
      digitalWrite(White, LOW);
      digitalWrite(Red, HIGH);
      delay(200);
      digitalWrite(Red, LOW);
    }
  }
}
}

void RESET() {
while (1) {
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
}


//FUNCTIONS
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

void loop() //Is only called if initialisation messes up
{ 
  digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);delay(150);
  digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);delay(150);
  digitalWrite(Red, HIGH); delay(100); digitalWrite(Red, LOW);
  delay(1000);
}
