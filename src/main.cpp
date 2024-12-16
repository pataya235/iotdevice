#include<Arduino.h>
#include<ESP8266WiFi.h>
#include<PubSubClient.h>

#define LED LED_BUILTIN
const char* ssid = "ssid";
const char* password = "password";
const char* mqtt_server = "broker_ip";
const int mqtt_port = 1883;
const char* mqtt_user = "mqtt_user";
const char* mqtt_password = "mqtt_password";
const char* mqtt_topic = "led_control";
WiFiServer server(80);
WiFiClient espClient;
PubSubClient mqttClient(espClient);

void mqttCallback(char* topic, byte* payload, int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println(message);

  if (message == "on") {
    digitalWrite(LED, HIGH);
  }
  else if (message == "off") {
    digitalWrite(LED, LOW);
  }
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      mqttClient.subscribe(mqtt_topic);
    }
    else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);                   
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    Serial.print(WiFi.status());
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();
  Serial.print("Received IP...");
  Serial.println(WiFi.localIP());

  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqttCallback);
}

void loop() {
  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
}
