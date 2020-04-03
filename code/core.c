#include <stdio.h>
#include <math.h>
#include "model_b.h"
#include "core.h"
#include "proc.h"

press_t volume_inspire(unsigned int ticks, vent_config config) {
  // Call the low level code to take a volume-controlled inspiration
  press_t press;
  press = 0;
  return press;
}

vol_t volume_expire(unsigned int ticks, vent_config config) {
  // Call the low level code to take a volume-controlled expiration
  vol_t vol;
  vol = 0;
  return vol;
}

press_t pressure_inspire(unsigned int ticks, vent_config config) {
  // Call the low level code to take a volume-controlled expiration
  press_t press;
  press = 0;
  return press;
}

vol_t pressure_expire(unsigned int ticks, vent_config config) {
  // Call the low level code to take a pressure-controlled expiration
  vol_t vol;
  vol = 0;
  return vol;
}

