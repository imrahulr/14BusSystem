#include <SoftwareSerial.h>

SoftwareSerial ESPserial(2, 3); // RX | TX

long lastMsg = 0; 
String msg_recv = "";
String msg_send = "";
int status_curr[20] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int status_temp[20];

void updateStatus() {
  
  for (int i=0; i<20; i++) {
    if (status_temp[i] >= 0 && status_temp[i] <=2) { 
      if (status_temp[i] != status_curr[i]) {
        status_curr[i] = status_temp[i];
      }
    }
  }
  
}

void getStatus() {
  
  // listen for status from the ESP8266
  if ( ESPserial.available() )   {
    msg_recv = "";
    msg_recv = ESPserial.readStringUntil('\n');
    Serial.print("msg recv :");
    Serial.println(msg_recv);
  }  
  if (msg_recv != "") {
    for (int i=1; i<60; i=i+3) {
       status_temp[i/3] = msg_recv[i]-48;  
    }
    updateStatus();
    msg_recv = "";
  }
   
}

void sendStatus() {
  
  // send status to ESP8266
  for (int j=0; j<20; j++) {
      if (j == 0) msg_send += "[";
      msg_send += status_curr[j];
      if (j == 19) msg_send += "]";
      else msg_send += ", ";
  }
  Serial.print("Msg sent :");
  Serial.println(msg_send);
  ESPserial.write(msg_send.c_str());
  delay(1000);
  msg_send = "";  

}

void setup() {
  
  Serial.begin(9600);
  ESPserial.begin(115200);   
  Serial.println("Ready");
  
}


void loop() {
  
  getStatus();
  long now = millis();
  if (now - lastMsg > 5000) {
    lastMsg = now;
    sendStatus(); 
  }
  
}

