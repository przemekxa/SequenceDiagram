//
//  Document.hpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#ifndef Document_hpp
#define Document_hpp

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
    Signals,
    NewSignalSource,
    NewSignalDestination
};

struct DocumentState {
    Mode mode;
    string documentName;
    bool changed = false;
    unsigned int selectedActor = 0;
    unsigned int selectedSignal = 0;
    int marginH = 0;
    int marginV = 0;
};


const Document EXAMPLE_DOCUMENT {
    {
        {ActorType::Player, "User"},
        {ActorType::Object, "ATM Machine"},
        {ActorType::Object, "Bank Server"}
    },
    {
        {SignalType::Changing, "Insert card", 0, 1},
        {SignalType::Informing, "Request PIN", 1, 0},
        {SignalType::Changing, "Insert PIN", 0, 1},
        {SignalType::Changing, "Verify PIN", 1, 2},
        {SignalType::Informing, "PIN Verified", 2, 1},
        {SignalType::Informing, "Show options", 1, 0},
        {SignalType::Changing, "Choose option", 0, 1},
        {SignalType::Changing, "Get balance", 1, 2},
        {SignalType::Informing, "Balance", 2, 1},
        {SignalType::Informing, "Show balance", 1, 0},
        {SignalType::Informing, "Print receipt", 1, 0},
        {SignalType::Changing, "Card removed", 0, 1},
        {SignalType::Informing, "\"Thank You\"", 1, 0},
    }
};

#endif
