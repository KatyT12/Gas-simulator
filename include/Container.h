#pragma once
#include <Particle.h>
#include <Utils.h>
#include <ParticleController.h>

#include <vector>




static class Container {
private:
	float width, posX, posY, area, height;
	ParticleController controller;
public:

	bool change_mode = false;

	Container() {
		//Setting the position and size of the container
		posX = CONTAINER_X_POS * WINDOW_WIDTH;
		posY = CONTAINER_MIN_Y_POS * WINDOW_HEIGHT;
		width = CONTAINER_WIDTH * WINDOW_WIDTH;
		height = 0;
		change_height(0.4 * WINDOW_HEIGHT);
		//Adjust the Y coordinate and recalculate area
		calculate_dimensions();	
		//Create an instance of the particle controller to control the simulation
		
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

	/// <summary>
	/// Change the height of the container
	/// </summary>
	/// <param name="amount"></param>
	void change_height(double amount) {
		if (height + amount < 600 && height + amount> 200) {
			height = height += amount;
			calculate_dimensions();
			controller.height = height;
			controller.correct_particles(amount);
		}
	}
	
	/// <summary>
	/// Load a state on the simulation, edit the height of the container then let
	/// the particle controller handle the rest
	/// </summary>
	/// <param name="state"></param>
	void load_state(State state) {
		change_height(state.height - height);
		controller.load_state(state);
	}
	
};
