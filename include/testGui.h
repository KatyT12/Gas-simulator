#pragma once
#include <Gui.h>


//Extends Gui class
static class testMode : public Gui {
public:
	//The controller is a needed to access the simulation
	testMode(ParticleController* controller) : Gui(olc::BLUE, controller) {

		addButton(Button({ 600, 100 }, { 200,50 }, olc::WHITE, "ADD"));  //Add a new button which adds a particle to the simulation

		buttons[0].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::LIGHT); }); //Set the function of the button
		
	}
};
