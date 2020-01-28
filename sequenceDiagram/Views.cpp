//
//  Views.cpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#include "Views.hpp"

void printSpaces(WINDOW* w, int posY, int posX, int len) {
    for(int i = 0; i < len; i++) {
        mvwaddch(w, posY, posX + i, ' ');
    }
}

void printCenter(WINDOW* w, const char * text, int posY, int posX, int len) {
    int padding = (len - (int)strlen(text)) / 2;
    mvwaddnstr(w, posY, posX + padding, text, len);
}


// MARK: - Status bar

StatusBarView::StatusBarView(Document* d, DocumentState* s) {
    doc = d;
    state = s;
    
    w = newwin(1, getmaxx(stdscr), getmaxy(stdscr) - 2, 0);
    wstandout(w);
}

StatusBarView::~StatusBarView() {
    delwin(w);
}

void StatusBarView::draw() {
    wclear(w);
    int width = getmaxx(stdscr);
    printSpaces(w, 0, 0, width);
            
    switch(state->mode) {
        case Mode::Actors:
            
            if(status != "") {
                mvwprintw(w, 0, 1, status.c_str());
            } else if(!doc->actors.empty()) {
                mvwprintw(w, 0, 1, doc->actors[state->selectedActor].name.c_str());
            }
            mvwprintw(w, 0, width - 16, "MODE: Actors");
            break;
            
        case Mode::Signals: {
            
            if(status != "") {
                mvwprintw(w, 0, 1, status.c_str());
            } else if(!doc->signals.empty()) {
                Signal& s = doc->signals[state->selectedSignal];
                string t = s.name;
                t += " [" + doc->actors[s.source].name;
                if(s.type == SignalType::Changing)
                    t += " => ";
                else
                    t += " -> ";
                t += doc->actors[s.destination].name + "]";
                mvwprintw(w, 0, 1, t.c_str());
            }
            
            mvwprintw(w, 0, width - 16, "MODE: Signals");
            break;
        }
        case Mode::NewSignalSource: {
            
            if(status != "") {
                mvwprintw(w, 0, 1, status.c_str());
            } else {
                mvwprintw(w, 0, 1, "Select SOURCE and press [n] ([q] to cancel)");
            }
            mvwprintw(w, 0, width - 16, "MODE: Signals+");
            
            break;
        }
        case Mode::NewSignalDestination: {
            
            if(status != "") {
                mvwprintw(w, 0, 1, status.c_str());
            } else {
                mvwprintw(w, 0, 1, "Select DESTINATION and press [n] ([q] to cancel)");
            }
            mvwprintw(w, 0, width - 16, "MODE: Signals+");
            
            break;
        }
        default: break;
    }
    wrefresh(w);
}

// MARK: Status

void StatusBarView::setStatus(string status) {
    this->status = status;
}

void StatusBarView::resetStatus() {
    this->status = "";
}



// MARK: - Border

BorderView::BorderView(DocumentState* s) {
    state = s;
    w = newwin(getmaxy(stdscr) - 3, getmaxx(stdscr), 1, 0);
}

BorderView::~BorderView() {
    delwin(w);
}

void BorderView::draw() {
    wclear(w);
    box(w, 0, 0);
    mvwhline(w, 6, 1, '-', getmaxx(w) - 2);
    
    string name = state->documentName;
    if(state->changed) name += "*";
    
    printCenter(w, name.c_str(), 0, 0, getmaxx(stdscr) - 2);
    
    wrefresh(w);
}





// MARK: - Actors

ActorsView::ActorsView(Document* d, DocumentState* s) {
    
    doc = d;
    state = s;
    
    w = newpad(5, (int)doc->actors.size() * ActorLength);
    //prefresh(w, 0 + state->marginH, 0, 2, 1, 6, getmaxx(stdscr) - 2);
    draw();
}

ActorsView::~ActorsView() {
    delwin(w);
}

void ActorsView::adjustMarginsForActor(int id) {
    
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

void ActorsView::draw() {
            
    for(int i = 0; i < doc->actors.size(); i++) {
        
        if(i == state->selectedActor && (
                                         state->mode == Mode::Actors ||
                                         state->mode == Mode::NewSignalSource ||
                                         state->mode == Mode::NewSignalDestination
                                         )
           ) wattron(w, A_STANDOUT);
        
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
        
        if(i == state->selectedActor && (
                                      state->mode == Mode::Actors ||
                                      state->mode == Mode::NewSignalSource ||
                                      state->mode == Mode::NewSignalDestination
                                      )
        ) wattroff(w, A_STANDOUT);
        
        
    }
    
    prefresh(w, 0, 0 + state->marginH, 2, 1, 6, getmaxx(stdscr) - 2);
    
}




// MARK: - Signals



void SignalsView::redrawActorLines() {
        
    // Thin lines
    for(int i = 0; i < doc->actors.size(); i++) {
        mvwvline(w, 0, ActorLength * i + (ActorLength / 2), '|', SignalLength * doc->signals.size());
    }
    
}


SignalsView::SignalsView(Document* d, DocumentState* s) {
    
    doc = d;
    state = s;
    
    w = newpad((int)doc->signals.size() * SignalLength, (int)doc->actors.size() * ActorLength);
    
    draw();
}

void SignalsView::adjustMarginsForSignal(int id) {
    
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


void SignalsView::draw() {
    
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
        else if(s->destination < s->source) {
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
        
        // Signal to self
        else {
            source = (s->source * ActorLength);
            dest = ActorLength / 2;
            
            printSpaces(w, SignalLength * i, source, ActorLength);
            printSpaces(w, SignalLength * i + 1, source, ActorLength);
            printSpaces(w, SignalLength * i + 2, source, ActorLength);
            printCenter(w, s->name.c_str(), SignalLength * i, source, ActorLength);
            
            char ch = s->type == SignalType::Changing ? '=' : '-';
            
            mvwhline(w, SignalLength * i + 1, source + dest + 1, ch, dest - 2);
            mvwhline(w, SignalLength * i + 2, source + dest + 1, ch, dest - 2);
            
            mvwaddch(w, SignalLength * i + 1, source + dest, '|');
            mvwaddch(w, SignalLength * i + 2, source + dest, '|');
            mvwaddch(w, SignalLength * i + 2, source + dest + 1, '<');
            mvwaddch(w, SignalLength * i + 1, source + 2 * dest - 1, '+');
            mvwaddch(w, SignalLength * i + 2, source + 2 * dest - 1, '+');
            
            //mvwaddch(w, SignalLength * i + 2, source, '<');
            
        }
        
        if(i == state->selectedSignal && state->mode == Mode::Signals) wattroff(w, A_STANDOUT);
        
    }
    
    prefresh(w, 0 + state->marginV, 0 + state->marginH, 8, 1, getmaxy(stdscr) - 4, getmaxx(stdscr) - 2);
    
}
