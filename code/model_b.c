#include <stdio.h>
#include <math.h>
#include "model_b.h"
#include "proc.h"

void calc_breath_ticks(unsigned int bpm, ie_t ie, unsigned int *inspire, unsigned int *expire) {
  unsigned int breath_ticks = TICKS_PER_MIN / bpm; 
  *inspire = breath_ticks / ((1<<IE_FRACBITS)+ie) * (1<<IE_FRACBITS);
  *expire = breath_ticks - *inspire;
  return;
}

vent_config init_config(vent_config config) {
  config.mode = VOL;
  config.tidal_vol = DEFAULT_VOLUME;
  config.control_pressure = DEFAULT_PRESS;
  config.peep = DEFAULT_PEEP;
  config.ie = DEFAULT_IE;
  config.bpm = DEFAULT_BPM;
  config.end = 0;
  return config;
}

int breath(func_table_t func_table, vent_config config) {
  unsigned int inspire_ticks, expire_ticks;
  vol_t expired_vol;
  press_t pressure;

  calc_breath_ticks(config.bpm, config.ie, &inspire_ticks, &expire_ticks);
  // Implement a mode-appropriate "breath"
  switch (config.mode) {
    case VOL:
      pressure = func_table.volume_inspire(inspire_ticks, config);
      expired_vol = func_table.volume_expire(expire_ticks, config);
      break;
    case PRESS:
      pressure = func_table.pressure_inspire(inspire_ticks, config);
      expired_vol = func_table.pressure_expire(expire_ticks, config);
      break;
    default :
      printf("ERROR, should never get here!\n");
  }
  return 0;
}

//______________________________________________________________________________
// Super-Loop

int sup_loop(func_table_t func_table, vent_config config) {
  while (!config.end) {
    config = func_table.update_config(config);
    breath(func_table, config);
    // Update user display with measured values
    func_table.update_display(config);
  }
}

//
// vent_main
int vent_main(func_table_t func_table, vent_config config) {
  config = func_table.init_config(config);
  sup_loop(func_table, config);
}

