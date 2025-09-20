#include <stdint.h>
#include "Arduino.h"

class Irnec
{
  public:
    
    Irnec(int pin);
    void begin();
    boolean decode();
    uint32_t get_code();
    char get_number();
    void resume();
  private:
    unsigned long diff;
    unsigned long micros_now;
    unsigned long micros_last;
    int this_pin;
    uint32_t code;
    uint32_t bitpattern;
    int index;
    boolean code_ready;

    static Irnec* sIrnec;
    static void int_handle();
    void irq_handle();
    uint8_t code_reverse(uint8_t code);
};
