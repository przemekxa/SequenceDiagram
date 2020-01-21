#!/bin/bash

# nice
sed -i.bak '79d' sequenceDiagram/backend-nice/nice-like.cpp
sed -i.bak '80i\
tool->redraw();\
' sequenceDiagram/backend-nice/nice-like.cpp

#sed -i.bak '89,90d' sequenceDiagram/backend-nice/nice-like.cpp

#sed -i.bak '89i\
#string p(1, c);\
#tool->setEntry("KEY", p);\
#' sequenceDiagram/backend-nice/nice-like.cpp

# nano
rm sequenceDiagram/backend-nano/main.cpp
rm sequenceDiagram/backend-nano/main
sed -i.bak '6i\
#include <string>\
' sequenceDiagram/backend-nano/shared.hpp

