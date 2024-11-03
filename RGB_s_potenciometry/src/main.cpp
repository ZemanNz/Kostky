#include <Arduino.h>
#include <ALKS.h>
int POT = 0; // Proměnná pro ukládání hodnoty potenciometru
int POT255 = 0; // Proměnná pro převod na rozsah 0-255
int POTT2=0;
int POT2255=0;
int stav=1;
void setup() {
    pinMode(L_R, OUTPUT);
    pinMode(L_Y, OUTPUT);
    pinMode(L_G, OUTPUT);
    pinMode(L_B, OUTPUT); // pinMode(L_G, OUTPUT); zelená v RGB ledce
    setupAll();
    Serial.begin(115200);
}
void loop(){
  POT = analogRead(POT1); // Čtení hodnoty potenciometru (0-1023)
  POT255 = map(POT, 0, 1023, 0, 255); // Převod na rozsah 0-255
  Serial.print("  |  Převod: ");
  Serial.println(POT255); // Vypíše převedenou hodnotu (0-255)
  ///////////////////
  POTT2 = analogRead(POT2); // Čtení hodnoty potenciometru (0-1023)
  POT2255 = map(POTT2, 0, 1023, 0, 255); // Převod na rozsah 0-255
  Serial.print("  |  Převod: ");
  Serial.println(POT2255); // Vypíše převedenou hodnotu (0-255)
  ////////////////
  delay(100); // Zpoždění pro lepší čitelnost
  stav = digitalRead(SW1);
  if(stav == 0){
      digitalWrite(L_R, HIGH);
      delay (1000);
      digitalWrite(L_R, LOW);
      digitalWrite(L_Y, HIGH);
      delay (1000);
      digitalWrite(L_Y, LOW);
      digitalWrite(L_G, HIGH);
      delay (1000);
      digitalWrite(L_G, LOW);
      digitalWrite(L_B, HIGH);
      delay (1000);
      digitalWrite(L_B, LOW);

    }

  analogWrite(L_RGB_R, POT255);
  analogWrite(L_RGB_G, 0);
  analogWrite(L_RGB_B, POT2255);
  delay(100);
}