// to compile without using SFML, remove GUI imports
// GUI is the only class that references SFML
#include "GUI.hpp"

// allows us to use the WorldState / simulation
#include "World.hpp"
#include "Simulator.hpp"
#include "RobotControllers.hpp"

#include <iostream>


int main()
{
    // set up a new world that will be used for our simulation
    // let's pull one from the ExampleWorlds
    //auto world = ExampleWorlds::GetGridWorld1080(6);
    auto world = ExampleWorlds::GetGetSquareWorld(Vec2(800, 800), 20, 10, 250, 10);

    // create a new simulator with the given world
    Simulator sim(world);

    // set up a GUI to visualize the simulation with a given frame rate limit
    // the frame rate limit should be set at least as high as your monitor refresh rate
    // this is completely optional, simulation will run with no visualization
    // GUI can also be created at any time to start visualizing an ongoing simulation
    GUI gui(sim, 144);

    // run the simulation and gui update() function in a loop
    while (true)
    {
        // un-comment to update the robots with a sample controller
         /*for (auto & robot : sim.getWorld().getRobots())
         {
             if (robot.type() == RobotTypes::Outie)
             {
                 robot.doAction(RobotControllers::TurnController(1, 0.03));
             }
             else if (robot.type() == RobotTypes::Innie)
             {
                 robot.doAction(RobotControllers::TurnController(1, -0.03));
             }
            
         }*/

        // call the world physics simulation update
        // parameter = how much sim time should pass (default 1.0)
        // the smaller the time update, the more accurate/smooth the simulation
        for (size_t i = 0; i < 10; i++)
        {
            sim.update(0.1);
        }
        
        // if a gui exists, call for its display to update
        // note: simulation is limited by gui frame rate limit
        gui.update();
    }
}