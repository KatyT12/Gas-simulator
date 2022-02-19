#include <ParticleController.h>


//Constructor
ParticleController::ParticleController() :particle_count(0) {
	srand(time(NULL)); //The time is used so it is different everytime the application is run
}

//Update simulation
void ParticleController::update() {
	check_collisions(); //Check and handle particle collisions
	for (Particle& p : particles) {
		bool collision = p.move(time_between_frames,height); //Move each particle
		if (collision) collision_counter += 1;
	}
	if (delta_energy != 0) {
		update_particle_energies();
	}
	
	calc_collisions_per_second();
}

olc::vf2d ParticleController::get_random_unit_vector() {
	float theta = (((float)rand() / RAND_MAX) * 2 * PI);
	float x = cos(theta);
	float y = sin(theta);
	return { x,y };
}

void ParticleController::calc_collisions_per_second() {
	seconds = std::min(seconds + time_between_frames,1.0f);
	if (seconds == 1) {
		collisions_per_second = collision_counter;
		collision_counter = 0;
		seconds = 0;
	}
}

/// <summary>
///  Calculate average kinetic energy for given particle type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
float ParticleController::get_average_kinetic_energy(PARTICLE_TYPE type) {
	std::vector<int> particle_indexes;
	if (type == PARTICLE_TYPE::LIGHT) { particle_indexes = light_particles;}
	else { particle_indexes = heavy_particles;}

	float total = 0;
	for (int index : particle_indexes) {
		total += particles[index].get_kinetic_energy();
	}
	
	if (particle_indexes.size() > 0) {
		return total / particle_indexes.size();
	}
	else {
		return 0;
	}
	
}

/// <summary>
/// Add a particle to the simulation of one of 
/// the particle types
/// </summary>
/// <param name="type"></param>
/// <returns>bool</returns>
bool ParticleController::add_particle(PARTICLE_TYPE type) {
	float energy = (3 / 2) * 1.3806452* pow(10, -2) * temperature;
	//Possible error
	if (particles.size() <= MAX_PARTICLES_NUM) {
		particle_count += 1;
		
		float mass;
		float size;
		//radius & mass changes depending on particle type so an if statement is needed
		if (type == PARTICLE_TYPE::HEAVY) {
			mass = LARGE_PARTICLE_MASS;
			size = LARGE_PARTICLE_SIZE;
			heavy_particles.push_back(particles.size());
		}
		else if (type == PARTICLE_TYPE::LIGHT) {
			mass = SMALL_PARTICLE_MASS;
			size = SMALL_PARTICLE_SIZE;
			light_particles.push_back(particles.size());
		}
		olc::vf2d position = { (float)(rand() % int(100 - rescale_length(2 * size)-3) + rescale_length(size)+1), float(rand() % 55 + 10) };
		particles.emplace_back(Particle(mass, size, position, type));

		float speed = pow(energy * 2 / mass, 0.5);
		olc::vf2d velocity = get_random_unit_vector() * std::max(speed,0.5f);
		particles.back().set_velocity(velocity);
		particles.back().set_energy(energy);
		//For debugging
		particles.back().id = particle_count;

		return true; //A particle was successfully added to the simulation
	}
	else {
		return false; //A particle couldn't be added to the simulation
	}
}



/// <summary>
/// change the temperature and calculate the change in energy needed for the particles
/// </summary>
/// <param name="temperature_add"></param>
void ParticleController::increment_temperature(float temperature_add) {
	
	float temp = temperature;
	temperature += temperature_add;
	temperature = std::max(temperature, 0.0f);
	temperature = std::min(temperature, 700.0f);

	if (temp + temperature_add < 700 && temp + temperature_add > 0) {
		// 3/2 * boltzmann constant * temperature = energy (boltzmann constant edited)
		for (Particle& p : particles) {
			p.set_energy(p.get_kinetic_energy() * temperature/std::max(temp,1/temperature));
			p.calc_velocity();
			
		
		}
		//delta_energy = delta_energy += (3 / 2) * 5 * 1.3806452 * pow(10, -2) * temperature_add;
	}	
}



void ParticleController::check_collisions() {
	//Initialize set of collisions
	std::set<Collision> collisions;
	//Check every particle against every particle
	for (int i = 0; i < particles.size(); i++) {
		Particle& p = particles[i]; //References needed rather than copies of data
		for (int j = i; j < particles.size(); j++) {
			Particle& a = particles[j];
			if (i != j) {
				//If the particles have collided, add them to the set as a Collision
				if (p.check_collision_with_particle(a, time_between_frames, height)) {
					collisions.insert({ &particles[i], &particles[j] }); 
				}
			}
		}
	}
	//Handle all the collisions that have just been detected
	handle_collisions(collisions);
}

