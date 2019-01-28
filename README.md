# CWaggle
CWaggle Robot Simulator

See main.cpp for brief tutorial

Run this command from the cwaggle/bin directory:

`g++ ../src/*.cpp -O3 -Isrc -std=c++14 -o ./cwaggle -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./cwaggle N`

Where N is the number of simulation ticks per GUI render (higher means faster)

Inspired by the JS Robot simulator 'Waggle' by Andrew Vardy
