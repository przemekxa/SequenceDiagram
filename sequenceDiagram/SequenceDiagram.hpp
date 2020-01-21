//
//  Controller.hpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#ifndef Controller_hpp
#define Controller_hpp

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <map>

#include "json.hpp"
#include "Document.hpp"
#include "Controllers.hpp"
#include "Views.hpp"
#include "obiektowe/shared.hpp"

using namespace std;

class SequenceDiagram : public Tool
{
private:
    
    // Views
    ActorsView* actorWin;
    SignalsView* signalWin;
    BorderView* borderWin;
    StatusBarView* statusBar;
    
    // Entries storage
    map<string, string> entries;
    
    // Document, state, data controller, signal creator
    Document doc;
    DocumentState state;
    DataController* control;
    SignalCreator* creator;

    // Mode changes
    void actorsMode();
    void signalsMode();
    
    // Panning
    void panLeft();
    void panRight();
    void panUp();
    void panDown();
    
    // Actors
    void nextActor();
    void previousActor();
    
    // Signals
    void nextSignal();
    void previousSignal();
    
    // Drawing
    void resetWindows();
    void draw(bool adjustingMargins = true);
    
    // Bindings and keys
    void bindActors();
    void bindSignals();
    void bindLeft();
    void bindRight();
    void bindUp();
    void bindDown();
    void bindToggleType();
    void bindRename();
    void bindDelete();
    void bindNewUp();
    void bindNewDown();
    void bindMoveUp();
    void bindMoveDown();
    void bindNew();
    void bindOpen();
    void bindSave();
    void bindSaveAs();
    void setupBindings();
    void handleKey();
    void handleKey(int key);
    
public:
    
    SequenceDiagram ();
    void init();
    void redraw();
    void setEntry(string field, string value);
    string getEntry(string field);
};


#endif /* Controller_hpp */
