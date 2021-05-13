#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Simulation.h"


int main() {
	Simulation sim = Simulation(100ul);
	sim.start();
	return 0;
}