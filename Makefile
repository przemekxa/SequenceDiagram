main:
	g++ -std=c++14 sequenceDiagram/*.cpp sequenceDiagram/backend-nice/*.cpp sequenceDiagram/backend-nano/*.cpp -o main -lncurses

init:
	git submodule update --init --recursive
	./prepareBackends.sh

