//
//  Controller.cpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#include "SequenceDiagram.hpp"

//
// MARK: Mode changes
//

void SequenceDiagram::actorsMode() {
    state.mode = Mode::Actors;
    draw();
}

void SequenceDiagram::signalsMode() {
    state.mode = Mode::Signals;
    draw();
}

//
// MARK: Panning
//

void SequenceDiagram::panLeft() {
    if(state.marginH > 0) state.marginH--;
    draw(false);
}

void SequenceDiagram::panRight() {
    if(state.marginH < (doc.actors.size() * ActorLength - getmaxx(stdscr) + 2)) state.marginH++;
    draw(false);
}

void SequenceDiagram::panUp() {
    if(state.marginV > 0) state.marginV--;
    draw(false);
}

void SequenceDiagram::panDown() {
    if(state.marginV < (doc.signals.size() * SignalLength - getmaxy(stdscr) + 10)) state.marginV++;
    draw(false);
}

//
// MARK: Actors
//

void SequenceDiagram::nextActor() {
    if(doc.actors.size() == 0) return;
    state.selectedActor = (state.selectedActor + 1) % doc.actors.size();
    draw();
}

void SequenceDiagram::previousActor() {
    if(doc.actors.size() == 0) return;
    if(state.selectedActor == 0) {
        state.selectedActor = (unsigned int)doc.actors.size() - 1;
    } else {
        state.selectedActor--;
    }
    
    draw();
}

//
// MARK: Signals
//

void SequenceDiagram::nextSignal() {
    if(doc.signals.size() == 0) return;
    state.selectedSignal = (state.selectedSignal + 1) % doc.signals.size();
    draw();
}

void SequenceDiagram::previousSignal() {
    if(doc.signals.size() == 0) return;
    if(state.selectedSignal == 0) {
        state.selectedSignal = (unsigned int)doc.signals.size() - 1;
    } else {
        state.selectedSignal--;
    }
    draw();
}

//
// MARK: Drawing
//

void SequenceDiagram::resetWindows() {
    delete actorWin;
    delete signalWin;
    actorWin = new ActorsView(&doc, &state);
    signalWin = new SignalsView(&doc, &state);
}

