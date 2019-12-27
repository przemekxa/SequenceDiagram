//
//  Controllers.hpp
//  umlSequence
//
//  Created by Przemek Ambroży on 27/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#ifndef Controllers_hpp
#define Controllers_hpp

#include <stdio.h>
#include "Document.hpp"

using namespace std;

class DataController {
private:
    Document* d;
    DocumentState* s;
    
public:
    
    DataController(Document* document, DocumentState* state) {
        d = document;
        s = state;
    }
    
    
    
    // MARK: Actors
    
    /// Add new actor
    /// @param pos Index to put new Actor
    /// @param type Type of the Actor
    /// @param name Name of the Actor
    void addActor(unsigned int pos, ActorType type, string name) {
        
        for(auto& s : d->signals) {
            if(s.source >= pos) s.source++;
            if(s.destination >= pos) s.destination++;
        }
        
        d->actors.emplace(d->actors.begin() + pos, Actor { type, name });
    }
    
    /// Delete actor at position
    /// @param pos Index of `Actor` to be deleted
    void removeActor(unsigned int pos) {
        
        auto it = d->signals.begin();
        while(it != d->signals.end()) {
            
            // Delete signals with the actor to be removed
            if(it->source == pos || it->destination == pos) {
                it = d->signals.erase(it);
            } else {
                
                // Modify signals with actors > actor to be removed
                if(it->source > pos) it->source--;
                if(it->destination > pos) it->destination--;
                
                it++;
            }
        }
        
        d->actors.erase(d->actors.begin() + pos);
    }
    
    /// Move actor to new position
    /// @param from Source position
    /// @param to New position
    bool moveActor(unsigned int from, int to) {
        if(to >= d->actors.size() || to < 0) return false;
        
        swap(d->actors[from], d->actors[to]);
        
        for(auto& i : d->signals) {
            if(i.source == from) i.source = to;
            else if(i.source == to) i.source = from;
            if(i.destination == from) i.destination = to;
            else if(i.destination == to) i.destination = from;
        }
        
        return true;
    }
    
    /// Change Actor type (Player/Object)
    /// @param pos Position of Actor to be changed
    void toggleActor(unsigned int pos) {
        if(d->actors.empty()) return;
        
        Actor& a = d->actors[pos];
        if(a.type == ActorType::Player)
            a.type = ActorType::Object;
        else
            a.type = ActorType::Player;
    }
    
    
    
    // MARK: Signals
    
    /// Add new signal
    /// @param pos Index to put new Signal
    /// @param source Source Actor
    /// @param dest Destination Actor
    /// @param type Type of the signal
    /// @param name Name of the signal
    void addSignal(unsigned int pos, unsigned int source, unsigned int dest, SignalType type, string name) {
        d->signals.emplace(d->signals.begin() + pos, Signal { type, name, source, dest });
    }
    
    /// Delete signal at position
    /// @param pos Index of `Signal` to be deleted
    void removeSignal(unsigned int pos) {
        d->signals.erase(d->signals.begin() + pos);
    }
    
    /// Move signal to new position
    /// @param from Source position
    /// @param to New position
    bool moveSignal(unsigned int from, int to) {
        if(to >= d->signals.size() || to < 0) return false;
        
        swap(d->signals[from], d->signals[to]);
        
        return true;
    }
    
    
    /// Change Signal type (Informing/Changing)
    /// @param pos Position of Signal to be changed
    void toggleSignal(unsigned int pos) {
        if(d->signals.empty()) return;
        
        Signal& i = d->signals[pos];
        if(i.type == SignalType::Informing)
            i.type = SignalType::Changing;
        else
            i.type = SignalType::Informing;
    }
    
    
};


class SignalCreator {
private:
    Document* d;
    DocumentState* s;
    DataController* c;
    
    unsigned int newSignalPosition = 0;
    unsigned int newSignalSource = 0;
    unsigned int savedSelectedSignal = 0;
    unsigned int savedSelectedActor = 0;
    
public:
    SignalCreator(Document* document, DocumentState* state, DataController* control) {
        d = document;
        s = state;
        c = control;
    }
    
    /// Begin creating new Signal
    /// @param pos Position to put the new Signal
    void begin(unsigned int pos) {
        if(d->actors.empty() || s->mode != Mode::Signals) return;
        
        // Save position to put new Signal
        newSignalPosition = pos;
        
        // Save current state
        savedSelectedActor = s->selectedActor;
        savedSelectedSignal = s->selectedSignal;
        
        // Set new mode
        s->mode = Mode::NewSignalSource;
        
        // Set new selected actor - for choosing
        s->selectedActor = min(d->signals[s->selectedSignal].source, d->signals[s->selectedSignal].destination);
    }
    
    /// Save source and continue with destination
    void next() {
        s->mode = Mode::NewSignalDestination;
        newSignalSource = s->selectedActor;
    }
    
    /// Finish creating new Signal
    void end() {
        
        c->addSignal(newSignalPosition, newSignalSource, s->selectedActor, SignalType::Informing, "CREATOR");
        s->mode = Mode::Signals;
        s->selectedActor = savedSelectedActor;
        s->selectedSignal = newSignalPosition;
    }
    
    /// Cancel creating new Signal
    void cancel() {
        if(s->mode == Mode::NewSignalSource || s->mode == Mode::NewSignalDestination) {
            // Restore prevoius state
            s->mode = Mode::Signals;
            s->selectedActor = savedSelectedActor;
            s->selectedSignal = savedSelectedSignal;
        }
    }
    
};

#endif /* Controllers_hpp */
