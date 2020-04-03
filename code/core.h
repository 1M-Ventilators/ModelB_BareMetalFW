#ifndef CORE
#define CORE

#include "vent_config.h"


typedef press_t (*volume_inspire_t)(unsigned int ticks, vent_config config);
typedef vol_t (*volume_expire_t)(unsigned int ticks, vent_config config);
typedef press_t (*pressure_inspire_t)(unsigned int ticks, vent_config config);
typedef vol_t (*pressure_expire_t)(unsigned int ticks, vent_config config);

press_t volume_inspire(unsigned int ticks, vent_config config);
vol_t volume_expire(unsigned int ticks, vent_config config);
press_t pressure_inspire(unsigned int ticks, vent_config config);
vol_t pressure_expire(unsigned int ticks, vent_config config);

#endif // CORE
