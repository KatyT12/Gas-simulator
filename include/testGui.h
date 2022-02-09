#pragma once
#include <Gui.h>


//Extends Gui class
static class testMode : public Gui {
public:
	
	//The controller and container is a needed to access the simulation
	testMode(ParticleController* controller, Container* container) : Gui(olc::BLUE, controller,container) {

		addButton(Button({ 600, 100 }, { 200,50 }, olc::WHITE, "ADD"));  //Add a new button which adds a particle to the simulation

		buttons[0].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::LIGHT); }); //Set the function of the button
		
		//Add the reset button
		addButton(Button({ 900,100 }, { 200,50 }, olc::WHITE, "RESET"));
		//Define the state to reset to: temperature of 30, height of 0.4* window width and 1 particle in the simulation.
		State Reset_state = { 30,0.4 * WINDOW_HEIGHT,1 };
		//Set the button to call the container.load_state() method
		buttons[1].set_pressed_func([container,Reset_state]() {container->load_state(Reset_state); });
	}
};
