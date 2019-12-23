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

const int ActorLength = 20;
const int SignalLength = 4;

enum class Mode {
    Actors,
    Signals
};

struct DocumentState {
    Mode mode;
    int selectedActor;
    int selectedTransition;
    int marginH;
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

void printSpaces(WINDOW* w, int posY, int posX, int len) {
    for(int i = 0; i < len; i++) {
        mvwaddch(w, posY, posX + i, ' ');
    }
}

void printCenter(WINDOW* w, const char * text, int posY, int posX, int len) {
    int padding = (len - (int)strlen(text)) / 2;
    mvwaddnstr(w, posY, posX + padding, text, len);
}


class ActorWindow {
private:
    WINDOW *w;
    Document* doc;
    DocumentState* state;
    
    void refresh() {
        prefresh(w, 0, 0 + state->marginH, 2, 1, 6, getmaxx(stdscr) - 2);
    }
    
public:
    
    ActorWindow(Document* d, DocumentState* s) {
        
        doc = d;
        state = s;
        
        int maxx = getmaxx(stdscr);
        w = newpad(5, doc->actors.size() * ActorLength);
        mvhline(7, 1, '-', maxx - 2);
        prefresh(w, 0 + state->marginH, 0, 2, 1, 6, getmaxx(stdscr) - 2);
    }
    
    void adjustMargin() {
        
        // Margin when scrolling right
        if((state->selectedActor + 1) * ActorLength - state->marginH > getmaxx(stdscr) - 2) {
            
            state->marginH =
            ((state->selectedActor + 1) * ActorLength)
            -
            (getmaxx(stdscr) - 2);
            
        // Margin when scrolling left
        } else if(state->marginH >= state->selectedActor * ActorLength) {
            state->marginH = state->selectedActor * ActorLength;
        }
        
    }
    
    void redraw() {
                
        for(int i = 0; i < doc->actors.size(); i++) {
            
            if(i == state->selectedActor && state->mode == Mode::Actors) wattron(w, A_STANDOUT);
            
            printSpaces(w, 0, ActorLength * i, ActorLength);
            printSpaces(w, 1, ActorLength * i, ActorLength);
            printSpaces(w, 2, ActorLength * i, ActorLength);
            printSpaces(w, 3, ActorLength * i, ActorLength);
            printSpaces(w, 4, ActorLength * i, ActorLength);
            
            if(doc->actors[i].type == ActorType::Player) {
                printCenter(w, "()", 0, ActorLength * i, ActorLength);
                printCenter(w, "/\\", 1, ActorLength * i, ActorLength);
                printCenter(w, "/\\", 2, ActorLength * i, ActorLength);
            } else {
                printCenter(w, "+----+", 0, ActorLength * i, ActorLength);
                printCenter(w, "|    |", 1, ActorLength * i, ActorLength);
                printCenter(w, "+----+", 2, ActorLength * i, ActorLength);
            }
            
            printCenter(w, doc->actors[i].name.c_str(), 4, ActorLength * i, ActorLength);
            
            if(i == state->selectedActor && state->mode == Mode::Actors) wattroff(w, A_STANDOUT);
            
            
        }
        
        refresh();
        
    }
};

class SignalWindow {
private:
    WINDOW *w;
    Document* doc;
    DocumentState* state;
    
