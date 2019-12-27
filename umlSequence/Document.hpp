//
//  Document.hpp
//  umlSequence
//
//  Created by Przemek Ambroży on 16/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#ifndef Document_hpp
#define Document_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "json.hpp"

using namespace std;
using nlohmann::json;


enum class ActorType {
    Player,
    Object
};

struct Actor {
    ActorType type;
    string name;
};
void to_json(json& j, const Actor& a);
void from_json(const json& j, Actor& a);
bool operator <(const Actor& a, const Actor& b);


enum class SignalType {
    Changing,
    Informing
};

struct Signal {
    SignalType type;
    string name;
    unsigned int source;
    unsigned int destination;
};
void to_json(json& j, const Signal& s);
void from_json(const json& j, Signal& s);
bool operator <(const Signal& a, const Signal& b);



struct Document {
    vector<Actor> actors;
    vector<Signal> signals;
};
void to_json(json& j, const Document& d);
void from_json(const json& j, Document& d);


enum class Mode {
    Actors,
    Signals
};

struct DocumentState {
    Mode mode;
    string documentName;
    bool changed;
    unsigned int selectedActor;
    unsigned int selectedSignal;
    int marginH;
    int marginV;
};


const Document EXAMPLE_DOCUMENT {
    {
        {ActorType::Player, "First"},
        {ActorType::Object, "Second"},
        {ActorType::Object, "Third"},
        {ActorType::Player, "Fourth"},
        {ActorType::Object, "Fifth"},
        {ActorType::Object, "Sixth"},
        {ActorType::Object, "Seventh"}
    },
    {
        {SignalType::Changing, "0 to 1", 0, 1},
        {SignalType::Informing, "1 to 2 tr", 1, 2},
        {SignalType::Changing, "Something", 3, 1},
        {SignalType::Changing, "Else", 4, 6},
        {SignalType::Changing, "ATM machine", 3, 5},
        {SignalType::Informing, "Working", 5, 0},
        {SignalType::Informing, "Inform", 2, 5}
    }
};

#endif
