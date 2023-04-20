#include <Servo.h>

#define enA 11  // PWM pin 10 za L298N
#define in1 6  // pin 12 za L298N
#define in2 7  // pin 13 za L298N
#define servoPin 5

int xOs, yOs; // definiranje Osi virtualnog joysticka
int  x = 0; // definiranje varijable x koja prima vrijednost preko aplikacije za OS
int  y = 0; // definiranje varijable y koja prima vrijednost preko aplikacije za OS
Servo steeringServo;
int motorA = 0; // kontrola DC motora za pogon, početna vrijednost "0"
int motorB = 0; // kontrola DC motora za skretanje, početna vrijednost "0"
int LED = 4;

void setup() {
  pinMode(enA, OUTPUT); // postavljne enA kao izlaz "OUTPUT"
  pinMode(in1, OUTPUT); // postavljne in1 kao izlaz "OUTPUT"
  pinMode(LED, OUTPUT);
  steeringServo.attach(servoPin);
  Serial.begin(9600); // Brzina komunikacije Bluetooth modula
}

void loop() {
  // Postavljanje početnih vrijednosti
  xOs = 510;
  yOs = 510;

  // Očitavanje vrijednosgi poslanih od strane mobilne aplikacije
  while (Serial.available() >= 2) {
    x = Serial.read();
    delay(10);
    y = Serial.read();
  }
  delay(10);
 
  // Ograničavanje vrijednosti "x" i "y" varijable koje očitava i šalje mobilna aplikacija
  if (x > 59 & x < 251) {
    xOs = map(x, 250, 60, 1023, 0); // Postavljanje "x" i "y" vrijednosti u rangu 1023 - 0
  }
  if (y > 59 & y < 251) {
    yOs = map(y, 250, 60, 0, 1023);
  }

 // Y-os koja kontrolira kretnju automobila nazad/naprijed
  if (yOs < 460) {
    // Postavljanje smjera automobila prema nazad
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    // Pretvaranje vrijednosti Y-os u granicama iz 0 - 470 u 0 - 160 za PWM signal koji kontrolira brzinu okretanja motora
    motorA = map(yOs, 470, 0, 0, 255);
   
  }
  else if (yOs > 560) {
    // Postavljanje smjera automobila prema naprijed
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
  
    // Pretvaranje vrijednosti Y-os u granicama iz 550 - 1023 u 0 - 160 za PWM signal koji kontrolira brzinu okretanja motora
    motorA = map(yOs, 550, 1023, 0, 255);
  
  }
  // Ako je Y-os oko svoje sredine, motor miruje
   else {
    digitalWrite(in1, HIGH);
    digitalWrite(in2, HIGH);
  }

 // X-os koja kontrolira kretnju automobila lijevo/desno
  if (xOs < 490) {
   int xMapped = map(xOs, 490, 0, 0, 33);
    steeringServo.write(87 - xMapped);
  }
  else if (xOs > 534) {
     int xMapped = map(xOs, 534, 1023, 0, 33);
    steeringServo.write(87 + xMapped);
  }
  // Ako je X-os oko svoje sredine, motor miruje
 else {
    steeringServo.write(87);
  }

 // Na niskom naponu elektromotor se ne vrti, ali bespotrebno zuji, ovo je zaštita od zujenja
  if (motorA < 15) {
    motorA = 0;
  }
  if (motorB < 50) {
    motorB = 0;
  }
    
  analogWrite(enA, motorA); // Prosljeđivanje PWM signala prema motoru A

  if (x == 0) {
  digitalWrite(LED, LOW);
  }
   
   if (x == 1) {
  digitalWrite(LED, HIGH);
  }
}
