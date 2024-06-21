#!/bin/zsh
cmake -S ./src -B ./build
cd build
make
./matching-engine ../tests/BUY-GFD.txt
