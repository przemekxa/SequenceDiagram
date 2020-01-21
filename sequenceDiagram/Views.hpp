//
//  Views.hpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#ifndef Views_hpp
#define Views_hpp

#include <stdio.h>
#include <ncurses.h>
#include "Document.hpp"

using namespace std;

const int ActorLength = 21;
const int SignalLength = 4;


// MARK: - Status bar

class StatusBarView {
private:
    WINDOW* w;
    Document* doc;
    DocumentState* state;
public:
    
    StatusBarView(Document* d, DocumentState* s);
    ~StatusBarView();
    
    //void setStatus(string status);
    void draw();
};




// MARK: - Border

class BorderView {
private:
    WINDOW* w;
    DocumentState* state;
public:
    
    BorderView(DocumentState* s);
    ~BorderView();
    
    void draw();
};




// MARK: - Actors

class ActorsView {
private:
    WINDOW *w;
    Document* doc;
    DocumentState* state;
    
public:
    
    ActorsView(Document* d, DocumentState* s);
    ~ActorsView();
    
    void adjustMarginsForActor(int id);
    void draw();
    
};




// MARK: - Signals


class SignalsView {
private:
    WINDOW *w;
    Document* doc;
    DocumentState* state;
    
    void redrawActorLines();
    
public:
    
    SignalsView(Document* d, DocumentState* s);
    
    void adjustMarginsForSignal(int id);
    void draw();
};

#endif /* Views_hpp */
