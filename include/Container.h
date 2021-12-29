#pragma once
#include <Particle.h>
#include <Utils.h>
#include <ParticleController.h>

#include <vector>

extern int height; // Other files will use this field so a static variable is used 

static class Container {
private:
	float width, posX, posY, area;
	ParticleController controller;
public:
	
	Container() {
		//Setting the position and size of the container
		posX = CONTAINER_X_POS * WINDOW_WIDTH;
		posY = CONTAINER_MIN_Y_POS * WINDOW_HEIGHT;
		width = CONTAINER_WIDTH * WINDOW_WIDTH;
		height = 0.4 * WINDOW_HEIGHT;
		//Adjust the Y coordinate and recalculate area
		calculate_dimensions();	
		//Create an instance of the particle controller to control the simulation
		controller = ParticleController();
	}
	
	//Will recalculate the position and area when height is changed
	void calculate_dimensions() {
		//Move Y coordinate down from the top of the screen by a constant and the height
		posY = WINDOW_HEIGHT - (CONTAINER_MIN_Y_POS * WINDOW_HEIGHT + height);
		area = width * height;
	}

	//Get methods
	olc::vi2d get_position() { return olc::vf2d(posX, posY); }
	int get_height() {return height;}
	int get_width() {return width;}
	//Return a reference to the particle controller so other class can use it
	ParticleController* get_controller() { return &controller; }

	//Will change the height and re calculate dimensions of the container
	void change_height(double amount) {
		height = height += amount;
		calculate_dimensions();
	}
	
	
};
