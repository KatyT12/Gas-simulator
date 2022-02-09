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


/// This is used to store the colision of two particles to later
/// be processed
struct Collision {
	Particle* a;
	Particle* b;
	bool operator <(const Collision& pt) const
	{
		return true;
	}
};


/// Instances of this struct will be used to store
/// the attributes of different states of the simulation
struct State {
	float temperature;
	float height;
	int particle_count;
};

static class ParticleController {
private:

	int particle_count;
	float delta_energy = 0;
	float temperature = 303.15; //In Kelvin
	float time_between_frames;
	

	void update_particle_energies();
	void check_collisions();
	void handle_collisions(std::set<Collision>& collisions);
public:
	int height;
	std::vector<Particle> particles;

	ParticleController(); //Constructor
	bool add_particle(PARTICLE_TYPE type); //add particle to simulation
	void load_state(State& state);
	inline void set_time_between_frames(float time) { time_between_frames = time; }; //Update time between frames
	void update(); //Ran every frame to update simulation
};
