.PHONY: all time dump  

GENERAL_OPTS = -std=c++2a 
TIME_OPTS    = -D"TIME"
DUMP_OPTS    = -D"DUMP"

COMPILE_NAIVE    = g++ recursive.cpp -o recursive.out
COMPILE_PARALLEL = g++ parallel.cpp -o parallel.out -lpthread

all: 
	$(COMPILE_NAIVE) $(GENERAL_OPTS) 
	$(COMPILE_PARALLEL) $(GENERAL_OPTS) 


time: 
	$(COMPILE_NAIVE) $(GENERAL_OPTS) $(TIME_OPTS)
	$(COMPILE_PARALLEL) $(GENERAL_OPTS) $(TIME_OPTS)


dump: 
	$(COMPILE_NAIVE) $(GENERAL_OPTS) $(DUMP_OPTS) 
	$(COMPILE_PARALLEL) $(GENERAL_OPTS) $(DUMP_OPTS)