void ParticleController::handle_collisions(std::set<Collision>& collisions) {



	for (auto& collision : collisions) {
		auto a = collision.a;
		auto b = collision.b;
	
		//Temporary variable to store particle A before it gets modified
		olc::vf2d temp = a->get_velocity();
		a->handle_collision(b, time_between_frames, b->get_velocity());
	    b->handle_collision(a, time_between_frames, temp);


		//std::cout << "Collision between particle " << collision.a->id << " and particle " << collision.b->id << " Have collided\n";
	}


}
/// <summary>
/// (Will be altered in the future) loads the 
/// attributes from the state to the simulation
/// </summary>
/// <param name="state"></param>
void ParticleController::load_state(State& state) {
	temperature = state.temperature;
	// Remove current articles stored and aadd as many new ones ads required
	particles.clear();
	light_particles.clear();
	heavy_particles.clear();
	for (int p = 0; p < state.particle_count; p++) {
		//In the future this may be edited to allow the state to specify an amount of light 
		//and an amount of heavy particles to add
		add_particle(PARTICLE_TYPE::LIGHT);
	}
}



/// <summary>
/// Update the enrgy of particles according to temperature change.
/// Done in each frame after a change in temperature until there is no change to
/// be done
/// </summary>
void ParticleController::update_particle_energies() {
	//delta energy is the change in energy that needs to be done
	//calculates energy to add/subtract this frame
	float energy = delta_energy * time_between_frames * 100;
	delta_energy = delta_energy - energy;

	//Alter energy of particles and recalculate their velocities
	for (Particle& p : particles) {
		p.add_energy(energy);
		p.calc_velocity();
	}
	//If the magnitude of the energy left to add is less than a very small number set it to 0
	if (std::pow(std::pow(delta_energy, 2), 0.5) < 0.0000001) {
		delta_energy = 0;
	}
}

/// <summary>
/// To be ran when volume changes.
/// This method will change particle positions so that they
/// are in the correct position when the volume changes
/// </summary>
/// <param name="amount"></param>
void ParticleController::correct_particles(int amount) {
	for (Particle& p : particles) {
		olc::vf2d pos = { p.get_position().x,p.get_position().y + (((float)amount / (float)height) * 100 * (float)(height / (float)(WINDOW_HEIGHT * 0.4))) };
		//If the volume has decreased, particles that would be moved above the container should be moved down and not corrected
		if (p.get_position().y>p.get_radius()){
			p.set_position(pos);
		}
	}
}

/// <summary>
/// Remove particle of certain particle type
/// </summary>
/// <param name="type"></param>
void ParticleController::remove_particle(PARTICLE_TYPE type) {
	
	int remove_index = -1;
	if (type == PARTICLE_TYPE::HEAVY) {
		if (heavy_particles.size() > 0) {
			remove_index = heavy_particles.back();
			particles.erase(particles.begin() + remove_index);
			heavy_particles.pop_back();
			
			//If light particles have been added after the last heavy particle 
			//their index needs to be decremented as the vector shrinks by one
			for (int& i : light_particles) {
				if (i > remove_index) {
					i -= 1;
				}
			}
		}
	}
	if (type == PARTICLE_TYPE::LIGHT) {
		if (light_particles.size() > 0) {
			remove_index = light_particles.back();
			particles.erase(particles.begin() + remove_index);
			light_particles.pop_back();
			//If heavy particles have been added after the last light particle 
			//their index needs to be decremented as the vector shrinks by one
			for (int& i : heavy_particles) {
				if (i > remove_index) {
					i -= 1;
				}
			}
		}
	}
}

float ParticleController::adjust_volume(CONSTANT constant) {
	float height_change = 0;
	if (constant == CONSTANT::TEMPERATURE) {
		height_change = 600 - ((float)particles.size() / (float)MAX_PARTICLES_NUM) * (float)400 - height;
	}
	return height_change;
}

int ParticleController::adjust_particles(CONSTANT constant) {
	int particle_change = 0;
	if (constant == TEMPERATURE) {
		particle_change = (1.0f - ((float)height - 200) / (float)400) * MAX_PARTICLES_NUM - particles.size();
	}
	std::cout << particle_change << "\n";

	if (particle_change < 0) {
		while (particle_change < 0 && light_particles.size() > 0) {
			remove_particle(PARTICLE_TYPE::LIGHT);
			particle_change++;
		}
		while (particle_change < 0 && heavy_particles.size() > 0) {
			remove_particle(PARTICLE_TYPE::HEAVY);
			particle_change++;
		}
	}
	else {
		for (int i = 0; i < particle_change; i++) {
			add_particle(PARTICLE_TYPE::LIGHT);
		}
	}
	return particle_change;
}