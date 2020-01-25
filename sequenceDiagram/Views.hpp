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

class View {
protected:
    WINDOW* w;
    DocumentState* state;
public:
    virtual void draw() = 0;
};


// MARK: - Status bar

class StatusBarView: public View {
private:
    Document* doc;
public:
    
    StatusBarView(Document* d, DocumentState* s);
    ~StatusBarView();
    
    //void setStatus(string status);
    void draw();
};




// MARK: - Border

class BorderView: public View {
public:
    
    BorderView(DocumentState* s);
    ~BorderView();
    
    void draw();
};




// MARK: - Actors

class ActorsView: public View {
private:
    Document* doc;
public:
    
    ActorsView(Document* d, DocumentState* s);
    ~ActorsView();
    
    void adjustMarginsForActor(int id);
    void draw();
    
};




// MARK: - Signals


class SignalsView: public View {
private:
    Document* doc;
    
    void redrawActorLines();
    
public:
    
    SignalsView(Document* d, DocumentState* s);
    
    void adjustMarginsForSignal(int id);
    void draw();
};

#endif /* Views_hpp */
