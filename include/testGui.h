#pragma once
#include <Gui.h>


//Extends Gui class
static class testMode : public Gui {
public:
	

	//The controller and container is a needed to access the simulation
	testMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(120,183,196), controller,container) {

		name = "Default Mode";
		help_text += "Currently the simulation is in default mode. This means\n\n"
			"that no variables will be kept constant and you are free to\n\n"
			"change anything you like.";
		
		addButton(Button({ position.x + 40, 150 }, { 200,30 }, olc::WHITE, "ADD LIGHT PARTICLE"));  
		buttons[buttons.size()-1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::LIGHT); }); 

		addButton(Button({ position.x + 40, 200 }, { 200,30 }, olc::WHITE, "ADD HEAVY PARTICLE"));  
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::HEAVY); }); 
		addButton(Button({ position.x + 40, 250 }, { 200,30 }, olc::WHITE, "INCREASE TEMPERATURE"));
		buttons[buttons.size()-1].set_pressed_func([controller]() {controller->increment_temperature(3); });
		addButton(Button({ position.x + 40, 300 }, { 200,30 }, olc::WHITE, "INCREASE VOLUME"));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(8); });
		addButton(Button({ int(position.x + size.x / 2 - 200 / 2), 350 }, { 200,30 }, olc::WHITE, "PAUSE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->paused = !controller->paused; });

		addButton(Button({ position.x + size.x - 240, 150 }, { 200,30 }, olc::WHITE, "REMOVE LIGHT PARTICLE")); 
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->remove_particle(PARTICLE_TYPE::LIGHT); });
		addButton(Button({ position.x + size.x - 240, 200 }, { 200,30 }, olc::WHITE, "REMOVE HEAVY PARTICLE"));  
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->remove_particle(PARTICLE_TYPE::HEAVY); }); 
		addButton(Button({ position.x + size.x - 240, 250 }, { 200,30 }, olc::WHITE, "DECREASE TEMPERATURE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->increment_temperature(-3); });
		addButton(Button({ position.x + size.x - 240, 300 }, { 200,30 }, olc::WHITE, "DECREASE VOLUME"));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(-8); });
		
		//Display temperature
		TextDisplay temperature_display = { {position.x + 40,600} ,[controller]() {
			return std::to_string((int)controller->get_temperature()) + "." + (std::to_string(controller->get_temperature() - (int)controller->get_temperature())).substr(2,2); },
			"K","Temperature" };
		addTextDisplay(temperature_display);


	}
};
