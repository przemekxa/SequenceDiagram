//
//  Controller.hpp
//  umlSequence
//
//  Created by Przemek Ambroży on 23/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#ifndef Controller_hpp
#define Controller_hpp

#include <iostream>
#include <stdlib.h>
#include <curses.h>
#include <form.h>
#include <vector>
#include <fstream>

#include "json.hpp"
#include "Document.hpp"

using namespace std;

const int ActorLength = 20;
const int SignalLength = 4;

enum class Mode {
    Actors,
    Signals
};

struct DocumentState {
    Mode mode;
    string documentName;
    bool changed;
    int selectedActor;
    int selectedSignal;
    int marginH;
    int marginV;
};

void printSpaces(WINDOW* w, int posY, int posX, int len);

void printCenter(WINDOW* w, const char * text, int posY, int posX, int len);


// MARK: - Status bar

class StatusBar {
private:
    WINDOW* w;
    Document* doc;
    DocumentState* state;
public:
    
    StatusBar(Document* d, DocumentState* s) {
        doc = d;
        state = s;
        
        w = newwin(1, getmaxx(stdscr), getmaxy(stdscr) - 2, 0);
        wstandout(w);
    }
    
    ~StatusBar() {
        delwin(w);
    }
    
    void draw() {
        wclear(w);
        int width = getmaxx(stdscr);
        printSpaces(w, 0, 0, width);
        
        wmove(w, 0, width - 15);
        
        if (state->mode == Mode::Actors) {
            mvwprintw(w, 0, 1, doc->actors[state->selectedActor].name.c_str());
            mvwprintw(w, 0, width - 15, "MODE: Actors");
        } else {
            Signal& s =doc->signals[state->selectedSignal];
            string t = s.name;
            t += " [" + doc->actors[s.source].name;
            if(s.type == SignalType::Changing)
                t += " => ";
            else
                t += " -> ";
            t += doc->actors[s.destination].name + "]";
            mvwprintw(w, 0, 1, t.c_str());
            mvwprintw(w, 0, width - 15, "MODE: Signals");
        }
        wrefresh(w);
    }
};




// MARK: - Border

class BorderWindow {
private:
    WINDOW* w;
    DocumentState* state;
public:
    
    BorderWindow(DocumentState* s) {
        state = s;
        w = newwin(getmaxy(stdscr) - 3, getmaxx(stdscr), 1, 0);
    }
    
    ~BorderWindow() {
        delwin(w);
    }
    
    void draw() {
        wclear(w);
        box(w, 0, 0);
        mvwhline(w, 6, 1, '-', getmaxx(w) - 2);
        
        string name = state->documentName;
        if(state->changed) name += "*";
        
        printCenter(w, name.c_str(), 0, 0, getmaxx(stdscr) - 2);
        
        wrefresh(w);
    }
};




// MARK: - Actors

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
        
        w = newpad(5, doc->actors.size() * ActorLength);
        //prefresh(w, 0 + state->marginH, 0, 2, 1, 6, getmaxx(stdscr) - 2);
        draw();
    }
    
    ~ActorWindow() {
        delwin(w);
    }
    
    void adjustMarginsForActor(int id) {
        
        // Margin when scrolling right
        if((id + 1) * ActorLength - state->marginH > getmaxx(stdscr) - 2) {
            
            state->marginH =
            ((id + 1) * ActorLength)
            -
            (getmaxx(stdscr) - 2);
            
        // Margin when scrolling left
        } else if(state->marginH >= id * ActorLength) {
            state->marginH = id * ActorLength;
        }
        
    }
    
    void draw() {
                
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
    
    void draw(int focusOn, bool highlight) {
                
        for(int i = 0; i < doc->actors.size(); i++) {
            
            if(i == focusOn && highlight) wattron(w, A_STANDOUT);
            
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
            
            if(i == focusOn && highlight) wattroff(w, A_STANDOUT);
            
            
        }
        
        refresh();
        
    }
    
};