    void redrawActorLines() {
            
            // Thin lines
            for(int i = 0; i < doc->actors.size(); i++) {
                mvwvline(w, 0, ActorLength * i + (ActorLength / 2), '|', SignalLength * doc->signals.size());
            }
            
            // Lifetime lines
    //
    //        bool* sigHas = new bool [doc->actors.size()] { false };
    //        int* sigMin = new int [doc->actors.size()] { INT_MAX };
    //        int* sigMax = new int [doc->actors.size()] { 0 };
    //
    //        for(auto const& s : doc->signals) {
    //            sigHas[s.source] = true;
    //            sigHas[s.destination] = true;
    //            if(s.id < sigMin[s.source]) sigMin[s.source] = s.id;
    //            if(s.id > sigMax[s.destination]) sigMax[s.destination] = s.id;
    //        }
    //
    //
    //        for(int i = 0; i < doc->actors.size(); i++) {
    //            if(sigHas[i] && sigMin[i] != sigMax[i]) {
    //                mvwvline(w, sigMin[i] * SignalLength + 2, ActorLength * i + (ActorLength / 2), 'X', sigMax[i] * SignalLength - 1);
    //            }
    //        }
    //
    //        delete [] sigHas;
    //        delete [] sigMin;
    //        delete [] sigMax;
        }
    
public:
    
    SignalWindow(Document* d, DocumentState* s) {
        
        doc = d;
        state = s;
        
        w = newpad(100, doc->actors.size() * ActorLength);
        
        draw();
    }
    
    
    
    
    void draw() {
        
        wclear(w);
        redrawActorLines();
        
        int source, dest;
        Signal* s;
        
        for(int i = 0; i < doc->signals.size(); i++) {
            s = &doc->signals[i];
            
            // Signal right
            if(s->destination > s->source) {
                source = (s->source * ActorLength) + (ActorLength / 2) + 1;
                dest = (s->destination - s->source) * ActorLength - 1;
                printCenter(w, s->name.c_str(), SignalLength * i + 1, source, dest);
                
                switch(s->type) {
                    case SignalType::Changing:
                        mvwhline(w, SignalLength * i + 2, source, '=', dest);
                        break;
                    case SignalType::Informing:
                    default:
                        mvwhline(w, SignalLength * i + 2, source, '-', dest);
                        break;
                }
                mvwaddch(w, SignalLength * i + 2, source + dest - 1, '>');
            }
            
            // Signal left
            else {
                source = (s->destination * ActorLength) + (ActorLength / 2) + 1;
                dest = (s->source - s->destination) * ActorLength - 1;
                printCenter(w, s->name.c_str(), SignalLength * i + 1, source, dest);
                
                switch(s->type) {
                    case SignalType::Changing:
                        mvwhline(w, SignalLength * i + 2, source, '=', dest);
                        break;
                    case SignalType::Informing:
                    default:
                        mvwhline(w, SignalLength * i + 2, source, '-', dest);
                        break;
                }
                mvwaddch(w, SignalLength * i + 2, source, '<');
            }
        }
        
        prefresh(w, 0, 0 + state->marginH, 8, 1, getmaxy(stdscr) - 3, getmaxx(stdscr) - 2);
        
    }
};

class DocumentWindow {
private:
    WINDOW *w;
    int width;
    int height;
    
    ActorWindow* actorWin;
    SignalWindow* signalWin;
    
    Document doc;
    DocumentState state;
    
public:
    DocumentWindow() {
        
        doc = EXAMPLE_DOCUMENT;
        state = {Mode::Actors, 0, 0, 0};
        
        actorWin = new ActorWindow(&doc, &state);
        signalWin = new SignalWindow(&doc, &state);
        
        
        int maxx = getmaxx(stdscr);
        int maxy = getmaxy(stdscr);
        w = newwin(maxy - 2, maxx, 1, 0);
        width = getmaxx(w);
        height = getmaxy(w);
        box(w, 0, 0);
        wrefresh(w);
        actorWin->redraw();
        signalWin->draw();
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
                actorWin->adjustMargin();
                
                
                break;
            case KEY_RIGHT:
                state.selectedActor = (state.selectedActor + 1) % doc.actors.size();
                actorWin->adjustMargin();
                
                
//                if(DocumentActorLength * (state.selectedActor + 3) > getmaxx(stdscr) - 2) {
//                    state.marginH = DocumentActorLength;
//                }
                break;
            default:
                break;
        }
        actorWin->redraw();
        signalWin->draw();
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
