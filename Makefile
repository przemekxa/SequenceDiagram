all: main

submodules:
	git submodule update --init --recursive

main:
	g++ -std=c++14 -lncurses umlSequence/*.cpp umlSequence/backend-nice/*.cpp umlSequence/backend-nano/*.cpp -o umlSequenceApp
