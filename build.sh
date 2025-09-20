#!/bin/bash

g++ -o cli cli.cpp || clang++ -o cli cli.cpp

if [ $? -eq 0 ]; then
    ./cli
else
    echo "Compilation failed."
    exit 1
fi