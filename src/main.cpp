// to compile without using SFML, remove GUI imports
// GUI is the only class that references SFML
#include "GUI.hpp"

// allows us to use the WorldState / simulation
#include "Simulator.hpp"

int main()
{
    // set up a new simulator with a given width and height
    // for now it has an init() function which adds some objects
    Simulator sim(1920, 1080);

    // set up a GUI to visualize the simulation with a world and frame rate limit
    // ideally the frame rate limit should be set to your monitor refresh rate
    // this is completely optional, simulation will run with no visualization
    // GUI can be created at any time to start visualizing an ongoing simulation
    GUI gui(sim, 144);

    // run the simulation update() function in a loop
    while (true)
    {
        // call the world physics simulation update
        sim.update();

        // if a gui exists, call for its display to update
        // note: simulation is limited by gui frame rate limit
        gui.update();
    }
}