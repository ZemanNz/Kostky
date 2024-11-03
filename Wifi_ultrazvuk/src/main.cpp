#include <Arduino.h>
#include <WiFi.h>
#include <ALKS.h>

// Definice LED pinů
#define GREEN_LED 5
#define RED_LED 18
#define YELLOW_LED 19
#define BLUE_LED 21

// Definice Wi-Fi přihlašovacích údajů
const char *ssid = "Vodafone-523D";
const char *password = "vDvctBYckkdg4h8T";
float duration_us, distance_cm;
WiFiServer server(80);

// Funkce pro "Vánoční" blikání LEDek
void startChristmasLights() {
  // Tady bude kód pro blikání různými LEDkami
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

void stopChristmasLights() {
  // Tady bude kód pro zastavení blikání
}

// Funkce pro měření vzdálenosti
void measureDistance(WiFiClient client) {
  // Tady bude kód pro měření vzdálenosti pomocí ultrazvukového senzoru
  // Placeholder - tady by byla skutečná hodnota
    // generate 10-microsecond pulse to TRIG pin
  digitalWrite(A4, HIGH);
  delayMicroseconds(10);
  digitalWrite(A4, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(A5, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  delay(300);
  client.print("Vzdálenost je: ");
  client.print(distance_cm);
  client.println(" cm");
}

void setup() {
  Serial.begin(115200);
  pinMode(A4, OUTPUT);
  // configure the echo pin to input mode
  pinMode(A5, INPUT);
  // Nastavení pinů pro LEDky
  pinMode(L_R, OUTPUT);
    pinMode(L_Y, OUTPUT);
    pinMode(L_G, OUTPUT);
    pinMode(L_B, OUTPUT); // pinMode(L_G, OUTPUT); zelená v RGB ledce
    setupAll();
  // Připojení k Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            // Hlavní HTML stránka s tlačítky
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<button onclick=\"location.href='/LED_ON'\">Zapnout LED Vánoce</button>");
            client.println("<button onclick=\"location.href='/LED_OFF'\">Vypnout LED Vánoce</button>");
            client.println("<button onclick=\"location.href='/DISTANCE'\">Měřit vzdálenost</button>");
            client.println("</html>");
            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }

        // Zpracování příkazů z tlačítek
        if (currentLine.endsWith("GET /LED_ON")) {
          startChristmasLights();  // Zapne blikání LEDek
        }
        if (currentLine.endsWith("GET /LED_OFF")) {
          stopChristmasLights();   // Vypne blikání LEDek
        }
        if (currentLine.endsWith("GET /DISTANCE")) {
          measureDistance(client); // Změří vzdálenost a zobrazí ji na webu
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }
}

