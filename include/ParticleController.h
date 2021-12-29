#pragma once
#include <vector>
#include <random>
#include "Particle.h"
#include "Utils.h"

#define MAX_PARTICLES_NUM 200

extern int height;
extern float time_between_frames;

static class ParticleController {
public:
	int particle_count; //Number of particles
	std::vector<Particle> particles; // Where all the particles will be stored
	
	//Constructor
	ParticleController() :particle_count(0){}

	//Update simulation
	void update() {
		for (Particle& p : particles) {
			p.move(time_between_frames); //Move each particle
		}
	}


	//	olc::vf2d position = localize_position(add_container_margins({ int(rand() % int(CONTAINER_WIDTH * WINDOW_WIDTH - SMALL_PARTICLE_SIZE)),int(rand() % int(height - SMALL_PARTICLE_SIZE))}));

	//Add particle to simulation
	bool add_particle(PARTICLE_TYPE type) {
		if (particle_count + 1 <= MAX_PARTICLES_NUM) {
			particle_count += 1;
			srand(time(NULL)); //The time is used so it is different everytime the application is run
			
			olc::vf2d velocity = { 200,200};
			
			//radius & mass changes depending on particle type so an if statement is needed
			if (type == PARTICLE_TYPE::HEAVY) {			
				olc::vf2d position = { (float)(rand() % int(100 - rescale_length(2 * LARGE_PARTICLE_SIZE) - 1) + rescale_length(LARGE_PARTICLE_SIZE)), float(rand() % int(100 - rescale_height(2 * LARGE_PARTICLE_SIZE)) + rescale_height(LARGE_PARTICLE_SIZE)) };
				particles.emplace_back(Particle(0.001, LARGE_PARTICLE_SIZE, position, type));
			}
			else if (type == PARTICLE_TYPE::LIGHT) {
				olc::vf2d position = { (float)(rand() % int(100 - rescale_length(2 * SMALL_PARTICLE_SIZE) - 2) + rescale_length(SMALL_PARTICLE_SIZE))+1, float(rand() % int(100 - rescale_height(2 * SMALL_PARTICLE_SIZE)-2) + rescale_height(SMALL_PARTICLE_SIZE)+1)};
				particles.emplace_back(Particle(0.001, SMALL_PARTICLE_SIZE, position, type));
				
			}
			
			particles.back().set_velocity(velocity);

			return true; //A particle was successfully added to the simulation
		}
		else { 
			return false; //A particle couldn't be added to the simulation
		}
	}
};
