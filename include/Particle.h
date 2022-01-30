#pragma once
#include "Utils.h"

#define COLLISION_RESOLUTION 0.1

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

	//For debugging only
	int id;

	void move(float seconds, int height) {
		check_collision_with_container({ position.x + (seconds * velocity.x), position.y + (seconds * velocity.y) }, height);
		position = { position.x + (seconds * velocity.x), position.y + (seconds * velocity.y) };

	}

	//Reverses velocity if either edge will be collided with
	bool check_collision_with_container(olc::vf2d pos, int height) {
		//Check if the particle has collided with the left or right edge
		if (pos.x + rescale_length(radius) >= 100 || pos.x - rescale_length(radius) <= 1 ) {
			velocity = { velocity.x * -1, velocity.y};
			return true;
		}
		//Check if the particle has collided with the top or bottom edge
		else if(pos.y + rescale_height(radius,height) >= 99 || pos.y - rescale_height(radius,height) <= 1){
			velocity = { velocity.x , velocity.y  * -1};
			return true;
		} 
		else { return false;}
	}

	//Check collisions with particles
	bool check_collision_with_particle(Particle p, float seconds, int height) {
		//Get next position
		olc::vf2d pos = { position.x + (seconds * velocity.x), position.y + (seconds  * velocity.y) };
		olc::vf2d pos1 = { p.position.x + (seconds * p.get_velocity().x), p.position.y + (seconds  * p.get_velocity().y) };

		//Convert position to screen coordinates
		olc::vi2d screen_pos = delocalize_position(pos, height);
		olc::vi2d screen_pos1 = delocalize_position(pos1, height);

		//Distance between particle's centres
		float distance = pow(pow((screen_pos.x - screen_pos1.x), 2) + pow((screen_pos.y - screen_pos1.y), 2), 0.5);

		//If the distance between the particles is smaller than the sum of their radius and some resolution
		if (distance - (radius + p.get_radius()) <= COLLISION_RESOLUTION) {
			return true;
		}
		return false;
	}
 
	void handle_collision(Particle* p, float seconds, olc::vf2d velocity2) {
		//Next position of the two particles
		olc::vf2d pos = { position.x + (seconds * velocity.x), position.y + (seconds  * velocity.y) };
		olc::vf2d pos1 = { p->position.x + (seconds * velocity2.x), p->position.y + (seconds  * velocity2.y) };

		//Vectors needed in equation
		olc::vf2d vec = pos - pos1;
		olc::vf2d vel = velocity - velocity2;

		//Using the two equations to find velocity
		velocity += ((vec * -1) / vec.mag()) * (vec.dot(vel) / vec.mag()) 
			* p->mass / (mass + p->mass) * 2; //This part at the end is to account for mass

		//Ek = 0.5 * mass * velocity^2
		kinetic_energy = std::pow(velocity.mag(), 2) * mass * 0.5;
		
		//Accounts for errors that may be caused when two collisions are detected when only one should be
		if (acos(-vec.norm().dot(velocity.norm())) < PI / 2) {
			set_velocity({ velocity.x * -1, velocity.y * -1 });
		}
	}

	olc::vf2d get_position() { return position; }

	int get_radius() { return radius;}

	void set_velocity(olc::vf2d velocity) { this->velocity = velocity; }
	
	olc::vf2d get_velocity() { return velocity;}

	float get_kinetic_energy() { return kinetic_energy; }
	void set_kinetic_energy(float ke) { kinetic_energy = ke; }

	float get_mass() { return mass; }

};

