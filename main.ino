#include<Servo.h> //include servo librery
#include <EEPROM.h>
Servo ServoLeft;
Servo ServoRight;
Servo Palanca;
#define Pecho 5
#define Ptrig 4
long duracion, distancia;
int sw = 0;
int sw1 = 0;

void setup() {
  // put your setup code here, to run once:
  ServoLeft.attach(12); // Servo izquierda
  ServoRight.attach(13); // Servo derecha
  Palanca.attach(11); // Servo de palanca
  pinMode(6, INPUT); //derecha
  pinMode(7, INPUT); //izquierda
  Serial.begin(9600);
  pinMode(Pecho, INPUT);     // define el pin 5 como entrada (echo)
  pinMode(Ptrig, OUTPUT);    // define el pin 4 como salida  (triger)
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorDerecho = digitalRead(6); //Leer sensor derecho
  int sensorIzquierdo = digitalRead(7); // Leer sensor izquierdo
  digitalWrite(Ptrig, LOW);
  delayMicroseconds(2);
  digitalWrite(Ptrig, HIGH);   // genera el pulso de triger por 10ms
  delayMicroseconds(10);
  digitalWrite(Ptrig, LOW);
  if (distancia != 1000000) {
    duracion = pulseIn(Pecho, HIGH);
    distancia = (duracion / 2) / 29; // calcula la distancia en centimetros
  }
  if (distancia <= 4.5) { // 4.5 --> Distancia a la cual debe bajar la palanca
    if (sw == 0) {
      Palanca.writeMicroseconds(1400);
      delay(200);                           // Mover Palanca 90º, si ya está abajo switch = 1
      Palanca.writeMicroseconds(1500);
      sw = 1;
    }
    else {
      ServoLeft.writeMicroseconds(1500); //Detenerse
      ServoRight.writeMicroseconds(1500);//
      delay(100);
      if (sw1 == 0) {
        ServoLeft.writeMicroseconds(1200); // Girar Bruscamente hacia la izquierda 180º para devolverse.
        ServoRight.writeMicroseconds(900);
        delay(1300);
        sw1 = 1;  // Switch para no repetir esta acción
      }
      distancia = 1000000; // Valor de distancia arbitrario para repetir la rutina del seguidor de linea
    }
  }
  else {
    ServoLeft.writeMicroseconds(1400); //hacia adelante
    ServoRight.writeMicroseconds(1600);//
    if (sensorDerecho == 1) { // Detecta linea
      while (sensorDerecho != 0 or sensorIzquierdo != 0) {  // Hasta que ambos sensores se encuentren en el piso
        ServoLeft.writeMicroseconds(1600); // Girar Bruscamente hacia la derecha
        ServoRight.writeMicroseconds(1800);
        delay(100);
        sensorDerecho = digitalRead(6);
        sensorIzquierdo = digitalRead(7);
      }
    }
    else if (sensorIzquierdo == 1) { // Detecta linea
      while (sensorDerecho != 0 or sensorIzquierdo != 0) {  // Hasta que ambos sensores se encuentren en el piso
        ServoLeft.writeMicroseconds(900); // Girar Bruscamente hacia la izquierda
        ServoRight.writeMicroseconds(1500);
        delay(100);
        sensorDerecho = digitalRead(6);
        sensorIzquierdo = digitalRead(7);
      }
    }
  }
}
