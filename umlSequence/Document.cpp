//
//  Document.cpp
//  umlSequence
//
//  Created by Przemek Ambroży on 16/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
//

#include "Document.hpp"
#include <iostream>
#include <algorithm>


// Actor

void to_json(json& j, const Actor& a) {
    j = json {
        {"id", a.id},
        {"name", a.name},
        {"type", a.type}
    };
};

void from_json(const json& j, Actor& a) {
    j.at("id").get_to(a.id);
    j.at("name").get_to(a.name);
    j.at("type").get_to(a.type);
};

bool operator <(const Actor& a, const Actor& b) {
    return a.id < b.id;
}


// Signal

void to_json(json& j, const Signal& s) {
    j = json {
        {"id", s.id},
        {"name", s.name},
        {"type", s.type},
        {"source", s.source},
        {"destination", s.destination}
    };
};

void from_json(const json& j, Signal& s) {
    j.at("id").get_to(s.id);
    j.at("name").get_to(s.name);
    j.at("type").get_to(s.type);
    j.at("source").get_to(s.source);
    j.at("destination").get_to(s.destination);
};

bool operator <(const Signal& a, const Signal& b) {
    return a.id < b.id;
}


// Document

void to_json(json& j, const Document& d) {
    j = json {
        {"actors", d.actors},
        {"signals", d.signals}
    };
};

void from_json(const json& j, Document& d) {
    j.at("actors").get_to(d.actors);
    sort(d.actors.begin(), d.actors.end());
    j.at("signals").get_to(d.signals);
    sort(d.signals.begin(), d.signals.end());
};