// MARK: - Signals


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
        
        w = newpad(doc->signals.size() * SignalLength, doc->actors.size() * ActorLength);
        
        draw();
    }
    
    void adjustMarginsForSignal(int id) {
        
        // Margin when scrolling down
        if((id + 1) * SignalLength - state->marginV > getmaxy(stdscr) - 10) {
            
            state->marginV =
            ((id + 1) * SignalLength)
            -
            (getmaxy(stdscr) - 10);
            
        // Margin when scrolling up
        } else if(state->marginV >= id * SignalLength) {
            state->marginV = id * SignalLength;
        }
        
    }
    
    
    void draw() {
        
        wclear(w);
        redrawActorLines();
        
        int source, dest;
        Signal* s;
        
        for(int i = 0; i < doc->signals.size(); i++) {
            s = &doc->signals[i];
            
            if(i == state->selectedSignal && state->mode == Mode::Signals) wattron(w, A_STANDOUT);
            
            // Signal right
            if(s->destination > s->source) {
                source = (s->source * ActorLength) + (ActorLength / 2) + 1;
                dest = (s->destination - s->source) * ActorLength - 1;
                printSpaces(w, SignalLength * i + 1, source, dest);
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
                printSpaces(w, SignalLength * i + 1, source, dest);
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
            
            if(i == state->selectedSignal && state->mode == Mode::Signals) wattroff(w, A_STANDOUT);
            
        }
        
        prefresh(w, 0 + state->marginV, 0 + state->marginH, 8, 1, getmaxy(stdscr) - 4, getmaxx(stdscr) - 2);
        
    }
};







// MARK: - Controller

class Controller {
private:
    
    WINDOW* docWin;
    ActorWindow* actorWin;
    SignalWindow* signalWin;
    BorderWindow* borderWin;
    StatusBar* statusBar;
    
    //
    // MARK: Mode changes
    //
    
    void actorsMode() {
        state.mode = Mode::Actors;
        actorWin->adjustMarginsForActor(state.selectedActor);
        draw();
    }
    
    void signalsMode() {
        state.mode = Mode::Signals;
        signalWin->adjustMarginsForSignal(state.selectedSignal);
        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].source);
        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].destination);
        draw();
    }
    
    //
    // MARK: Margin adjustments
    //
    
    void panLeft() {
        if(state.marginH > 0) state.marginH--;
        draw();
    }
    
    void panRight() {
        if(state.marginH < (doc.actors.size() * ActorLength - getmaxx(stdscr) + 2)) state.marginH++;
        draw();
    }
    
    void panUp() {
        if(state.marginV > 0) state.marginV--;
        draw();
    }
    
    void panDown() {
        if(state.marginV < (doc.signals.size() * SignalLength - getmaxy(stdscr) + 10)) state.marginV++;
        draw();
    }
    
    //
    // MARK: Actors
    //
    
    void nextActor() {
        state.selectedActor = (state.selectedActor + 1) % doc.actors.size();
        actorWin->adjustMarginsForActor(state.selectedActor);
        draw();
    }
    
    void previousActor() {
        if(state.selectedActor == 0) {
            state.selectedActor = doc.actors.size() - 1;
        } else {
            state.selectedActor--;
        }
        
        actorWin->adjustMarginsForActor(state.selectedActor);
        draw();
    }
    
    
    //
    // MARK: Signals
    //
    
    void nextSignal() {
        state.selectedSignal = (state.selectedSignal + 1) % doc.signals.size();
        signalWin->adjustMarginsForSignal(state.selectedSignal);
        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].source);
        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].destination);
        draw();
    }
    
    void previousSignal() {
        if(state.selectedSignal == 0) {
            state.selectedSignal = doc.signals.size() - 1;
        } else {
            state.selectedSignal--;
        }
        signalWin->adjustMarginsForSignal(state.selectedSignal);
        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].source);
        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].destination);
        draw();
    }
    
    //
    // MARK: Drawing
    //
    
    void draw() {
        borderWin->draw();
        actorWin->draw();
        signalWin->draw();
        statusBar->draw();
    }
    
    
    
public:
    
    // Document & it's state
    Document doc;
    DocumentState state;
    
    // Scrolling margin
    int marginH;
    int marginV;
    
    Controller() {
        doc = EXAMPLE_DOCUMENT;
        state = {Mode::Actors, "example.json", true, 0, 0, 0, 0};
        
        borderWin = new BorderWindow(&state);
        actorWin = new ActorWindow(&doc, &state);
        signalWin = new SignalWindow(&doc, &state);
        statusBar = new StatusBar(&doc, &state);
        draw();
    }
    
    
    void handleKey(int key) {
        borderWin->draw();
        switch (key) {
            case 'a':
                actorsMode();
                break;
            case 's':
                signalsMode();
                break;
            case KEY_LEFT:
                if(state.mode == Mode::Actors) previousActor();
                else panLeft();
                break;
            case KEY_RIGHT:
                if(state.mode == Mode::Actors) nextActor();
                else panRight();
                break;
            case KEY_UP:
                if(state.mode == Mode::Signals) previousSignal();
                else panUp();
                break;
            case KEY_DOWN:
                if(state.mode == Mode::Signals) nextSignal();
                else panDown();
                break;
            default:
                break;
        }
        actorWin->draw();
        signalWin->draw();
        statusBar->draw();
    }
    
};

#endif /* Controller_hpp */
