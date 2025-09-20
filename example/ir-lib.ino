#include "Irnec.h"

const byte ir_pin = 2;
Irnec IrReceiver(ir_pin);

void setup() {
  IrReceiver.begin();
  Serial.begin(115200);
  Serial.println("IR-lib init");
}

void loop() {
  if(IrReceiver.decode()){
    uint32_t code = IrReceiver.get_code();
    char number = IrReceiver.get_number();
    Serial.print("code: ");
    Serial.print(code, HEX);
    Serial.print(" number: ");
    Serial.println(number);
    delay(100);
    IrReceiver.resume();
  }
}
