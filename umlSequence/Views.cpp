//
//  Views.cpp
//  umlSequence
//
//  Created by Przemek Ambroży on 27/12/2019.
//  Copyright © 2019 Przemek Ambroży. All rights reserved.
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
