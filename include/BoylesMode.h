#pragma once
#include <Gui.h>
#include <Container.h>

//Extends Gui class
static class BoylesMode : public Gui {
public:


	//The controller and container is a needed to access the simulation
	BoylesMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(120, 183, 196), controller, container, { 303.15,580,10 }) {


		name = "Default Mode";
		help_text += "Currently the simulation is in default mode. This means\n\n"
			"that no variables will be kept constant and you are free to\n\n"
			"change anything you like.";


		addButton(Button({ position.x + 40, 150 }, { 200,30 }, olc::WHITE, "ADD LIGHT PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::LIGHT); });

		addButton(Button({ position.x + 40, 200 }, { 200,30 }, olc::WHITE, "ADD HEAVY PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::HEAVY); });
		addButton(Button({ position.x + 40, 250 }, { 200,30 }, olc::WHITE, "INCREASE TEMPERATURE", CONSTANT::TEMPERATURE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->increment_temperature(3); });
		addButton(Button({ position.x + 40, 300 }, { 200,30 }, olc::WHITE, "INCREASE VOLUME", CONSTANT::VOLUME));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(8); });
		addButton(Button({ int(position.x + size.x / 2 - 200 / 2), 350 }, { 200,30 }, olc::WHITE, "PAUSE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->paused = !controller->paused; });

		addButton(Button({ position.x + size.x - 240, 150 }, { 200,30 }, olc::WHITE, "REMOVE LIGHT PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->remove_particle(PARTICLE_TYPE::LIGHT); });
		addButton(Button({ position.x + size.x - 240, 200 }, { 200,30 }, olc::WHITE, "REMOVE HEAVY PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->remove_particle(PARTICLE_TYPE::HEAVY); });
		addButton(Button({ position.x + size.x - 240, 250 }, { 200,30 }, olc::WHITE, "DECREASE TEMPERATURE", CONSTANT::TEMPERATURE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->increment_temperature(-3); });
		addButton(Button({ position.x + size.x - 240, 300 }, { 200,30 }, olc::WHITE, "DECREASE VOLUME", CONSTANT::VOLUME));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(-8); });


		//Display temperature
		TextDisplay temperature_display = { {position.x + 40,600} ,[controller]() {
			return std::to_string((int)controller->get_temperature()) + "." + (std::to_string(controller->get_temperature() - (int)controller->get_temperature())).substr(2,2); },
			"K","Temperature" };
		addTextDisplay(temperature_display);

		TextDisplay NoOfParticles = { {position.x + 40,550},[controller]() {
			return std::to_string((int)controller->particles.size()); },"","Particles" };
		addTextDisplay(NoOfParticles);

		TextDisplay NoOfCollisions = { {position.x + 40,650},[controller]() {
			return std::to_string((int)controller->collisions_per_second); },"","Collisions per second" };
		addTextDisplay(NoOfCollisions);

		TextDisplay AverageKeLightParticles = { {position.x + 400,550},[controller]() {
		return std::to_string((int)controller->get_average_kinetic_energy(PARTICLE_TYPE::LIGHT)); },"J","Light Particles Ke" };
		addTextDisplay(AverageKeLightParticles);

		TextDisplay AverageKeHeavyParticles = { {position.x + 400,600},[controller]() {
	return std::to_string((int)controller->get_average_kinetic_energy(PARTICLE_TYPE::HEAVY)); },"J","Heavy Particles Ke" };
		addTextDisplay(AverageKeHeavyParticles);
	}


	void adjustments(CONSTANT constant) {
		if (constant == CONSTANT::PARTICLE) {
			float volume = controller->adjust_volume(CONSTANT::TEMPERATURE);
			container->change_height(volume);
		}
		if (constant == CONSTANT::VOLUME) {
			int particle_change = controller->adjust_particles(CONSTANT::TEMPERATURE);
		
		}
		
	}
};
