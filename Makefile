all: submodules main

submodules:
	git submodule update --init --recursive
	./prepareBackends.sh

main:
	g++ -std=c++14 -lncurses sequenceDiagram/*.cpp sequenceDiagram/backend-nice/*.cpp sequenceDiagram/backend-nano/*.cpp -o main
