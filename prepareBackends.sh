#!/bin/bash

# nice
sed -i.bak '73d' umlSequence/backend-nice/nice-like.cpp
sed -i.bak '74i\
tool->redraw();\
' umlSequence/backend-nice/nice-like.cpp

sed -i.bak '89,90d' umlSequence/backend-nice/nice-like.cpp

sed -i.bak '89i\
string p(1, c);\
tool->setEntry("KEY", p);\
' umlSequence/backend-nice/nice-like.cpp

# nano
rm umlSequence/backend-nano/main.cpp
rm umlSequence/backend-nano/main
sed -i.bak '6i\
#include <string>\
' umlSequence/backend-nano/shared.hpp

