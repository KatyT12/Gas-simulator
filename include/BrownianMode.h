#pragma once
#include <Gui.h>
#include <Container.h>

//Extends Gui class
static class BrownianMode : public Gui {
public:


	//The controller and container is a needed to access the simulation
	BrownianMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(219, 213,129), controller, container, { 500,400,199,0,1 }) {


		name = "Brownian-Motion  Mode";
		help_text += "The current mode is the Brownian-Motion mode. Brownian motion\n\n"
			"is the principle that particles in fluids will move randomly\n\nThis is "
			"because they are bombarded with other particles\n\nin the container.\n\n\n\n"

			"This can be observed by the large particle in the container\n\nbeing "
			"moved in random directions from the collisions\n\nof smaller "
			"particles around it.\n\n";


		const_variable = CONSTANT::PARTICLE;

	}

};
