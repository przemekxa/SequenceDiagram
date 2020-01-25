.PHONY: init submodule prepare

main:
	g++ -std=c++14 sequenceDiagram/*.cpp sequenceDiagram/backend-nice/*.cpp sequenceDiagram/backend-nano/*.cpp -o main -lncurses

init: submodule prepare

submodule:
	git submodule update --init --recursive
	
prepare:
	# nano
	rm -f sequenceDiagram/backend-nano/main.cpp
	rm -f sequenceDiagram/backend-nano/main
	
