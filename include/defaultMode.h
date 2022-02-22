#pragma once
#include <Gui.h>


//Extends Gui class
static class DefaultMode : public Gui {
public:
	

	//The controller and container is a needed to access the simulation
	DefaultMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(120, 183, 196), controller, container, { 309.0f,0.4 * WINDOW_HEIGHT,1 }) {

		name = "Default Mode";
		help_text += "Currently the simulation is in default mode. This means\n\n"
			"that no variables will be kept constant and you are free to\n\n"
			"change anything you like.";
	}
protected:


};
