//
//  main.cpp
//  umlSequence
//
//  Created by Przemek Ambroży on 13/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#include "UMLSequenceDiagram.hpp"

using namespace std;
using nlohmann::json;


int main() {
    
    int key;
    
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    
    UMLSequenceDiagram c;
        
    while(1) {
        key = getch();
        c.handleKey(key);
    }
    endwin();
    return 0;
}


//int main() {
//
//    ofstream file("example.json");
//    file << json(EXAMPLE_DOCUMENT).dump(4) << endl;
//
//    return 0;
//}
