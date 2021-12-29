#pragma once
#include <vector>

#include <Utils.h>
#include <Button.h>
#include <ParticleController.h>

class Gui {
public:
	std::vector<Button> buttons; //Buttons the mode will contain
	
	//Position,size and colour on the window
	olc::vi2d position{ int(GUI_POS_X * WINDOW_WIDTH),int(GUI_POS_Y * WINDOW_HEIGHT) }; 
	olc::vi2d size{ int(GUI_SIZE_X * WINDOW_WIDTH),int(GUI_SIZE_Y * WINDOW_HEIGHT) };
	olc::Pixel colour;
	//An empty constructor to initialize the class before constructing it
	Gui() {};
protected:
	
	//Constructor
	Gui(olc::Pixel colour,ParticleController* controller) : colour(colour),controller(controller) {}

	void addButton(Button b) {
		buttons.push_back(b); //push the new button to the back of the vector
	}
	//A reference to the controller is needed to access/alter the simulation
	ParticleController* controller;
};


