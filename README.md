# CWaggle
CWaggle Robot Simulator

CWaggle requires the SFML library: https://www.sfml-dev.org/

Run `make` from the cwaggle root directory to build 3 demo executables in the `cwaggle/bin` directory:

- `cwaggle_example` - drag circles / lines around to demonstrate physics
- `cwaggle_orbital` - demo of Andrew Vardy's orbital construction algorithm
- `cwaggle_rl` - demo of our reinforcement learning orbital construction algorithm

Run either program to see a demo of the cwaggle system

If you want to run the make command from the `cwaggle/bin` directory, you can type `make -C ..` to specify that the Makefile is one directory up from the current location

Inspired by the JS Robot simulator 'Waggle' by Andrew Vardy
