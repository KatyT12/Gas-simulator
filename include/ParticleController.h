#pragma once
#include <vector>
#include <random>
#include <set>

#include <olcPixelGameEngine.h>
#include <Particle.h>
#include <Utils.h>


#define MAX_PARTICLES_NUM 200
#define MAX_Z_VALUE 3.0f


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
	int light_particle_count;
	int heavy_particle_count;
	int very_heavy_particle_count;
};

static class ParticleController {
private:
	olc::Pixel colours[13];
	int particle_count;
	float temperature = 303.15; //In Kelvin
	float time_between_frames;
	
	float collision_counter = 0;
	float seconds = 0;
	

	
	void check_collisions();
	void handle_collisions(std::set<Collision>& collisions);
	olc::vf2d get_random_unit_vector();
public:
	bool paused = false;
	bool help_dialog = false;
	bool colour_particles = false;
	bool use_kelvin = true;

	float collisions_per_second = 0;
	float average_ke_light_particles;
	float average_ke_heavy_particles;
	float average_ke_all_particles;
	

	int height;
	std::vector<Particle> particles;
	std::vector<int> heavy_particles;
	std::vector<int> light_particles;

	ParticleController(); //Constructor
	
	bool add_particle(PARTICLE_TYPE type); //add particle to simulation
	void remove_particle(PARTICLE_TYPE type);



	void increment_temperature(float temperature_add);
	inline float get_temperature() { return temperature; }
	void correct_particles(int amount);
	void load_state(State& state);
	inline void set_time_between_frames(float time) { time_between_frames = time; }; //Update time between frames
	void update(); //Ran every frame to update simulation
	float get_average_kinetic_energy(PARTICLE_TYPE type);
	
		
	void calc_collisions_per_second();
	float adjust_volume(CONSTANT constant);
	float adjust_temperature(CONSTANT constant);
	float calc_pressure();
	
	float calc_standard_deviation();
	void update_particle_z_value(float standard_deviation);
	olc::Pixel get_particle_colour(Particle& p);
	float calc_rms();

};
