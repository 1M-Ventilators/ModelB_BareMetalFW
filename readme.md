A Bare-Metal firmware approach for Model-B ventilators.
* Uses the "Super-Loop" paradigm, in which all of the main, non-interrupt, action takes place in a single infinite loop.
* Currently has stubs for UI and Core low level firmware that does not yet exist.
* Designed to be largely independent of the UI and Core firmware.

The super-loop code can be exercised now.
* Simulator.c
* Substitutes customized versions of UI and Core functions.
* Allows an input .csv file to exercise Model B modes and settings.
* New versions can be made to exercise UI without Core code and vice-versa.

To build/run:

cd code

make

./simulator ../data/sim_input.csv ../data/sim_output.csv
