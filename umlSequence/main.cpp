//
//  main.cpp
//  umlSequence
//
//  Created by Przemek Ambroży on 13/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#include <iostream>
#include <stdlib.h>
#include <curses.h>
#include <form.h>
#include <vector>
#include <fstream>

#include "json.hpp"
#include "Document.hpp"

using namespace std;
using nlohmann::json;

const int DocumentActorLength = 10;
const int DocumentActorSpacing = 3;
const int DocumentActorWidth = DocumentActorLength + DocumentActorSpacing;

enum class Mode {
    Actors,
    Signals
};

struct DocumentState {
    Mode mode;
    int selectedActor;
    int selectedTransition;
};


class StatusBar {
private:
    WINDOW *w;
    int width;
public:
    StatusBar() {
        w = newwin(1, getmaxx(stdscr), getmaxy(stdscr) - 1, 0);
        width = getmaxx(w);
        wstandout(w);
    }
    ~StatusBar() {
        delwin(w);
    }
    
    void setMode(Mode &mode) {
        wclear(w);
        width = getmaxx(w);
        for(int i = 0; i < width; i++) {
            waddch(w, ' ');
        }
        wmove(w, 0, width - 15);
        
        if (mode == Mode::Actors) {
            wprintw(w, "Mode: Actors");
        } else {
            wprintw(w, "Mode: Signals");
        }
        wrefresh(w);
    }
    
    void setModeLite(Mode &mode) {
        wmove(w, 0, width - 9);
        if (mode == Mode::Actors) {
            wprintw(w, "Actors ");
        } else {
            wprintw(w, "Signals");
        }
        wrefresh(w);
    }
};

class ActorWindow {
private:
    WINDOW *w;
    int width;
    int height;
    int margin = 0;
    Document* doc;
    DocumentState* state;
public:
    
    ActorWindow(Document* d, DocumentState* s) {
        
        doc = d;
        state = s;
        
        int maxx = getmaxx(stdscr);
        w = newwin(7, maxx - 2, 2, 1);
        width = getmaxx(w);
        height = getmaxy(w);
        //box(w, 0, 0);
        mvhline(7, 1, '-', maxx - 2);
        //scrollok(w, true);
        wrefresh(w);
    }
    
    void redraw() {
        
        int sumW = 0;
        int dispW;
        
        if(state->selectedActor * DocumentActorWidth > width - DocumentActorWidth) {
            margin = -20;
        } else {
            margin = 0;
        }
        
        for(int i = 0; i < doc->actors.size(); i++) {
            
            if(sumW + DocumentActorWidth < width) {
                dispW = DocumentActorLength;
            } else {
                dispW = width - sumW;
            }
            
            if(i == state->selectedActor) {
                wattron(w, A_STANDOUT);
            }
            
            if(doc->actors[i].type == ActorType::Player) {
                mvwaddnstr(w, 0, DocumentActorWidth*i + margin, "    ()    ", dispW);
                mvwaddnstr(w, 1, DocumentActorWidth*i, "    /\\    ", dispW);
                mvwaddnstr(w, 2, DocumentActorWidth*i, "    /\\    ", dispW);
            } else {
                mvwaddnstr(w, 0, DocumentActorWidth*i, "  +----+  ", dispW);
                mvwaddnstr(w, 1, DocumentActorWidth*i, "  |    |  ", dispW);
                //mvwprintw(w, 2, DocumentActorWidth*i + 1, "  +----+  ");
                mvwaddnstr(w, 2, DocumentActorWidth*i, "  +----+  ", dispW);
            }
            mvwaddnstr(w, 3, DocumentActorWidth*i + margin, "          ", dispW);
            mvwaddnstr(w, 4, DocumentActorWidth*i, "          ", dispW);
            mvwaddnstr(w, 4,
                       DocumentActorWidth*i + ( (DocumentActorLength - doc->actors[i].name.length()) / 2),
                       doc->actors[i].name.c_str(),
                       dispW - ((DocumentActorLength - doc->actors[i].name.length()) / 2));
            
            if(i == state->selectedActor) {
                wattroff(w, A_STANDOUT);
            }
            
            sumW += DocumentActorWidth;
            
        }
        
        wrefresh(w);
        
    }
};

class DocumentWindow {
private:
    WINDOW *w;
    int width;
    int height;
    
    ActorWindow* actorWin;
    
    Document doc;
    DocumentState state;
    
public:
    DocumentWindow() {
        
        doc = EXAMPLE_DOCUMENT;
        state = {Mode::Actors, 0, 0};
        
        actorWin = new ActorWindow(&doc, &state);
        
        
        int maxx = getmaxx(stdscr);
        int maxy = getmaxy(stdscr);
        w = newwin(maxy - 2, maxx, 1, 0);
        width = getmaxx(w);
        height = getmaxy(w);
        box(w, 0, 0);
        wrefresh(w);
        actorWin->redraw();
    }
    
    void setName(string name) {
        wmove(w, 0, width / 2 - (name.length() / 2));
        wprintw(w, name.c_str());
        wrefresh(w);
    }
    
    void handleKey(int key) {
        switch (key) {
            case 'a':
                state.mode = Mode::Actors;
                break;
            case 's':
                state.mode = Mode::Signals;
                break;
            case KEY_LEFT:
                if(state.selectedActor == 0) {
                    state.selectedActor = doc.actors.size() - 1;
                } else {
                    state.selectedActor--;
                }
                break;
            case KEY_RIGHT:
                state.selectedActor = (state.selectedActor + 1) % doc.actors.size();
                break;
            default:
                break;
        }
        actorWin->redraw();
    }
    
};



int main() {
    
    Mode mode = Mode::Actors;
    int key;
    
    initscr();
    keypad(stdscr, TRUE);
    cbreak();
    noecho();
    

    clear();
    printw("Hello");
    refresh();
    
    StatusBar s;
    s.setMode(mode);
    
    DocumentWindow win;
    win.setName("example.json");
    
    Document example = EXAMPLE_DOCUMENT;
    
    while(1) {
        key = getch();
        switch (key) {
            case 'a':
                mode = Mode::Actors;
                break;
            case 's':
                mode = Mode::Signals;
                break;
            default:
                break;
        }
        win.handleKey(key);
        s.setMode(mode);
        move(0, 0);
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
