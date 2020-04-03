#ifndef VENT_CONFIG
#define VENT_CONFIG

#define VOLUME_FRACBITS 3
#define PRESS_FRACBITS 9
#define IE_FRACBITS 14

#define INC_VOLUME (50<<VOLUME_FRACBITS) // 50
#define INC_PRESS 1<<PRESS_FRACBITS // 1
#define INC_IE (( 0b0001100110011001 )>>(16-IE_FRACBITS)) // 0.1 -> 10:1

#define DEFAULT_VOLUME (8*INC_VOLUME) // 400
#define DEFAULT_PRESS (25*INC_PRESS)  // 25
#define DEFAULT_PEEP (5*INC_PRESS)    // 5
#define DEFAULT_IE (20*INC_IE)        // 2 -> 1:2
#define DEFAULT_BPM 10

typedef enum control_mode {VOL, PRESS} control_mode;

typedef unsigned short vol_t;       // ml, f32.VOLUME_FRACBITS
typedef short press_t;              // cmH2O, f32.PRESS_FRACBITS
typedef unsigned short ie_t;        // ratio, f32.IE_FRACBITS, exp/insp

typedef struct vent_config {
  vol_t tidal_vol;
  press_t control_pressure;
  press_t peep;
  ie_t ie;
  unsigned short bpm;
  control_mode mode;
  int end;
} vent_config;

#endif // VENT_CONFIG
