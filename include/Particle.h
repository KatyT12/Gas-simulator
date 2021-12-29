#pragma once
#include "Utils.h"

extern int height;

class Particle {
private:
	//Particle's individual properties
	float mass;
	float kinetic_energy;
	int radius;
	olc::vf2d velocity;
	olc::vf2d position;
	PARTICLE_TYPE type;
	
public:
	//Constructor
	Particle(float mass, int radius, olc::vf2d position, PARTICLE_TYPE type) 
		//Setting up the fields of the particle
		:mass(mass), radius(radius), position(position), kinetic_energy(0), velocity(olc::vf2d(0, 0)), type(type) {} 


	void move(float seconds) {
		//Check if the particle will collide with the container when it moves
		check_collision_with_container(position + seconds * velocity);
		position = position + seconds * velocity; //update position
	}

	//Reverses velocity if either edge will be collided with
	bool check_collision_with_container(olc::vf2d pos) {
		
		//Check if the particle has collided with the left or right edge
		if (pos.x + rescale_length(radius) >= 100 || pos.x - rescale_length(radius) <= 1 ) {
			velocity = { velocity.x * -1, velocity.y};
			return true;
		}
		//Check if the particle has collided with the top or bottom edge
		else if(pos.y + rescale_height(radius) >= 99 || pos.y - rescale_height(radius) <= 1){
			velocity = { velocity.x , velocity.y  * -1};
			return true;
		} 
		else { return false;}
	}

	olc::vf2d get_position() { return position; }

	int get_radius() { return radius;}

	void set_velocity(olc::vf2d velocity) { this->velocity = velocity; }
	
	olc::vf2d get_velocity() { return velocity;}

};

