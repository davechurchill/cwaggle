// to compile without using SFML, remove GUI imports
// GUI is the only class that references SFML
#include "GUI.hpp"

// allows us to use the WorldState / simulation
#include "World.hpp"
#include "Simulator.hpp"
#include "EntityControllers.hpp"
#include "RLExperiment.hpp"

void PhysicsPlayExample(int argc, char ** argv)
{
    // set up a new world that will be used for our simulation
    // let's pull one from the ExampleWorlds
    //auto world = ExampleWorlds::GetGridWorld720(2);
    auto world = ExampleWorlds::GetGridWorld720(2);

    // create a new simulator with the given world
    auto simulator = std::make_shared<Simulator>(world);

    // set up a GUI to visualize the simulation with a given frame rate limit
    // the frame rate limit should be set at least as high as your monitor refresh rate
    // this is completely optional, simulation will run with no visualization
    // GUI can also be created at any time to start visualizing an ongoing simulation
    GUI gui(simulator, 144);

    // determines the amount of 'time' that passes per simulation tick
    // lower is more 'accurate', but 'slower' to get to a given time
    double simulationTimeStep = 1.0;

    // how many simulation ticks are peformed before each world render in the GUI
    double stepsPerRender = 1;

    // read that value from console if it exists
    if (argc == 2)
    {
        std::stringstream ss(argv[1]);
        ss >> stepsPerRender;
    }

    // run the simulation and gui update() function in a loop
    while (true)
    {
        for (size_t i = 0; i < stepsPerRender; i++)
        {
            // call the world physics simulation update
            // parameter = how much sim time should pass (default 1.0)
            simulator->update(simulationTimeStep);
        }

        // if a gui exists, call for its display to update
        // note: simulation is limited by gui frame rate limit
        gui.update();
    }
}

void OrbitalConstructionExample(int argc, char ** argv)
{
    // set up a new world that will be used for our simulation
    // let's pull one from the ExampleWorlds
    //auto world = ExampleWorlds::GetGridWorld720(2);
    auto world = ExampleWorlds::GetGetSquareWorld(800, 800, 20, 10, 250, 10);

    // create a new simulator with the given world
    auto simulator = std::make_shared<Simulator>(world);

    // set up a GUI to visualize the simulation with a given frame rate limit
    // the frame rate limit should be set at least as high as your monitor refresh rate
    // this is completely optional, simulation will run with no visualization
    // GUI can also be created at any time to start visualizing an ongoing simulation
    GUI gui(simulator, 144);

    // determines the amount of 'time' that passes per simulation tick
    // lower is more 'accurate', but 'slower' to get to a given time
    double simulationTimeStep = 1.0;

    // how many simulation ticks are peformed before each world render in the GUI
    double stepsPerRender = 1;

    // read that value from console if it exists
    if (argc == 2)
    {
        std::stringstream ss(argv[1]);
        ss >> stepsPerRender;
    }

    // run the simulation and gui update() function in a loop
    while (true)
    {
        for (size_t i = 0; i < stepsPerRender; i++)
        {
            // un-comment to update the robots with a sample controller
            for (auto & robot : simulator->getWorld()->getEntities("robot"))
            {
                // if the entity doesn't have a controller we can skip it
                if (!robot.hasComponent<CController>()) { continue; }

                // get the action that should be done for this entity
                EntityAction action = robot.getComponent<CController>().controller->getAction();

                // have the action apply its effects to the entity
                action.doAction(robot, simulationTimeStep);
            }

            // call the world physics simulation update
            // parameter = how much sim time should pass (default 1.0)
            simulator->update(simulationTimeStep);
        }

        // if a gui exists, call for its display to update
        // note: simulation is limited by gui frame rate limit
        gui.update();
    }
}

int main(int argc, char ** argv)
{
    // Drag around large robots that puck around pucks
    PhysicsPlayExample(argc, argv);

    // Robots guided by orbital construction algorithm
    // OrbitalConstructionExample(argc, argv);

    // Do a reinforcement learning experiment
    // RLExperiments::MainRLExperiment();

    return 0;
}