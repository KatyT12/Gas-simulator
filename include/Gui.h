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
	std::function<std::string()> func;
	std::string unit;
	std::string title;
};


class Gui {
public:
	
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
	//An empty constructor to initialize the class before constructing it
	Gui() {};
protected:
	
	//Constructor
	Gui(olc::Pixel colour,ParticleController* controller, Container* container) : colour(colour),controller(controller) {
		addButton(Button({position.x-4, 40}, { 150,30 }, olc::WHITE, "RESET"));
		//Define the state to reset to: temperature of 30, height of 0.4* window width and 1 particle in the simulation.
		State Reset_state = { 303.15,0.4 * WINDOW_HEIGHT,1 };
		//Set the button to call the container.load_state() method
		buttons[buttons.size()-1].set_pressed_func([container, Reset_state]() {container->load_state(Reset_state); });

		addButton(Button({ int(position.x+size.x/2 -150/2), 40 }, { 150,30 }, olc::WHITE, "HELP"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->help_dialog = !controller->help_dialog; });

		addButton(Button({ int(position.x + size.x - 150) , 40 }, { 150,30 }, olc::WHITE, "CHANGE MODE"));
		buttons[buttons.size() - 1].set_pressed_func([controller]() {controller->help_dialog = !controller->help_dialog; });
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
	//A reference to the controller is needed to access/alter the simulation
	ParticleController* controller;
};


