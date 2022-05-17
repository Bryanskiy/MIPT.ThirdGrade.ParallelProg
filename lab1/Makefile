.PHONY: all time dump  

GENERAL_OPTS = -std=c++2a -I matrix/ 
TIME_OPTS    = -D"TIME"
DUMP_OPTS    = -D"DUMP"

COMPILE_NAIVE    = g++ naive.cpp helper.cpp -o naive.out
COMPILE_PARALLEL = mpic++ parallel.cpp helper.cpp -o parallel.out 

all: 
	$(COMPILE_NAIVE) $(GENERAL_OPTS) 
	$(COMPILE_PARALLEL) $(GENERAL_OPTS) 


time: 
	$(COMPILE_NAIVE) $(GENERAL_OPTS) $(TIME_OPTS)
	$(COMPILE_PARALLEL) $(GENERAL_OPTS) $(TIME_OPTS)


dump: 
	$(COMPILE_NAIVE) $(GENERAL_OPTS) $(DUMP_OPTS) 
	$(COMPILE_PARALLEL) $(GENERAL_OPTS) $(DUMP_OPTS)
