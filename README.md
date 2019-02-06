# CWaggle
CWaggle Robot Simulator

See main.cpp for brief tutorial

Run this command from the cwaggle/bin directory:

`g++ ../src/example/main.cpp -I../include/ -std=c++14 -o ./cwaggle -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -Wall`

`./cwaggle N`

Where N is the number of simulation ticks per GUI render (higher means faster)

Inspired by the JS Robot simulator 'Waggle' by Andrew Vardy
