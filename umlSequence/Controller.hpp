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
#include <vector>
#include <fstream>

#include "json.hpp"
#include "Document.hpp"
#include "Controllers.hpp"
#include "Views.hpp"

using namespace std;


// MARK: - Controller

class Controller {
private:
    
    WINDOW* docWin;
    ActorsView* actorWin;
    SignalsView* signalWin;
    BorderView* borderWin;
    StatusBarView* statusBar;
    
    //
    // MARK: Mode changes
    //
    
    void actorsMode() {
        state.mode = Mode::Actors;
        draw();
    }
    
    void signalsMode() {
        state.mode = Mode::Signals;
        draw();
    }
    
    //
    // MARK: Panning
    //
    
    void panLeft() {
        if(state.marginH > 0) state.marginH--;
        draw(false);
    }
    
    void panRight() {
        if(state.marginH < (doc.actors.size() * ActorLength - getmaxx(stdscr) + 2)) state.marginH++;
        draw(false);
    }
    
    void panUp() {
        if(state.marginV > 0) state.marginV--;
        draw(false);
    }
    
    void panDown() {
        if(state.marginV < (doc.signals.size() * SignalLength - getmaxy(stdscr) + 10)) state.marginV++;
        draw(false);
    }
    
    //
    // MARK: Actors
    //
    
    void nextActor() {
        if(doc.actors.size() == 0) return;
        state.selectedActor = (state.selectedActor + 1) % doc.actors.size();
        draw();
    }
    
    void previousActor() {
        if(doc.actors.size() == 0) return;
        if(state.selectedActor == 0) {
            state.selectedActor = doc.actors.size() - 1;
        } else {
            state.selectedActor--;
        }
        
        draw();
    }
    
    
    
    //
    // MARK: Signals
    //
    
    void nextSignal() {
        if(doc.signals.size() == 0) return;
        state.selectedSignal = (state.selectedSignal + 1) % doc.signals.size();
        draw();
    }
    
    void previousSignal() {
        if(doc.signals.size() == 0) return;
        if(state.selectedSignal == 0) {
            state.selectedSignal = doc.signals.size() - 1;
        } else {
            state.selectedSignal--;
        }
        draw();
    }
    
    //
    // MARK: Drawing
    //
    
    void resetWindows() {
        delete actorWin;
        delete signalWin;
        actorWin = new ActorsView(&doc, &state);
        signalWin = new SignalsView(&doc, &state);
    }
    
    void draw(bool adjustingMargins = true) {
        
        // Adjust margins
        if(adjustingMargins) {
            
            switch(state.mode) {
                case Mode::Actors:
                case Mode::NewSignalSource:
                case Mode::NewSignalDestination:
                    
                    actorWin->adjustMarginsForActor(state.selectedActor);
                    break;
                    
                case Mode::Signals:
                default:
                    
                    signalWin->adjustMarginsForSignal(state.selectedSignal);
                    if(!doc.signals.empty()) {
                        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].source);
                        actorWin->adjustMarginsForActor(doc.signals[state.selectedSignal].destination);
                    }
                    
                    break;
            }
        }
        
        borderWin->draw();
        actorWin->draw();
        signalWin->draw();
        statusBar->draw();
    }
    
    
    
public:
    
    // Document, state, data controller, signal creator
    Document doc;
    DocumentState state;
    DataController* control;
    SignalCreator* creator;
    
    Controller() {
        doc = EXAMPLE_DOCUMENT;
        state = {Mode::Actors, "example.json", true};
        control = new DataController(&doc, &state);
        creator = new SignalCreator(&doc, &state, control);
        
        borderWin = new BorderView(&state);
        actorWin = new ActorsView(&doc, &state);
        signalWin = new SignalsView(&doc, &state);
        statusBar = new StatusBarView(&doc, &state);
        
        draw();
    }
    
    
    void handleKey(int key) {
        borderWin->draw();
        switch (key) {
            case 'a':
                if(state.mode == Mode::Signals) actorsMode();
                break;
            case 's':
                if(state.mode == Mode::Actors) signalsMode();
                break;
            case KEY_LEFT:
                if(state.mode == Mode::Actors ||
                   state.mode == Mode::NewSignalSource ||
                   state.mode == Mode::NewSignalDestination) previousActor();
                else panLeft();
                break;
            case KEY_RIGHT:
                if(state.mode == Mode::Actors ||
                   state.mode == Mode::NewSignalSource ||
                   state.mode == Mode::NewSignalDestination)  nextActor();
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
            case 'M':
                if(state.mode == Mode::Signals) {
                    if(control->moveSignal(state.selectedSignal, state.selectedSignal - 1)) {
                        state.selectedSignal--;
                        draw();
                    }
                }
                else if(state.mode == Mode::Actors) {
                    if(control->moveActor(state.selectedActor, state.selectedActor - 1)) {
                        state.selectedActor--;
                        draw();
                    }
                }
                break;
            case 'm':
                if(state.mode == Mode::Signals) {
                    if(control->moveSignal(state.selectedSignal, state.selectedSignal + 1)) {
                        state.selectedSignal++;
                        draw();
                    }
                }
                else if(state.mode == Mode::Actors) {
                    if(control->moveActor(state.selectedActor, state.selectedActor + 1)) {
                        state.selectedActor++;
                        draw();
                    }
                }
                break;
            case 'N':
                switch(state.mode) {
                    case Mode::Actors:
                        control->addActor(state.selectedActor, ActorType::Object, "UNNAMED");
                        resetWindows();
                        draw();
                        break;
                    case Mode::Signals:
                        creator->begin(state.selectedSignal);
                        draw();
                        break;
                    default: break;
                }
                break;
            case 'n':
                switch(state.mode) {
                    case Mode::Actors:
                        if(!doc.actors.empty()) state.selectedActor++;
                        control->addActor(state.selectedActor, ActorType::Object, "UNNAMED");
                        resetWindows();
                        draw();
                        break;
                    case Mode::Signals:
                        if(!doc.signals.empty())
                            creator->begin(state.selectedSignal + 1);
                        else
                            creator->begin(state.selectedSignal);
                        draw();
                        break;
                    case Mode::NewSignalSource:
                        creator->next();
                        draw();
                        break;
                    case Mode::NewSignalDestination:
                        creator->end();
                        resetWindows();
                        draw();
                        break;
                    default: break;
                }
                break;
            case 'c':
                switch(state.mode) {
                    case Mode::Actors:
                        control->toggleActor(state.selectedActor);
                        draw();
                        break;
                    case Mode::Signals:
                        control->toggleSignal(state.selectedSignal);
                        draw();
                        break;
                    default: break;
                }
                break;
            case 'x':
                if(state.mode == Mode::Signals) {
                    control->removeSignal(state.selectedSignal);
                    if(doc.signals.size() == state.selectedSignal) state.selectedSignal--;
                    if(doc.signals.empty()) state.selectedSignal = 0;
                    resetWindows();
                    draw();
                }
                else if(state.mode == Mode::Actors) {
                    control->removeActor(state.selectedActor);
                    if(doc.actors.size() == state.selectedActor) state.selectedActor--;
                    if(doc.actors.empty()) state.selectedActor = 0;
                    resetWindows();
                    draw();
                }
                break;
            case 'q':
                if(state.mode == Mode::NewSignalSource || state.mode == Mode::NewSignalDestination) {
                    creator->cancel();
                    draw();
                }
            default:
                break;
        }
        actorWin->draw();
        signalWin->draw();
        statusBar->draw();
    }
    
};

#endif /* Controller_hpp */

// 650
