#define OLC_PGE_APPLICATION //A macro that needs to be defined to use the PixelGameEngine class
#include <Simulation.h>

int main()
{
	Simulation simulation; //Initialize the simulation
	if (simulation.Construct(WINDOW_WIDTH, WINDOW_HEIGHT, 1, 1)) //Will return false if it fails to create an instance 
		simulation.Start(); // Starts the simulation

	return 0;
}

/*
* Changes to document:
* Particle.cpp
* Temperature and energy methods
* Possibly change private and public methods
* srand in constructor (possible testing error)
*/


