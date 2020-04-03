#ifndef MODEL_B
#define MODEL_B

#include "vent_config.h"
#include "ui.h"
#include "core.h"

typedef vent_config (*init_config_t)(vent_config);

typedef struct func_table_t {
  init_config_t init_config;
  // ui functions
  update_config_t update_config;
  update_display_t update_display;
  // core functions
  volume_inspire_t volume_inspire;
  volume_expire_t volume_expire;
  pressure_inspire_t pressure_inspire;
  pressure_expire_t pressure_expire;
} func_table_t;


vent_config init_config(vent_config config);
int vent_main(func_table_t func_table, vent_config config);

#endif // MODEL_B
