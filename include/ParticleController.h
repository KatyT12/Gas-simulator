#pragma once
#include <vector>
#include <random>
#include <set>


#include <olcPixelGameEngine.h>
#include <Particle.h>
#include <Utils.h>

#define SMALL_PARTICLE_SIZE 8
#define LARGE_PARTICLE_SIZE 16
#define SMALL_PARTICLE_MASS 0.001
#define LARGE_PARTICLE_MASS 0.003


#define MAX_PARTICLES_NUM 200


struct Collision {
	Particle* a;
	Particle* b;


	bool operator <(const Collision& pt) const
	{
		return true;
	}
};

static class ParticleController {
public:
	int height;
	int particle_count;
	float time_between_frames;
	std::vector<Particle> particles;
	

	
	ParticleController(); //Constructor
	void update(); //Ran every frame to update simulation
	bool add_particle(PARTICLE_TYPE type); //add particle to simulation
	inline void set_time_between_frames(float time) { time_between_frames = time; }; //Update time between frames
	
	void check_collisions();
	void handle_collisions(std::set<Collision>& collisions);
};
