#!/bin/bash

if [[ $OSTYPE == darwin* ]]; then
antlr -Dlanguage=Cpp -no-listener -visitor -o antlr4-runtime fluenc.g4
else
antlr4 -Dlanguage=Cpp -no-listener -visitor -o antlr4-runtime fluenc.g4
fi
