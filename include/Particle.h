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

	//Update the positions of each particle
	void move(float seconds, int height);

	//Reverses velocity if either edge will be collided with
	bool check_collision_with_container(olc::vf2d pos, int height);

	//Check collisions with particles
	bool check_collision_with_particle(Particle p, float seconds, int height);
	//After detecting the collisions, deal with them
	void handle_collision(Particle* p, float seconds, olc::vf2d velocity2);

	/// <summary>
	/// Calculate the new velocity, after a change in energy
	/// Direction is the same but magnitude is altered.
	/// </summary>
	void calc_velocity();

	//Get and set methods
	inline olc::vf2d get_position() { return position; }
	inline int get_radius() { return radius;}
	
	inline void set_velocity(olc::vf2d velocity) { this->velocity = velocity; }
	inline olc::vf2d get_velocity() { return velocity;}
	
	inline float get_kinetic_energy() { return kinetic_energy; }
	inline void add_energy(float energy) { kinetic_energy += energy; }
	inline void set_energy(float energy) { kinetic_energy = energy; }
	
	inline float get_mass() { return mass; }

};

