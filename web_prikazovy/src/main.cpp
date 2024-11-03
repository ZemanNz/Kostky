#include <Arduino.h>
#include <ALKS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Servo.h>
static const int servoPin = SERVO4;

Servo servo1;


/////////////////////////////////////
// Připojení k Wi-Fi
const char* ssid = "zemcom";
const char* password = "radekzeman";

// Nastavení pinů pro ultrazvukový senzor
#define TRIG_PIN A4 // Trig
#define ECHO_PIN A5 // Echo// LED dioda na pinu 2
float duration_us, distance_cm;
WebServer server(80);

// Funkce pro měření vzdálenosti
float measureDistance() {
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    duration_us = pulseIn(ECHO_PIN, HIGH);
    distance_cm = 0.017 * duration_us;

    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    delay(500);
    return distance_cm;
}

// Funkce pro rozsvícení LED na určitou dobu
void svit(int sec) {
    digitalWrite(L_R, HIGH);
    delay(sec * 1000);
    digitalWrite(L_R, LOW);
}
void serv(int opak) {
  for(int i=0; i<opak; i++){
    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        servo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    } 
  }
}
void mere(int pocet){
  for(int i =0; i<pocet; i++){
    int sensorValue = analogReadRaw(A3); // Čtení hodnoty ze snímače
    Serial.println("sensorValue:"); 
    Serial.println(sensorValue);
    delay(300); // Malé zpoždění pro čtení hodnoty každých 200ms
  }
}

// Obsluha webové stránky
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>ESP32 Prikazovy rádek</h1>";
  html += "<p>Zadejte prikaz a kliknete na 'Poslat'.</p>";
  html += "<input type='text' id='command' placeholder='Zadejte prikaz...'><button onclick='sendCommand()'>Poslat</button>";
  
  html += "<script>";
  html += "function sendCommand() {";
  html += "  const cmd = document.getElementById('command').value;";
  html += "  fetch('/run?cmd=' + encodeURIComponent(cmd));";
  html += "}";
  html += "</script>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Obsluha příkazů z webu
void handleRun() {
  String cmd = server.arg("cmd");
  Serial.println("Prijaty prikaz: " + cmd);

  // Rozpoznání příkazu a spuštění odpovídající funkce
  if (cmd.startsWith("svit(") && cmd.endsWith(")")) {
    int sec = cmd.substring(5, cmd.length() - 1).toInt();
    if (sec > 0) {
      svit(sec);
      server.send(200, "text/plain", "Rozsveceni LED na " + String(sec) + " sekund.");
      return;
    }
  }
  if (cmd.startsWith("mere(") && cmd.endsWith(")")) {
    int kolik = cmd.substring(5, cmd.length() - 1).toInt();
    if (kolik > 0) {
      mere(kolik);
      server.send(200, "text/plain", "mereni " + String(kolik) + " sekund.");
      return;
    }
  }
  if (cmd.startsWith("serv(") && cmd.endsWith(")")) {
   int opakov = cmd.substring(5, cmd.length() - 1).toInt();
    if (opakov > 0) {
      serv(opakov);
      server.send(200, "text/plain", "servo " + String(opakov) + " sekund.");
      return;
    }
  }
  
  server.send(400, "text/plain", "Neplatny prikaz.");
}

void setup() {
  Serial.begin(9600);
  setupAll();
  servo1.attach(servoPin);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(L_R, OUTPUT);
  digitalWrite(L_R, LOW);
   // pinMode(A3, INPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start webového serveru
  server.on("/", handleRoot);
  server.on("/run", handleRun);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  server.handleClient();
}
