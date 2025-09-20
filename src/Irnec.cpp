#include "Arduino.h"
#include "Irnec.h"

Irnec* Irnec::sIrnec = 0;

void Irnec::int_handle()
{
  if(sIrnec != 0){
    sIrnec->irq_handle();
  }
}

void Irnec::irq_handle()
{
  micros_now = micros();
  diff = micros_now - micros_last;
  micros_last = micros_now;

  noInterrupts();
  if(diff >= 14000){
    // none
  }else if(diff >= 13000){
    // Start
    index = 0;
    bitpattern = 0;
  }else if( diff >= 2000){
    // bit 1
    bitpattern |= (uint32_t)(1UL << (31-index));
    index += 1;
  }else{
    // bit 0
    index += 1;
  }
  interrupts();

  if(index >= 32){
    code = bitpattern;
    code_ready = true;
  }
}

Irnec::Irnec(int pin)
{
  sIrnec = this;
  this_pin = pin;
  code_ready = false;
  index = 0;
  micros_now = 0;
  micros_last = 0;
}

void Irnec::begin()
{
  pinMode(this_pin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(this_pin), Irnec::int_handle, FALLING);
}

boolean Irnec::decode()
{
  return code_ready;
}

uint32_t Irnec::get_code()
{
  return code_reverse(0xFF&(code>>8UL));
}

char Irnec::get_number()
{
  char output;
  uint32_t code = get_code();
  switch (code) {
    case 0x19: output = '0';break;
    case 0x45: output = '1';break;
    case 0x46: output = '2';break;
    case 0x47: output = '3';break;
    case 0x44: output = '4';break;
    case 0x40: output = '5';break;
    case 0x43: output = '6';break;
    case 0x07: output = '7';break;
    case 0x15: output = '8';break;
    case 0x09: output = '9';break;
    case 0x16: output = '*';break;
    case 0x0D: output = '#';break;
    case 0x1C: output = 'k';break;
    case 0x18: output = 'u';break;
    case 0x08: output = 'l';break;
    case 0x5A: output = 'r';break;
    case 0x52: output = 'd';break;
    default: output = code; break;
  }
  return output;
}

uint8_t Irnec::code_reverse(uint8_t code)
{
  int ind;
  uint8_t bit;
  uint8_t output = 0;;
  for(ind = 0; ind < 8; ind++){
    bit = code & (1UL<<ind);
    if(bit != 0){
      output |= 1UL<<(7-ind);
    }
  }
  return output;
}

void Irnec::resume()
{
  index = 0;
  bitpattern = 0;
  code_ready = false;
}
