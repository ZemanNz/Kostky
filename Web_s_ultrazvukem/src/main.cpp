#include <Arduino.h>
#include <ALKS.h>
#include <WiFi.h>
#include <WebServer.h>

// Připojení k Wi-Fi
const char* ssid = "zemcom";
const char* password = "radekzeman";

// Nastavení pinů pro ultrazvukový senzor
#define TRIG_PIN A4 // Trig
#define ECHO_PIN A5 // Echo
float duration_us, distance_cm;
WebServer server(80);

// Funkce pro měření vzdálenosti
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

// Obsluha webové stránky
void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Mereni vzdalenosti</h1>";
  html += "<p>Kliknete na 'Zmer' pro pridani nove hodnoty do tabulky.</p>";
  html += "<button style='background-color: green; color: white;' onclick=\"measure()\">Zmer</button>";
  html += "<table border='1' id='distanceTable'>";
  html += "<tr><th>Čas</th><th>Vzdalenost (cm)</th></tr>";
  html += "</table>";
  
  // JavaScript pro dynamické přidávání hodnot do tabulky
  html += "<script>";
  html += "function measure() {";
  html += "  fetch('/measure').then(response => response.json()).then(data => {";
  html += "    const table = document.getElementById('distanceTable');";
  html += "    const row = table.insertRow(-1);";
  html += "    const timeCell = row.insertCell(0);";
  html += "    const distanceCell = row.insertCell(1);";
  html += "    timeCell.innerHTML = new Date().toLocaleTimeString();";
  html += "    distanceCell.innerHTML = data.distance + ' cm';";
  html += "  });";
  html += "}";
  html += "</script>";

  html += "</body></html>";
  server.send(200, "text/html", html);
}

// Obsluha měření vzdálenosti
void handleMeasure() {
  float distance = measureDistance();
  String json = "{\"distance\": " + String(distance) + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(9600);
  setupAll();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Připojení k Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");

  // **Přidáno: Výpis IP adresy**
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start webového serveru
  server.on("/", handleRoot);
  server.on("/measure", handleMeasure);
  server.begin();
  Serial.println("Server started");
}

void loop() {
  // Obsluha serveru
  server.handleClient();
}
