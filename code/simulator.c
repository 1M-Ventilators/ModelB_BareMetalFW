#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "model_b.h"
#include "proc.h"

//_____________________________________________________________________________
// Global variables
vent_config   config;
func_table_t  func_table;
float         elapsed_secs = 0.0f;
float         breath_secs, inspire_secs, expire_secs; 

FILE    *fp_in, *fp_out;
char    mode_set[100];
float   vol_set;
float   press_set;
float   peep_set;
float   ie_set;
int     bpm_set;
int     end_set;
int     update_flag = 1;
float   update_secs;

//_____________________________________________________________________________
// Simulator funcs
float eval_fixed(int val, int fracbits) {
  return (float)(val)/(float)pow((double)(2.0), (double)(fracbits));
}

float eval_volume(vol_t v) {
  return eval_fixed(v, VOLUME_FRACBITS);
}

float eval_press(press_t p) {
  return eval_fixed(p, PRESS_FRACBITS);
}

float eval_ie(ie_t ie) {
  return eval_fixed(ie, IE_FRACBITS);
}

int print_settings(vent_config config) {
   if (config.mode == VOL)
     printf("Mode: VOLUME CONTROL  ||  ");
   if (config.mode == PRESS)
     printf("Mode: PRESSURE CONTROL  ||  ");
   printf("Vol: %6.1f ml  ||  ", eval_volume(config.tidal_vol));
   printf("Press: %4.1f cmH2O  ||  ", eval_press(config.control_pressure));
   printf("PEEP: %4.1f cmH2O ||  ", eval_press(config.peep));
   printf("I : E: %5.2f  ||  ", eval_ie(config.ie));
   printf("BPM: %d\n", config.bpm);
   if (config.mode == VOL)
     fprintf(fp_out, "VOL, %6.1f, %4.1f, %4.1f, %5.2f, %d\n", \
         eval_volume(config.tidal_vol), eval_press(config.control_pressure), \
         eval_press(config.peep), eval_ie(config.ie), config.bpm); 
   if (config.mode == PRESS)
     fprintf(fp_out, "PRESS, %6.1f, %4.1f, %4.1f, %5.2f, %d\n", \
         eval_volume(config.tidal_vol), eval_press(config.control_pressure), \
         eval_press(config.peep), eval_ie(config.ie), config.bpm); 
}

int int_from_float(float val, int fracbits) {
  double int_part, frac_part;
  int conv;

  frac_part = modf((double)(val), &int_part);
  conv = ((int)(int_part)<<fracbits) + (int)( frac_part*pow(2.0f,(double)(fracbits)) );
  return conv;
}

//_____________________________________________________________________________
// Simulator versions of vent functions

vent_config sim_update_config(vent_config config) {
  // We read lines from the sim_input file as needed, and use them to reconfigure model_b at the designated time. 
  if (update_flag == 1) {
    // read a line from the input file
    fscanf(fp_in, "%f, %6[^,], %f, %f, %f, %f, %d, %d", &update_secs, mode_set, &vol_set, &press_set, &peep_set, &ie_set, &bpm_set, &end_set);
    update_flag = 0;
  }
  if (elapsed_secs >= update_secs) {
    // Change settings of model_b to reflect the last read input line of sim_input file.
    // Also log the new settings to the output file
    if (!strcmp(mode_set, "VOL")){
      config.mode = VOL;
      fprintf(fp_out, "%5.0f, VOL, %6.1f, %4.1f, %4.1f, %5.2f, %d\n", update_secs, vol_set, press_set, peep_set, ie_set, bpm_set);  
    }
    if (!strcmp(mode_set, "PRESS")) {
      config.mode = PRESS;
      fprintf(fp_out, "%5.0f, PRESS, %6.1f, %4.1f, %4.1f, %5.2f, %d\n", update_secs, vol_set, press_set, peep_set, ie_set, bpm_set);  
    }
    config.tidal_vol = (vol_t)(int_from_float(vol_set, VOLUME_FRACBITS));
    config.control_pressure = (press_t)(int_from_float(press_set, PRESS_FRACBITS));
    config.peep = (press_t)(int_from_float(peep_set, PRESS_FRACBITS));
    config.ie = (ie_t)(int_from_float(ie_set, IE_FRACBITS));
    config.bpm = (int)(bpm_set);
    config.end = (int)(end_set);
    update_flag = 1;
  }
  return config;
}

