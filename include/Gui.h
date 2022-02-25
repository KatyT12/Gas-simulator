#pragma once
#include <vector>

#include <Utils.h>
#include <Button.h>
#include <ParticleController.h>

/// <summary>
/// Display statistics on the user interface in the format:
/// Title: data unit eg. Temperature: 303 K
/// </summary>
struct TextDisplay {
	olc::vi2d pos;
	olc::vi2d size;
	std::function<std::string()> func;
	std::string unit;
	std::string title;
	
};
class Gui {
public:

	State default_state;
	CONSTANT const_variable = CONSTANT::NONE;
	std::vector<Button> buttons; //Buttons the mode will contain
	std::vector<TextDisplay> text_displays;
	//Position,size and colour on the window
	olc::vi2d position{ int(GUI_POS_X * WINDOW_WIDTH),int(GUI_POS_Y * WINDOW_HEIGHT) };
	olc::vi2d size{ int(GUI_SIZE_X * WINDOW_WIDTH),int(GUI_SIZE_Y * WINDOW_HEIGHT) };
	olc::Pixel colour;
	std::string name;
	std::string help_text = std::string(
		"Press the buttons on the left to interact with the simulator.\n\n"
		"The change mode button will change the mode the simulation\n\nis currently"
		" in. Available modes are: Default, Boyles law,\n\nCharles law, the Pressure law"
		" and Brownian motion.\n\n\n\n");
	virtual void adjustments(CONSTANT constant) { return; }
	//An empty constructor to initialize the class before constructing it
	Gui() {};
protected:
	//A reference to the controller is needed to access/alter the simulation
	ParticleController* controller;
	Container* container;
	//Constructor
	Gui(olc::Pixel colour, ParticleController* controller, Container* container, State default_state) : colour(colour), controller(controller), container(container), default_state(default_state) {
		addButton(Button({ position.x - 3, 40 }, { 150,30 }, olc::Pixel(154, 164, 179), "RESET", olc::WHITE));
		//Define the state to reset to: temperature of 30, height of 0.4* window width and 1 particle in the simulation.
		State Reset_state = default_state;
		//Set the button to call the container.load_state() method
		buttons[buttons.size() - 1].set_pressed_func([container, Reset_state]() {container->load_state(Reset_state); });

		addButton(Button({ int(2 * position.x + size.x - 150) / 2, 40 }, { 150,30 }, olc::Pixel(154, 164, 179), "HELP", olc::WHITE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->help_dialog = !controller->help_dialog; });

		addButton(Button({ int(position.x + size.x - 149) , 40 }, { 150,30 }, olc::Pixel(154, 164, 179), "CHANGE MODE", olc::WHITE)); 
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_mode = true; });

		addButton(Button({ 1000, 500 }, { 250,30 }, olc::WHITE, "TOGGLE PARTICLES COLOUR SCALE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->colour_particles = !controller->colour_particles; });

		addButton(Button({ 1000, 550 }, { 250,30 }, olc::WHITE, "TOGGLE TEMPERATURE UNIT"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->use_kelvin = !controller->use_kelvin; });

		add_variable_buttons(controller, container);
		add_text_displays(controller, container);
	}

	/// <summary>
	/// Add a button to the vector buttons
	/// </summary>
	void addButton(Button b) {
		buttons.push_back(b); //push the new button to the back of the vector
	}
	/// <summary>
	/// Add a text display to the vector text_displays
	/// </summary>
	void addTextDisplay(TextDisplay td) {
		text_displays.push_back(td);
	}

	void add_variable_buttons(ParticleController* controller, Container* container) {
		addButton(Button({ position.x + 40, 150 }, { 200,30 }, olc::WHITE, "ADD LIGHT PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::LIGHT); });

		addButton(Button({ position.x + 40, 200 }, { 200,30 }, olc::WHITE, "ADD HEAVY PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->add_particle(PARTICLE_TYPE::HEAVY); });
		addButton(Button({ position.x + 40, 250 }, { 200,30 }, olc::WHITE, "INCREASE TEMPERATURE", CONSTANT::TEMPERATURE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->increment_temperature(9); });
		addButton(Button({ position.x + 40, 300 }, { 200,30 }, olc::WHITE, "INCREASE VOLUME", CONSTANT::VOLUME));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(16); });
		addButton(Button({ int(position.x + size.x / 2 - 200 / 2), 350 }, { 200,30 }, olc::WHITE, "PAUSE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->paused = !controller->paused; });

		addButton(Button({ position.x + size.x - 240, 150 }, { 200,30 }, olc::WHITE, "REMOVE LIGHT PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->remove_particle(PARTICLE_TYPE::LIGHT); });
		addButton(Button({ position.x + size.x - 240, 200 }, { 200,30 }, olc::WHITE, "REMOVE HEAVY PARTICLE", CONSTANT::PARTICLE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->remove_particle(PARTICLE_TYPE::HEAVY); });
		addButton(Button({ position.x + size.x - 240, 250 }, { 200,30 }, olc::WHITE, "DECREASE TEMPERATURE", CONSTANT::TEMPERATURE));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->increment_temperature(-9); });
		addButton(Button({ position.x + size.x - 240, 300 }, { 200,30 }, olc::WHITE, "DECREASE VOLUME", CONSTANT::VOLUME));
		buttons[buttons.size() - 1].set_pressed_func([container]() {container->change_height(-16); });


	};


	void add_text_displays(ParticleController* controller,Container* container) {

		//Display temperature	
		TextDisplay temperature_display;
		temperature_display = { { int(position.x + size.x / 2 - 200 / 2),250} ,{200,30},[controller]() {
			float temperature = controller->get_temperature();
			//convert to celsius if necessary
			if (!controller->use_kelvin)temperature -= 272.15;
			return std::to_string((int)temperature)+ "." + (std::to_string(temperature - (int)temperature)).substr(2,2); },
			"K","Temperature" };
		addTextDisplay(temperature_display);

		TextDisplay NoOfLightParticles = { {int(position.x + size.x / 2 - 200 / 2),150},{200,30},[controller]() {
			return std::to_string((int)controller->light_particles.size()); },"","Light Particles" };
		addTextDisplay(NoOfLightParticles);

		TextDisplay NoOfHeavyParticles = { {int(position.x + size.x / 2 - 200 / 2),200},{200,30},[controller]() {
	return std::to_string((int)controller->heavy_particles.size()); },"","Heavy Particles" };
		addTextDisplay(NoOfHeavyParticles);

		TextDisplay volume_display = { {int(position.x + size.x / 2 - 200 / 2),300},{200,30},[container]() {
			return std::to_string((int)container->get_height() / 100) + "." + std::to_string((container->get_height() / 100.0f) - (int)container->get_height() / 100).substr(2,2); },"m^3","Volume" };
		addTextDisplay(volume_display);


		TextDisplay NoOfCollisions = { {position.x + 40,650},{300,30},[controller]() {
			return std::to_string((int)controller->collisions_per_second); },"","Collisions per second" };
		addTextDisplay(NoOfCollisions);

		TextDisplay rms_display = { {position.x + 40,500},{300,30},[controller]() {
		float rms = controller->calc_rms();
		return std::to_string((int)rms) + "." + std::to_string(rms - (int)rms).substr(2,2); },"m/s","Root Mean Square Speed" };
		addTextDisplay(rms_display);

		TextDisplay AverageKeLightParticles = { {position.x + 40,550},{300,30},[controller]() {
		return std::to_string((int)controller->average_ke_light_particles) + "."
			+ std::to_string(controller->average_ke_light_particles - (int)controller->average_ke_light_particles).substr(2,2); },"J","Average Ke - Light Particles" };
		addTextDisplay(AverageKeLightParticles);

		TextDisplay AverageKeHeavyParticles = { {position.x + 40,600},{300,30},[controller]() {
		return std::to_string((int)controller->average_ke_heavy_particles) + "."
			+ std::to_string(controller->average_ke_heavy_particles - (int)controller->average_ke_heavy_particles).substr(2,2); },"J","Average Ke - Heavy Particles" };
		addTextDisplay(AverageKeHeavyParticles);
	}
};

