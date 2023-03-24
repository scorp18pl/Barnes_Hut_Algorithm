#include <SFML/System.hpp>
#include <Simulation.h>
#include <filesystem>

int main()
{
    Simulation::GetSimulation().Start();
    return 0;
}