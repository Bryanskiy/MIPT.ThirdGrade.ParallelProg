.PHONY: all parallel naive debug 

all: 
	g++ -std=c++2a naive.cpp -o native.out -I matrix/ -g
	mpic++ -std=c++2a parallel.cpp -o parallel.out -I matrix/ -g

parallel: 
	mpic++ -std=c++2a parallel.cpp -o parallel.out -I matrix/ -g

debug: 
	g++ -std=c++2a naive.cpp -I matrix/ -g

naive:
	g++ -std=c++2a naive.cpp -o naive.out -I matrix/ 
