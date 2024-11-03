#include <PS4Controller.h>
#include <Arduino.h>
#include <Servo.h>
#include <ALKS.h>

#define TRIG_PIN A4 // Trig
#define ECHO_PIN A5 // Echo
float duration_us, distance_cm;

Servo servo1;
Servo servo2;
Servo servo3;
int ls;
int ps;
bool L2;
bool P2;
int fuu= 90;
void setup() {
  Serial.begin(115200);
  setupAll();
  servo1.attach(S3);
  servo2.attach(S2);
  servo3.attach(S1);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  servo3.write(fuu);
  PS4.begin("1a:2b:3c:01:01:01");
  Serial.println("Ready.");
}

float measureDistance() {
    // generate 10-microsecond pulse to TRIG pin
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // measure duration of pulse from ECHO pin
    duration_us = pulseIn(ECHO_PIN, HIGH);

    // calculate the distance
    distance_cm = 0.017 * duration_us;

    // print the value to Serial Monitor
    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    delay(500);

    return distance_cm;
}
void read() {
  // Below has all accessible outputs from the controller
  if (PS4.isConnected()) {
    if (PS4.Right()){ 
      Serial.println("Right Button");
      //prava=true;
    }
    if (PS4.Down()) Serial.println("Down Button");
    if (PS4.Up()){ 
      Serial.println("Up Button");
      //horni=true;
    }
    if (PS4.Left()){ 
      Serial.println("Left Button");
      //leva=true;
    }

    if (PS4.Square()) Serial.println("Square Button");
    if (PS4.Cross()) Serial.println("Cross Button");
    if (PS4.Circle()) Serial.println("Circle Button");
    if (PS4.Triangle()){
      Serial.println("Triangle Button");
      fuu=90;
      servo3.write(fuu);
    } 

    if (PS4.UpRight()) Serial.println("Up Right");
    if (PS4.DownRight()) Serial.println("Down Right");
    if (PS4.UpLeft()) Serial.println("Up Left");
    if (PS4.DownLeft()) Serial.println("Down Left");

    if (PS4.L1()) Serial.println("L1 Button");
    if (PS4.R1()) Serial.println("R1 Button");

    if (PS4.Share()) Serial.println("Share Button");
    if (PS4.Options()) Serial.println("Options Button");
    if (PS4.L3()) Serial.println("L3 Button");
    if (PS4.R3()) Serial.println("R3 Button");

    if (PS4.PSButton()) Serial.println("PS Button");
    if (PS4.Touchpad()) Serial.println("Touch Pad Button");

    if (PS4.L2()) {
      Serial.printf("L2 button at %d\n", PS4.L2Value());
      L2= true;
    }
    else{
     
    }
    if (PS4.R2()) {
      Serial.printf("R2 button at %d\n", PS4.R2Value());
      P2= true;
    }
    else{
      
    }

    if (PS4.LStickX()) {
      Serial.printf("Left Stick x at %d\n", PS4.LStickX());
    }
    if (PS4.LStickY()) {
      Serial.printf("Left Stick y at %d\n", PS4.LStickY());
      ls= PS4.LStickY();
    }
    if (PS4.RStickX()) {
      Serial.printf("Right Stick x at %d\n", PS4.RStickX());
    }
    if (PS4.RStickY()) {
      Serial.printf("Right Stick y at %d\n", PS4.RStickY());
     ps= PS4.RStickY();
    }

    if (PS4.Charging()) Serial.println("The controller is charging");
    if (PS4.Audio()) Serial.println("The controller has headphones attached");
    if (PS4.Mic()) Serial.println("The controller has a mic attached");

    Serial.printf("Battery Level : %d\n", PS4.Battery());

    Serial.println();
    // This delay is to make the output more human readable
    // Remove it when you're not trying to see the output
    delay(30);
  }
}
void serva(){
  if(ls > 7){
    int rychlost= map(ls, 8, 127, 91, 179);
    servo1.write(rychlost); 
  }
  else if(ls <= -7){
    int rychlost= map(ls, -7, -127, 89, 0);
    servo1.write(rychlost); 
  }
  else{
    servo1.write(90);
  }
  /////////////////////////////////////////////////
  if(ps > 7){
    int rychlost= map(ps, 8, 127, 91, 179);
    servo2.write(rychlost); 
  }
  else if(ps <= -7){
    int rychlost= map(ps, -7, -127, 89, 0);
    servo2.write(rychlost); 
  }
  else{
    servo2.write(90);
  }
  /////////////////////////////////////////
if((L2==true) && (fuu > 7)) {
    fuu = ( fuu + map(PS4.L2Value(),0 ,255, 0, 5));
    servo3.write(fuu);
    delay(20);  // krátké zpoždění
    L2=false;
}
if((P2==true) && (fuu < 173)) {
    fuu = ( fuu- map(PS4.R2Value(),0 ,255, 0, 5));
    servo3.write(fuu);
    delay(20);  // krátké zpoždění
    P2= false;
}

}
void loop(){
  read();
  // Měření vzdálenosti a vibrace
  /*if(measureDistance() < 20){
    if (PS4.isConnected()) {
      PS4.setRumble(200, 200); 
      delay(100);
      PS4.setRumble(0, 0); // Vypnout vibrace po určité době
    }
  }*/
  serva();
  Serial.print("fuu: ");
  Serial.print(fuu);
  delay(10);
   Serial.print("ps: "); 
  Serial.print(ps);
}
