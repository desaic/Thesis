#!/bin/bash
bison -d --defines=include/parser.hpp -o src/parser.cpp src/parser.y
flex -o src/tokens.cpp src/tokens.l include/parser.hpp

