all: submodules main

submodules:
	git submodule update --init --recursive
	./prepareBackends.sh

main:
	g++ -std=c++14 -lncurses umlSequence/*.cpp umlSequence/backend-nice/*.cpp umlSequence/backend-nano/*.cpp -o main
