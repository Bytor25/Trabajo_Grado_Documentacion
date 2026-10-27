#include <WiFi.h>

const char* WIFI_SSID = "Micelular";
const char* WIFI_PASS = "244466666";

const char* THINGSPEAK_API_KEY = "S4SAFKMAL0C6031C";

const unsigned long SEND_INTERVAL_MS = 15000;

unsigned long lastSend = 0;

float y = 0.0;
float step = 1.0;
float y_min = 0.0;
float y_max = 100.0;

void connectWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

float nextSawtooth() {
  y += step;
  if (y > y_max) {
    y = y_min;
  }
  return y;
}

bool sendToThingSpeak(float value) {
  WiFiClient client;

  Serial.println("Intentando conectar a ThingSpeak...");
  if (!client.connect("api.thingspeak.com", 80)) {
    Serial.println("Error: no se pudo conectar a ThingSpeak");
    return false;
  }

  String url = "/update?api_key=" + String(THINGSPEAK_API_KEY) +
               "&field1=" + String(value, 2);

  Serial.print("GET ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.thingspeak.com\r\n" +
               "Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (!client.available()) {
    if (millis() - timeout > 5000) {
      Serial.println("Timeout esperando respuesta del servidor");
      client.stop();
      return false;
    }
  }

  String response = "";
  while (client.available()) {
    response += client.readString();
  }
  client.stop();

  Serial.println("Respuesta completa:");
  Serial.println(response);

  int pos = response.lastIndexOf('\n');
  String body = (pos >= 0) ? response.substring(pos + 1) : response;
  body.trim();

  Serial.print("Body recibido: ");
  Serial.println(body);

  return (body.length() > 0 && body != "0");
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  connectWiFi();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi caido. Reconectando...");
    connectWiFi();
  }

  unsigned long now = millis();
  if (now - lastSend >= SEND_INTERVAL_MS) {
    lastSend = now;

    float v = nextSawtooth();
    Serial.print("Enviando field1 = ");
    Serial.println(v, 2);

    bool ok = sendToThingSpeak(v);

    if (ok) {
      Serial.println("Envio exitoso");
    } else {
      Serial.println("Envio fallo");
    }
  }
}