//
//  Controllers.hpp
//  umlSequence
//
//  Created by Przemek Ambroży on 27/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#ifndef Controllers_hpp
#define Controllers_hpp

#include "Document.hpp"
#include <fstream>

using namespace std;


enum class FileError {
    OpeningError,
    SavingError
};


class DataController {
private:
    Document* d;
    DocumentState* s;
    
public:
    
    DataController(Document* document, DocumentState* state);
    
    // MARK: Actors
    
    /// Add new actor
    /// @param pos Index to put new Actor
    /// @param type Type of the Actor
    /// @param name Name of the Actor
    void addActor(unsigned int pos, ActorType type, string name);
    
    /// Delete actor at position
    /// @param pos Index of `Actor` to be deleted
    void removeActor(unsigned int pos);
    
    /// Move actor to new position
    /// @param from Source position
    /// @param to New position
    bool moveActor(unsigned int from, int to);
    
    /// Change Actor type (Player/Object)
    /// @param pos Position of Actor to be changed
    void toggleActor(unsigned int pos);
    
    
    
    // MARK: Signals
    
    /// Add new signal
    /// @param pos Index to put new Signal
    /// @param source Source Actor
    /// @param dest Destination Actor
    /// @param type Type of the signal
    /// @param name Name of the signal
    void addSignal(unsigned int pos, unsigned int source, unsigned int dest, SignalType type, string name);
    
    /// Delete signal at position
    /// @param pos Index of `Signal` to be deleted
    void removeSignal(unsigned int pos);
    
    /// Move signal to new position
    /// @param from Source position
    /// @param to New position
    bool moveSignal(unsigned int from, int to);
    
    
    /// Change Signal type (Informing/Changing)
    /// @param pos Position of Signal to be changed
    void toggleSignal(unsigned int pos);
    
    
    
    // MARK: File I/O
    
    /// Create new Document
    void newDocument();
    
    /// Save a file
    /// @param filename Name of the file
    void save(string filename);
    
    /// Open a file
    /// @param filename Name of the file
    void open(string filename);
    
    
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
    SignalCreator(Document* document, DocumentState* state, DataController* control);
    
    /// Begin creating new Signal
    /// @param pos Position to put the new Signal
    void begin(unsigned int pos);
    
    /// Save source and continue with destination
    void next();
    
    /// Finish creating new Signal
    void end();
    
    /// Cancel creating new Signal
    void cancel();
    
};

#endif /* Controllers_hpp */
