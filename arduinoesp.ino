#include <SoftwareSerial.h>
#include <ShiftRegister74HC595.h>

SoftwareSerial ESPserial(2, 3); // RX | TX

// ShiftRegister74HC595 sr (numberOfShiftRegisters, serialDataPin, clockPin, latchPin); 
ShiftRegister74HC595 sr1 (3, 4, 2, 3);
ShiftRegister74HC595 sr2 (3, 7, 5, 6);


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
        if (status_curr[i] == 0) {
          sr1.set(i, HIGH);
          sr2.set(i, LOW);
          delay(500);
        } else if (status_curr[i] == 1) {
          sr1.set(i, LOW);
          sr2.set(i, LOW);  
          delay(500);    
        } else if (status_curr[i] == 2) {
          sr1.set(i, LOW);
          sr2.set(i, HIGH);
          delay(500);
        } else {
          sr1.set(i, LOW);
          sr2.set(i, LOW);
          delay(500);            
        }
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
  sr1.setAllLow();
  sr2.setAllLow();
  delay(5000);  

}


void loop() {
  
  getStatus();
  long now = millis();
  if (now - lastMsg > 20000) {
    lastMsg = now;
    sendStatus(); 
  }
  
}

