#include <ALKS.h>
#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "Vodafone-523D";
const char *password = "vDvctBYckkdg4h8T";

WiFiServer server(80); // Změna třídy na WiFiServer

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);  // set the LED pin mode

  delay(10);

  // Připojení k WiFi síti
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin(); // Spuštění serveru
}

void loop() {
  WiFiClient client = server.available(); // Naslouchání pro příchozí klienty

  if (client) {                     // Pokud se připojí klient
    Serial.println("New Client.");  // Vypiš zprávu na sériový port
    String currentLine = "";        // String pro uchování dat od klienta
    while (client.connected()) {    // Dokud je klient připojený
      if (client.available()) {     // Pokud jsou dostupná data
        char c = client.read();     // Přečti jeden byte
        Serial.write(c);            // Vypiš na sériový port
        if (c == '\n') {            // Pokud je to znak nového řádku

          // Pokud je aktuální řádek prázdný, odesíláme odpověď:
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') { 
          currentLine += c;
        }

        // Kontrola požadavků "GET /H" nebo "GET /L"
        if (currentLine.endsWith("GET /H")) {
          digitalWrite(5, HIGH);  // Zapnutí LED
        }
        if (currentLine.endsWith("GET /L")) {
          digitalWrite(5, LOW);   // Vypnutí LED
        }
      }
    }
    client.stop(); // Zavření připojení
    Serial.println("Client Disconnected.");
  }
}
