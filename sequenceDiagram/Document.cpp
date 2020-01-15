//
//  Document.cpp
//  UML Sequence Diagram
//
//  Przemysław Ambroży
//

#include "Document.hpp"

// Actor

void to_json(json& j, const Actor& a) {
    j = json {
        {"name", a.name},
        {"type", a.type}
    };
};

void from_json(const json& j, Actor& a) {
    j.at("name").get_to(a.name);
    j.at("type").get_to(a.type);
};


// Signal

void to_json(json& j, const Signal& s) {
    j = json {
        {"name", s.name},
        {"type", s.type},
        {"source", s.source},
        {"destination", s.destination}
    };
};

void from_json(const json& j, Signal& s) {
    j.at("name").get_to(s.name);
    j.at("type").get_to(s.type);
    j.at("source").get_to(s.source);
    j.at("destination").get_to(s.destination);
};


// Document

void to_json(json& j, const Document& d) {
    j = json {
        {"actors", d.actors},
        {"signals", d.signals}
    };
};

void from_json(const json& j, Document& d) {
    j.at("actors").get_to(d.actors);
    j.at("signals").get_to(d.signals);
};

