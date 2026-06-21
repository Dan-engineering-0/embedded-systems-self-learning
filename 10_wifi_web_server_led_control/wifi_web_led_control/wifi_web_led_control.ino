#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2

const char* apName = "ESP32_LAB";
const char* apPassword = "12345678";

WebServer server(80);

bool blinkMode = false;
bool ledState = false;

unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 500;

String htmlPage() {
  String page = "";

  page += "<!DOCTYPE html>";
  page += "<html>";
  page += "<head>";
  page += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  page += "<title>ESP32 Lab</title>";

  page += "<style>";
  page += "body { font-family: Arial; text-align: center; margin-top: 40px; }";
  page += "button { font-size: 24px; padding: 15px 30px; margin: 10px; }";
  page += ".on { background-color: #4CAF50; color: white; }";
  page += ".off { background-color: #f44336; color: white; }";
  page += ".blink { background-color: #2196F3; color: white; }";
  page += "</style>";

  page += "</head>";
  page += "<body>";
  page += "<h1>ESP32 Web Control</h1>";
  page += "<p>Control the ESP32 built-in LED from your phone.</p>";

  page += "<a href='/on'><button class='on'>LED ON</button></a><br>";
  page += "<a href='/off'><button class='off'>LED OFF</button></a><br>";
  page += "<a href='/blink'><button class='blink'>BLINK</button></a><br>";

  page += "<p>Status: ";
  if (blinkMode) {
    page += "BLINKING";
  } else if (ledState) {
    page += "ON";
  } else {
    page += "OFF";
  }
  page += "</p>";

  page += "</body>";
  page += "</html>";

  return page;
}

void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleOn() {
  blinkMode = false;
  ledState = true;
  digitalWrite(LED_PIN, HIGH);

  Serial.println("Web command: LED ON");
  server.send(200, "text/html", htmlPage());
}

void handleOff() {
  blinkMode = false;
  ledState = false;
  digitalWrite(LED_PIN, LOW);

  Serial.println("Web command: LED OFF");
  server.send(200, "text/html", htmlPage());
}

void handleBlink() {
  blinkMode = true;

  Serial.println("Web command: BLINK");
  server.send(200, "text/html", htmlPage());
}

void setup() {
  pinMode(LED_PIN, OUTPUT);

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("Starting ESP32 Access Point...");

  WiFi.softAP(apName, apPassword);

  IPAddress ip = WiFi.softAPIP();

  Serial.print("WiFi network name: ");
  Serial.println(apName);

  Serial.print("WiFi password: ");
  Serial.println(apPassword);

  Serial.print("Open this address in phone browser: ");
  Serial.println(ip);

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.on("/blink", handleBlink);

  server.begin();

  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  if (blinkMode) {
    unsigned long currentTime = millis();

    if (currentTime - lastBlinkTime >= blinkInterval) {
      lastBlinkTime = currentTime;

      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
    }
  }
}
