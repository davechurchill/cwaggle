// to compile without using SFML, remove GUI imports
// GUI is the only class that references SFML
#include "GUI.hpp"

// allows us to use the WorldState / simulation
#include "World.hpp"
#include "Simulator.hpp"

#include <iostream>

void benchmark()
{
    auto world = ExampleWorlds::GetGridWorld720(2);
    Simulator sim(world);
    for (size_t i = 0; i < 100000; i++)
    {
        if (i % 1000 == 0) { std::cout << i << "\n"; }
        sim.update();
    }
}

int main()
{
    // set up a new world that will be used for our simulation
    // let's pull one from the ExampleWorlds
    auto world = ExampleWorlds::GetGridWorld720(2);

    // create a new simulator with the given world
    Simulator sim(world);

    // set up a GUI to visualize the simulation with a given frame rate limit
    // the frame rate limit should be set at least as high as your monitor refresh rate
    // this is completely optional, simulation will run with no visualization
    // GUI can also be created at any time to start visualizing an ongoing simulation
    GUI gui(sim, 60);

    // run the simulation and gui update() function in a loop

    while (true)
    {
        // call the world physics simulation update
        sim.update();

        // if a gui exists, call for its display to update
        // note: simulation is limited by gui frame rate limit
        gui.update();
    }
}