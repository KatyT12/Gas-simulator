#pragma once
#include <Gui.h>
#include <Container.h>

//Extends Gui class
static class PressureMode : public Gui {
public:


	//The controller and container is a needed to access the simulation
	PressureMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(224, 96, 96), controller, container, { 309,300,100 }) {


		name = "Pressure-Law  Mode";
		help_text += "The current mode is the Pressure-law mode. The pressure law\n\n"
			"states that as temperature increases, pressure will also\n\nincrease "
			"at a constant volume. You can test this by changing\n\nthe temperature.\n\n\n\n"
			
			"This is because as temperature increases, the average kinetic\n\nenergy "
			"of the particles increase, this means more collisions\n\nwill happen"
			"in the container and particles will collide at\n\nhigher velocities."
			" When particles collide with the container\n\nat higher velocities they"
			" will have a higher momentum and\n\ntherefore create a larger force on "
			"the container.";
		
		
		const_variable = CONSTANT::VOLUME;

	}

};
