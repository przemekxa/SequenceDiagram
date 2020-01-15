//
//  main.cpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#include "SequenceDiagram.hpp"
#include "backend-nano/nano.hpp"
#include "backend-nice/nice-like.hpp"

using namespace std;

int main(int argc, char* argv[]) {
    
    if(argc < 2) {
        cout << "Choose backend by running with option:\n\n";
        cout << argv[0] << " i - Nice\n";
        cout << argv[0] << " a - Nano\n";
        cout << argv[0] << " v - Vim\n";
        cout << argv[0] << " m - Mcedit\n";
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
    
    SequenceDiagram t;
    
    b->tool = &t;
    t.backend = b;
    
    t.init();
    b->start();
    
    delete b;
    
    return 0;
}
