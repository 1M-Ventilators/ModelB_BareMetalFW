#ifndef UI
#define UI

#include "vent_config.h"
#include "core.h"

typedef vent_config (*update_config_t)(vent_config);
typedef void (*update_display_t)(vent_config config);


vent_config update_config(vent_config config);
void update_display(vent_config config);


#endif // UI
