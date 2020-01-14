//
//  Views.hpp
//  umlSequence
//
//  Created by Przemek Ambroży on 27/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#ifndef Views_hpp
#define Views_hpp

#include <stdio.h>
#include <curses.h>
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
