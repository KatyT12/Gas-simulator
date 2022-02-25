#pragma once
#include <Gui.h>
#include <Container.h>

//Extends Gui class
static class BoylesMode : public Gui {
public:

	
	//The controller and container is a needed to access the simulation
	BoylesMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(27, 211, 131), controller, container, { 507,580,150,0,0 }) {


		name = "Boyles-Law  Mode";
		help_text += "The current mode presents how Boyles law works "
			"observe how\n\npressure changes as you change the volume"
			" of the container.\n\nThis change in pressure will be more"
			" apparent as the number\n\nof particles increase.\n\n\n\n"
			"The relationship between pressure and volume is inversely\n\n"
			"proportional. The equation PV = k shows this where k is a \n\nconstant";
		const_variable = CONSTANT::TEMPERATURE;

	}
};
