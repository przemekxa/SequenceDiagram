//
//  main.cpp
//  umlSequence
//
//  Created by Przemek Ambroży on 13/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#include "UMLSequenceDiagram.hpp"
#include "backend-nano/nano.hpp"
#include "backend-nice/nice-like.hpp"

using namespace std;
using nlohmann::json;

int main(int argc, char* argv[]) {
    
    if(argc < 2) {
        cout << "Choose backend by running with option:\n\n";
        cout << "./" << argv[0] << " i - Nice\n";
        cout << "./" << argv[0] << " a - Nano\n";
        cout << "./" << argv[0] << " v - Vim\n";
        cout << "./" << argv[0] << " m - Mcedit\n";
        return 0;
    }


    Backend* b;

    switch(argv[1][0]) {
        case 'i':
            b = new nice();
            break;
        case 'a':
            b = new Nano();
            break;
        case 'v':
            //b = new Vim;
            break;
        case 'm':
            //b = new Mcedit;
            break;
    }
    
    UMLSequenceDiagram t;
    
    b->tool = &t;
    t.backend = b;
    
    t.init();
    b->start();
    
    delete b;
    
    
//    Nano b;
//    UMLSequenceDiagram diagram;
//
//    b.tool = &diagram;
//    diagram.backend = &b;
//
//    diagram.init();
//    b.start();
    
    return 0;
}


//int main() {
//
//    int key;
//
//    initscr();
//    keypad(stdscr, TRUE);
//    cbreak();
//    noecho();
//
//    UMLSequenceDiagram c;
//
//    while(1) {
//        key = getch();
//        c.handleKey(key);
//    }
//    endwin();
//    return 0;
//}


//int main() {
//
//    ofstream file("example.json");
//    file << json(EXAMPLE_DOCUMENT).dump(4) << endl;
//
//    return 0;
//}
