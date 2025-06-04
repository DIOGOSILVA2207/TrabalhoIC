#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// === Configurações WiFi e MQTT ===
const char* mqtt_server = "broker.emqx.io";
const char* ssid = "Wokwi-GUEST";
const char* password = "";

WiFiClient espClient;
PubSubClient client(espClient);

// === Sensores ===
#define DHTPIN1 13  // Sensor de ar
#define DHTPIN2 2   // Sensor de solo
#define DHTTYPE DHT22
#define LDR_PIN 34  // Pino analógico do LDR

DHT dhtAr(DHTPIN1, DHTTYPE);
DHT dhtSolo(DHTPIN2, DHTTYPE);

// === Controle de tempo ===
long lastTime = 0;
const long interval = 10000; // 10 segundos

void setup_wifi() {
  Serial.println("Starting Setup Wifi");
  delay(10);
  Serial.print("Connecting to Wifi ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected");
}

void callback(String topic, byte* payload, unsigned int length) {
  String strPayload;
  for (int i = 0; i < length; i++) {
    strPayload += (char)payload[i];
  }

  JsonDocument doc;
  if (deserializeJson(doc, strPayload)) return;

  if (topic == "IPB/IoT/plantaBOT/DHT22") {
    Serial.println("Mensagem recebida no tópico: " + topic);
  }
}

void reconnect() {
  while (!client.connected()) {
    String clientId = "AulaIoT-" + String(random(0xffff), TX);
    if (client.connect(clientId.c_str())) {
      client.subscribe("IPB/IoT/plantaBOT/DHT22");
    } else {
      delay(5000);
    }
  }
}

String classificarLuminosidade(float percent) {
  if (percent > 66.0) return "alta";
  else if (percent >= 33.0) return "média";
  else return "baixa";
}

String avaliarEstadoPlanta(float temp, float humAr, float humSolo, float percentLuz) {
  int score = 0;

  if (temp >= 15 && temp <= 30) score++;
  if (humAr >= 40 && humAr <= 70) score++;
  if (humSolo >= 40 && humSolo <= 70) score++;
  if (percentLuz >= 33.0 && percentLuz <= 66.0) score++;

  if (score == 4) return "saudável";
  else if (score >= 2) return "perigo";
  else return "crítico";
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  dhtAr.begin();
  dhtSolo.begin();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) WiFi.reconnect();
  if (!client.connected()) reconnect();
  client.loop();

  long now = millis();
  if (now - lastTime > interval) {
    lastTime = now;

    float tempAr = dhtAr.readTemperature();
    float humidadeAr = dhtAr.readHumidity();
    float humidadeSolo = dhtSolo.readHumidity();

    int analogValue = analogRead(LDR_PIN);
    float percentLuminosidade = (1.0 - (analogValue / 4095.0)) * 100.0;

    Serial.print("Valor analógico LDR: ");
    Serial.println(analogValue);
    Serial.print("Luminosidade (%): ");
    Serial.println(percentLuminosidade, 1);

    if (isnan(tempAr) || isnan(humidadeAr) || isnan(humidadeSolo)) {
      Serial.println("Erro ao ler dos sensores DHT!");
      return;
    }

    String textoLuminosidade = classificarLuminosidade(percentLuminosidade);
    String estadoPlanta = avaliarEstadoPlanta(tempAr, humidadeAr, humidadeSolo, percentLuminosidade);

    StaticJsonDocument<256> doc;
    doc["temperatura"] = tempAr;
    doc["humidade"] = humidadeAr;
    doc["humidade_solo"] = humidadeSolo;
    doc["luminosidade"] = String(percentLuminosidade, 1) + "%";
    doc["nivel_luminosidade"] = textoLuminosidade;
    doc["estado_planta"] = estadoPlanta;

    char buffer[256];
    serializeJson(doc, buffer);

    client.publish("IPB/IoT/plantaBOT/DHT22", buffer);
    Serial.print("Publicado: ");
    Serial.println(buffer);
  }
}
