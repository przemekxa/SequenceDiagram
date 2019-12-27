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
    
    
};

#endif /* Controllers_hpp */
