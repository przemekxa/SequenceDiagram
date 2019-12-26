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
    unsigned int id;
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
    unsigned int id;
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



const Document EXAMPLE_DOCUMENT {
    {
        {0, ActorType::Player, "First"},
        {1, ActorType::Object, "Second"},
        {2, ActorType::Object, "Third"},
        {3, ActorType::Player, "Fourth"},
        {4, ActorType::Object, "Fifth"},
        {5, ActorType::Object, "Sixth"},
        {6, ActorType::Object, "Seventh"}
    },
    {
        {0, SignalType::Changing, "0 to 1", 0, 1},
        {1, SignalType::Informing, "1 to 2 tr", 1, 2},
        {2, SignalType::Changing, "Something", 3, 1},
        {3, SignalType::Changing, "Else", 4, 6},
        {4, SignalType::Changing, "ATM machine", 3, 5},
        {5, SignalType::Informing, "Working", 5, 0},
        {6, SignalType::Informing, "Inform", 2, 5}
    }
};

#endif
