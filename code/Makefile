simulator : simulator.o model_b.o
	cc -o simulator simulator.c model_b.o -lm

model_b.o : model_b.c vent_config.h ui.h core.h model_b.h
	cc -c model_b.c -lm
clean :
	rm simulator model_b.o