void SequenceDiagram::draw(bool adjustingMargins) {
    
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

//
// MARK: - Init
//

SequenceDiagram::SequenceDiagram() {
    doc = EXAMPLE_DOCUMENT;
    state = {Mode::Actors, "example.json"};
    control = new DataController(&doc, &state);
    creator = new SignalCreator(&doc, &state, control);
    
    
    
    borderWin = new BorderView(&state);
    actorWin = new ActorsView(&doc, &state);
    signalWin = new SignalsView(&doc, &state);
    statusBar = new StatusBarView(&doc, &state);
    
    draw();
}

void SequenceDiagram::init() {
    setupBindings();
    draw();
}

void SequenceDiagram::redraw() {
    draw(false);
}

//
// MARK: Entries
//

void SequenceDiagram::setEntry(string field, string value) {
    entries[field] = value;
}

string SequenceDiagram::getEntry(string field) {
    if(field == "IS_SAVED") {
        return state.changed ? "NO" : "YES";
    }
    return entries[field];
}

//
// MARK: Bindings
//

void SequenceDiagram::bindActors() {
    mvaddstr(0, 0, "<CTRL>1          ");
    if(state.mode == Mode::Signals) actorsMode();
}

void SequenceDiagram::bindSignals() {
    mvaddstr(0, 0, "<CTRL>2          ");
    if(state.mode == Mode::Actors) signalsMode();
}

void SequenceDiagram::bindLeft() {
    if(state.mode == Mode::Actors ||
       state.mode == Mode::NewSignalSource ||
       state.mode == Mode::NewSignalDestination) previousActor();
    else panLeft();
}

void SequenceDiagram::bindRight() {
    if(state.mode == Mode::Actors ||
       state.mode == Mode::NewSignalSource ||
       state.mode == Mode::NewSignalDestination)  nextActor();
    else panRight();
}

void SequenceDiagram::bindUp() {
    if(state.mode == Mode::Signals) previousSignal();
    else panUp();
}

void SequenceDiagram::bindDown() {
    if(state.mode == Mode::Signals) nextSignal();
    else panDown();
}


void SequenceDiagram::bindToggleType() {
    mvaddstr(0, 0, "<CTRL>t          ");
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
}

void SequenceDiagram::bindRename() {
    mvaddstr(0, 0, "<CTRL>r          ");
    if(state.mode == Mode::Signals) {
        control->renameSignal(entries["RENAME"]);
        draw();
    }
    else if(state.mode == Mode::Actors) {
        control->renameActor(entries["RENAME"]);
        draw();
        draw();
    }
}

void SequenceDiagram::bindDelete() {
    mvaddstr(0, 0, "<CTRL>x          ");
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
}


void SequenceDiagram::bindNewUp() {
    mvaddstr(0, 0, "<CTRL><SHIFT>w          ");
    switch(state.mode) {
        case Mode::Actors:
            control->addActor(state.selectedActor, ActorType::Object, "New Actor");
            resetWindows();
            draw();
            break;
        case Mode::Signals:
            creator->begin(state.selectedSignal);
            draw();
            break;
        default: break;
    }
}

void SequenceDiagram::bindNewDown() {
    mvaddstr(0, 0, "<CTRL>w          ");
    switch(state.mode) {
        case Mode::Actors:
            if(!doc.actors.empty()) state.selectedActor++;
            control->addActor(state.selectedActor, ActorType::Object, "New Actor");
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
}


void SequenceDiagram::bindMoveUp() {
    mvaddstr(0, 0, "<CTRL><SHIFT>M          ");
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
}

void SequenceDiagram::bindMoveDown() {
    mvaddstr(0, 0, "<CTRL>m          ");
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
}


void SequenceDiagram::bindNew() {
    mvaddstr(0, 0, "<CTRL>n          ");
    
    control->newDocument();
    resetWindows();
    draw();
}

void SequenceDiagram::bindOpen() {
    mvaddstr(0, 0, "<CTRL>o          ");
    
    try {
        control->open(entries["FILENAME"]);
        resetWindows();
        draw();
        //statusBar->setStatus("File " + entries["FILENAME"] + " opened.");
    } catch(...) {
        resetWindows();
        draw();
        //statusBar->setStatus("Error opening file.");
    }
    
}

void SequenceDiagram::bindSave() {
    mvaddstr(0, 0, "<CTRL>s          ");
    
    try {
        control->save(state.documentName);
        draw();
        //statusBar->setStatus("File " + entries["FILENAME"] + " saved.");
    } catch(...) {
        draw();
        //statusBar->setStatus("Error saving file.");
    }
    
}

void SequenceDiagram::bindSaveAs() {
    mvaddstr(0, 0, "<CTRL><SHIFT>s          ");
    
    try {
        control->save(entries["FILENAME"]);
        draw();
        //statusBar->setStatus("File " + entries["FILENAME"] + " saved.");
    } catch(...) {
        draw();
        //statusBar->setStatus("Error saving file.");
    }
    
}

//
// MARK: Bindings setup
//

void SequenceDiagram::setupBindings() {
    
    // Handle keys
    backend->bind("<EDITION>", [this]() { handleKey(); }, "---");
    
    
    // Toggle type
    // <CTRL> + T
    backend->bind("#nano#<CTRL>t%Toggle", [&]() { bindToggleType(); }, "Change Actor/Signal type");
    backend->bind("#nice#.Edit.ToggleType", [&]() { bindToggleType(); }, "Change Actor/Signal type");
    
    // Rename
    // <CTRL> + R
    backend->bind("#nano#<CTRL>r%Rename!New name${RENAME}", [this]() { bindRename(); }, "Rename Actor/Signal");
    backend->bind("#nice#.Edit.Rename${New name: |RENAME}", [this]() { bindRename(); }, "Rename Actor/Signal");
    
    // Delete
    // <CTRL> + X
    backend->bind("#nano#<CTRL>x%Delete", [this]() { bindDelete(); }, "Delete Actor/Signal");
    backend->bind("#nice#.Edit.Delete", [this]() { bindDelete(); }, "Delete Actor/Signal");
    
    
    
    // Actors mode
    // <CTRL> + 1
    backend->bind("#nano#<CTRL>1%Actors", [&]() { bindActors(); }, "Actors mode");
    backend->bind("#nice#.Mode.Actors", [&]() { bindActors(); }, "Actors mode");
    
    // Signals mode
    // <CTRL> + 2
    backend->bind("#nano#<CTRL>2%Signals", [&]() { bindSignals(); }, "Actors mode");
    backend->bind("#nice#.Mode.Signals", [&]() { bindSignals(); }, "Actors mode");
    

    
    // New up
    // <CTRL> + <SHIFT> + W
    backend->bind("#nano#<CTRL><SHIFT>W%New before", [&]() { bindNewUp(); }, "Create Actor/Signal before this");
    backend->bind("#nice#.New.Before", [&]() { bindNewUp(); }, "Create Actor/Signal before this");
    
    // New down
    // <CTRL> + W
    backend->bind("#nano#<CTRL>w%New here", [&]() { bindNewDown(); }, "Create Actor/Signal there");
    backend->bind("#nice#.New.There", [&]() { bindNewDown(); }, "Create Actor/Signal there");
    
    // Move up
    // <CTRL> + <SHIFT> + M
    backend->bind("#nano#<CTRL>M%Move up", [&]() { bindMoveUp(); }, "Move Actor/Signal backward");
    backend->bind("#nice#.Move.Up", [&]() { bindMoveUp(); }, "Move Actor/Signal backward");
    
    // Move down
    // <CTRL> + M
    backend->bind("#nano#<CTRL>m%Move down", [&]() { bindMoveDown(); }, "Move Actor/Signal forward");
    backend->bind("#nice#.Move.Down", [&]() { bindMoveDown(); }, "Move Actor/Signal forward");
    
    
    
    // New file
    // <CTRL> + N
    backend->bind("#nano#<CTRL>n%New", [&]() { bindNew(); }, "Create new file");
    backend->bind("#nice#.File.New", [&]() { bindNew(); }, "Create new file");
    
    // Open a file
    // <CTRL> + O
    backend->bind("#nano#<CTRL>o%Save!Filename${FILENAME}", [&]() { bindOpen(); }, "Open a file");
    backend->bind("#nice#.File.Open${Filename: |FILENAME}", [&]() { bindOpen(); }, "Open a file");
    
    // Save a file
    // <CTRL> + S
    backend->bind("#nano#<CTRL>s%Save", [&]() { bindSave(); }, "Save data to existing file");
    backend->bind("#nice#.File.Save", [&]() { bindSave(); }, "Save data to existing file");
    
    // Save a file as...
    // <CTRL> + <SHIFT> + S
    backend->bind("#nano#<CTRL><SHIFT>S%Save as!Filename${FILENAME}", [&]() { bindSaveAs(); }, "Save data to a new file");
    backend->bind("#nice#.File.Save as${Filename: |FILENAME}", [&]() { bindSaveAs(); }, "Save data to a new file");
    
            
}

//
// MARK: Handling keys
//
void SequenceDiagram::handleKey()
                 {
    string s = getEntry("KEY");
    
    // Arrow left
    if(s == "<LARROW>") {
        if(state.mode == Mode::Actors ||
           state.mode == Mode::NewSignalSource ||
           state.mode == Mode::NewSignalDestination) previousActor();
        else panLeft();
    }
    
    // Arrow right
    else if(s == "<RARROW>") {
        if(state.mode == Mode::Actors ||
           state.mode == Mode::NewSignalSource ||
           state.mode == Mode::NewSignalDestination)  nextActor();
        else panRight();
    }
    
    // Arrow up
    else if(s == "<UARROW>") {
        if(state.mode == Mode::Signals) previousSignal();
        else panUp();
    }
    
    // Arrow down
    else if(s == "<DARROW>") {
        if(state.mode == Mode::Signals) nextSignal();
        else panDown();
    }
    
    // Actors mode
    else if(s == "a") {
        if(state.mode == Mode::Signals) actorsMode();
    }
    
    // Signals mode
    else if(s == "s") {
        if(state.mode == Mode::Actors) signalsMode();
    }
    
    // Signal creator - next
    else if(s == "n") {
        switch(state.mode) {
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
    }
    
    // Signal creator - cancel
    else if(s == "q") {
        if(state.mode == Mode::NewSignalSource || state.mode == Mode::NewSignalDestination) {
            creator->cancel();
            draw();
        }
    }
    
}

/// TODO: Delete
void SequenceDiagram::handleKey(int key) {
    
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
}
