#include "CWaggle.h"


class EntityController_OrbitalConstruction : public EntityController
{
    std::shared_ptr<World> m_world;
    Entity          m_robot;
    SensorReading   m_reading;
    double          m_threshold[2] = { 0.65, 0.8 };

public:

    EntityController_OrbitalConstruction(Entity robot, std::shared_ptr<World> world)
        : m_world(world)
        , m_robot(robot)
    {

    }

    virtual EntityAction getAction()
    {
        // read the sensors and store it in m_reading
        SensorTools::ReadSensorArray(m_robot, m_world, m_reading);

        const double MaxAngularSpeed = 0.3;
        const double ForwardSpeed = 2;

        if (m_reading.leftObstacle > 0)
        {
            m_previousAction = EntityAction(ForwardSpeed, MaxAngularSpeed);
            return m_previousAction;
        }

        size_t type = m_robot.getComponent<CRobotType>().type;
        bool innie = type == 1;

        if (m_reading.rightNest >= m_reading.midNest && m_reading.midNest >= m_reading.leftNest)
        {
            // The gradient is in the desired orientation with the highest
            // sensed value to the right, then the centre value in the middle,
            // followed by the lowest on the left.

            // These conditions steer in (for an innie) and out (for an outie)
            // to nudge a puck inwards or outwards.
            if (innie && m_reading.rightPucks > 0)
            {
                m_previousAction = EntityAction(ForwardSpeed, MaxAngularSpeed);
                return m_previousAction;
            }
            else if (!innie && m_reading.leftPucks > 0)
            {
                m_previousAction = EntityAction(ForwardSpeed, -MaxAngularSpeed);
                return m_previousAction;
            }

            // We now act to maintain the centre value at the desired isoline.
            if (m_reading.midNest < m_threshold[type])
            {
                m_previousAction = EntityAction(ForwardSpeed, 0.3 * MaxAngularSpeed);
                return m_previousAction;
            }
            else
            {
                m_previousAction = EntityAction(ForwardSpeed, -0.3 * MaxAngularSpeed);
                return m_previousAction;
            }
        }
        else if (m_reading.midNest >= m_reading.rightNest && m_reading.midNest >= m_reading.leftNest)
        {
            // We are heading uphill of the gradient, turn left to return to a
            // clockwise orbit.
            m_previousAction = EntityAction(ForwardSpeed, -MaxAngularSpeed);
            return m_previousAction;
        }
        else
        {
            // We are heading downhill, turn right to return to clockwise orbit.
            m_previousAction = EntityAction(ForwardSpeed, MaxAngularSpeed);
            return m_previousAction;
        }
    }
};


void OrbitalConstructionExample(int argc, char ** argv)
{
    // set up a new world that will be used for our simulation
    // let's pull one from the ExampleWorlds
    //auto world = ExampleWorlds::GetGridWorld720(2);
    auto world = ExampleWorlds::GetGetSquareWorld(800, 800, 20, 10, 250, 10);

    // add orbital controllers to all the robots
    for (auto e : world->getEntities("robot"))
    {
        e.addComponent<CController>(std::make_shared<EntityController_OrbitalConstruction>(e, world));
    }

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
    // Robots guided by orbital construction algorithm
    OrbitalConstructionExample(argc, argv);

    return 0;
}