press_t sim_volume_inspire(unsigned int ticks, vent_config config) {
  // update the simulator elapsed time with the configured inspire time
  press_t press;
  press = 0;
  inspire_secs = (float)(ticks)/(float)(TICKS_PER_SEC);
  return press;
}

vol_t sim_volume_expire(unsigned int ticks, vent_config config) {
  // update the simulator elapsed time with the configured expire time
  vol_t vol;
  vol = 0;
  expire_secs = (float)(ticks)/(float)(TICKS_PER_SEC);
  breath_secs = inspire_secs + expire_secs;
  elapsed_secs += breath_secs;
  return vol;
}

press_t sim_pressure_inspire(unsigned int ticks, vent_config config) {
  // update the simulator elapsed time with the configured inspire time
  press_t press;
  press = 0;
  inspire_secs = (float)(ticks)/(float)(TICKS_PER_SEC);
  return press;
}

vol_t sim_pressure_expire(unsigned int ticks, vent_config config) {
  // update the simulator elapsed time with the configured expire time
  vol_t vol;
  vol = 0;
  expire_secs = (float)(ticks)/(float)(TICKS_PER_SEC);
  breath_secs = inspire_secs + expire_secs;
  elapsed_secs += breath_secs;
  return vol;
}

void sim_update_display(vent_config config) {
  // log the parameters of the last breath to the sim_output file
  fprintf(fp_out, ", , , , , , , , ");
  fprintf(fp_out, "%6.2f, %3.1f, %3.1f, %3.1f,", elapsed_secs, breath_secs, inspire_secs, expire_secs);
  print_settings(config);
  printf("elapsed: %6.2f   ||   breath: %3.1f   ||   inspire: %3.1f   ||   expire: %3.1f\n\n", elapsed_secs, breath_secs, inspire_secs, expire_secs); 
  return;
}

//
// main
int main(int argc, char **argv) {
  if ((argc < 2) || (argc > 3)) {
    printf("Expecting one or two filenames on command line.\n");
  } else {
    // open our input file and read the column header row
    fp_in = fopen(argv[1], "r");
    if (fp_in == NULL)
      exit(EXIT_FAILURE);
    char buf[256];
    fscanf(fp_in, "%s %s %s %s %s %s %s %s", buf, buf, buf, buf, buf, buf, buf, buf);
    // open file for simulation results
    if (argc > 2)
      fp_out = fopen(argv[2], "w");
    else
      fp_out = fopen("sim_output.csv", "w");
    if (fp_out == NULL)
      exit(EXIT_FAILURE);
    fprintf(fp_out, "Time, CMD_Mode, CMD_TV, CMD_PRESS, CMD_PEEP, CMD_I:E, CMD_BPM, , Elapsed_Time, Breath, Inspire, Expire, Mode, Tidal_Volume, Control_Pressure, PEEP, IE, BPM\n");

    // set up function table
    func_table.init_config = init_config;
    func_table.update_config = sim_update_config;
    func_table.volume_inspire = sim_volume_inspire;
    func_table.volume_expire = sim_volume_expire;
    func_table.pressure_inspire = sim_volume_inspire;
    func_table.pressure_expire = sim_volume_expire;
    func_table.update_display = sim_update_display;

    // call model_b
    vent_main(func_table, config);
    fclose(fp_in);
    fclose(fp_out);
  }
  return 0;
}
