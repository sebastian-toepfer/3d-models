#include "pinmap.h"
#include <Arduino.h>

// cppcheck-suppress unusedFunction
void setup()
{
  pinMode(PIN_HAUPTRELAIS, OUTPUT);
  pinMode(PIN_POOLRELAIS, OUTPUT);
  pinMode(PIN_BEWAESSERUNGSRELAIS, OUTPUT);
}

// cppcheck-suppress unusedFunction
void loop()
{
}
