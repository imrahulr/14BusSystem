#include <ShiftRegister74HC595.h>

// ShiftRegister74HC595 sr (numberOfShiftRegisters, serialDataPin, clockPin, latchPin); 
// sr.set(pin, HIGH); --> set single pin HIGH
// sr.setAllHigh();   --> set all pins HIGH
// sr.setAllLow();    --> set all pins LOW
// uint8_t pinValues[] = { B10101010 }; sr.setAll(pinValues);  --> set all pins at once 
// uint8_t stateOfPin5 = sr.get(5);  --> read pin(zero based)
    
ShiftRegister74HC595 sr (3, 4, 2, 3);

void setup() {
}

void loop() {
  sr.setAllLow(); // set all pins LOW
  delay(2000);
  
  for (int i = 0; i < 24; i++) {
    sr.set(i, HIGH);
    delay(1000);    
  }

  for (int i = 0; i < 24; i++) {
    sr.set(i, LOW);
    delay(1000);    
  }
}
