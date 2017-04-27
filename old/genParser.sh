#!/bin/bash
GRAMMAR_DIR=grammar
bison -d --defines=include/parser.hpp -o \
  src/parser.cpp ${GRAMMAR_DIR}/parser.y --report=$1
flex -o src/tokens.cpp \
  ${GRAMMAR_DIR}/tokens.l include/parser.hpp

