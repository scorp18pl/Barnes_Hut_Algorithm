#include <SFML/System.hpp>
#include <Simulation.h>

int main()
{
    std::cout << "Please enter the entity count." << std::endl;
    std::cout << "For good performance this number should be less than 5000."
              << std::endl;
    unsigned long ecount;
    std::cin >> ecount;

    Simulation sim = Simulation(ecount);
    sim.Start();
    return 0;
}