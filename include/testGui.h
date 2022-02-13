#pragma once
#include <Gui.h>


//Extends Gui class
static class testMode : public Gui {
public:
	

	//The controller and container is a needed to access the simulation
	testMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(89,214,214), controller,container) {

		name = "Default Mode";
		help_text += "Currently the simulation is in default mode. This means\n\n"
			"that no variables will be kept constant and you are free to\n\n"
			"change anything you like.";
		
		addButton(Button({ position.x + 40, 150 }, { 200,30 }, olc::WHITE, "ADD"));  //Add a new button which adds a particle to the simulation
		buttons[buttons.size()-1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::LIGHT); }); //Set the function of the button
		addButton(Button({ position.x + 40, 200 }, { 200,30 }, olc::WHITE, "+ TEMPERATURE"));
		buttons[buttons.size()-1].set_pressed_func([controller]() {controller->increment_temperature(2); });
		addButton(Button({ position.x + 40, 250 }, { 200,30 }, olc::WHITE, "+ VOLUME"));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(8); });
		addButton(Button({ position.x + 40, 300 }, { 200,30 }, olc::WHITE, "PAUSE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->paused = !controller->paused; });

		//Display temperature
		TextDisplay temperature_display = { {position.x + 40,600} ,[controller]() {
			return std::to_string((int)controller->get_temperature()) + "." + (std::to_string(controller->get_temperature() - (int)controller->get_temperature())).substr(2,2); },
			"K","Temperature" };
		addTextDisplay(temperature_display);


	}
};
