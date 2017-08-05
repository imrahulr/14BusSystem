#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>


const char* ssid = "";
const char* password = "";
const char* mqtt_server = "broker.hivemq.com";

WiFiClient espClient;
PubSubClient client(espClient);

String data_recv = "";
String data_send = "";


void setup_wifi() {

  delay(10);
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");
  randomSeed(micros());
  
}

void callback(char* topic, byte* payload, unsigned int length) {
  
  for (int i = 0; i < length; i++) {
    data_recv += (char)payload[i];
  }
  Serial.println(data_recv);
  data_recv = "";
  
}

void reconnect() {
  
  while (!client.connected()) {
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      //     Serial.println("connected");
      client.subscribe("bus14/pub");
    } else {
      //     Serial.print("failed, rc=");
      delay(5000);
    }
  }
  
}
   
void setup() {
  
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  if (Serial.available())   {
    data_send = Serial.readStringUntil('\n');
    if (data_send != "") {
      client.publish("bus14/data", (char *) data_send.c_str());  
      data_send = "";
    }  
  }
  
}
