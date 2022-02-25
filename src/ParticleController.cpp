#include <ParticleController.h>


//Constructor
ParticleController::ParticleController() :particle_count(0) {
	srand(time(NULL)); //The time is used so it is different everytime the application is run
	
	colours[0] = olc::Pixel(161,0,0);
	colours[1] = olc::Pixel(176, 0, 0);
	colours[2] = olc::Pixel(192, 46, 0);
	colours[3] = olc::Pixel(208, 98, 0);
	colours[4] = olc::Pixel(224, 148, 0);
	colours[5] = olc::Pixel(239, 192, 0);
	colours[6] = olc::Pixel(255, 229, 0);
	colours[7] = olc::Pixel(255, 238, 40);
	colours[8] = olc::Pixel(255, 245, 79);
	colours[9] = olc::Pixel(255, 250, 119);
	colours[10] = olc::Pixel(255, 254, 158);
	colours[11] = olc::Pixel(255, 255, 198);
	colours[12] = olc::Pixel(254,255,237);
}

/// <summary>
/// This method is ran each frame and is what updates the
/// simulation. Is not ran when the simulation is paused.
/// </summary>
void ParticleController::update() {
	check_collisions(); //Check and handle particle collisions
	for (Particle& p : particles) {
		bool collision = p.move(time_between_frames,height); //Move each particle
		//Increment collision counter
		if (collision) collision_counter += 1;
	}
	
	//calculate statistics
	calc_collisions_per_second();
	average_ke_heavy_particles = get_average_kinetic_energy(PARTICLE_TYPE::HEAVY);
	average_ke_light_particles = get_average_kinetic_energy(PARTICLE_TYPE::LIGHT);
	average_ke_all_particles = get_average_kinetic_energy(PARTICLE_TYPE::ANY);

	if (colour_particles) {
		update_particle_z_value(calc_standard_deviation());
	}
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
		else if (type == PARTICLE_TYPE::VERY_HEAVY) {
			mass = VERY_LARGE_PARTICLE_MASS;
			size = VERY_LARGE_PARTICLE_SIZE;
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
	temperature = std::max(temperature, 1.0f);
	temperature = std::min(temperature, 700.0f);

	if (temp != temperature) {
		for (Particle& p : particles) {
			
			p.set_energy(p.get_kinetic_energy() * temperature/std::max(temp,1/temperature));
			p.calc_velocity();
		}
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
	for (int p = 0; p < state.light_particle_count; p++) {
		add_particle(PARTICLE_TYPE::LIGHT);
	}
	for (int p = 0; p < state.heavy_particle_count; p++) {
		add_particle(PARTICLE_TYPE::HEAVY);
	}
	for (int p = 0; p < state.very_heavy_particle_count; p++) {
		add_particle(PARTICLE_TYPE::VERY_HEAVY);
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
		if (p.get_position().y>p.get_radius()*2){
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

/// <summary>
/// Adjust volume according to temperature proportionally
/// </summary>
float ParticleController::adjust_volume(CONSTANT constant) {
	float height_change = 0;
	height_change =  (400 * temperature / 700) - height + 200;
	return height_change;
}
/// <summary>
/// Adjust temperature according to volume
/// </summary>
float ParticleController::adjust_temperature(CONSTANT constant) {
	float temperature_change = 0;
	
	temperature_change = (((float)height-200) / 400.0f) * 700.0f - temperature;
	return temperature_change;
}

float ParticleController::calc_pressure() {
	
	float pressure = average_ke_all_particles *1/9 * collisions_per_second;
	return pressure;
}

/// <summary>
///  Calculate average kinetic energy for given particle type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
float ParticleController::get_average_kinetic_energy(PARTICLE_TYPE type) {
	std::vector<int> particle_indexes;
	float total = 0;
	if (type == PARTICLE_TYPE::LIGHT) { particle_indexes = light_particles; }
	else if (type == PARTICLE_TYPE::HEAVY) { particle_indexes = heavy_particles; }

	else if (type == PARTICLE_TYPE::ANY && particles.size() > 0) {
		for (Particle& p : particles) {
			total += p.get_kinetic_energy();
		}
		total /= particles.size();
		return total;
	}

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
/// z values - number of standard deviations away from the mean
/// this method updates the z values of each particle
/// </summary>
/// <param name="standard_deviation"></param>

float ParticleController::calc_standard_deviation() {
	float total = 0;
	//Sum of squares
	for (Particle& p : particles) {
		total += pow(p.get_kinetic_energy(), 2);
	}

	float standard_deviation = std::pow(total / (float)particles.size() - std::pow(average_ke_all_particles,2),0.5);
	return standard_deviation;
}

/// <summary>
/// z values - number of standard deviations away from the mean
/// this method updates the z values of each particle
/// </summary>
/// <param name="standard_deviation"></param>
void ParticleController::update_particle_z_value(float standard_deviation) {
	
	for (Particle& p : particles) {
		float z = p.get_kinetic_energy() - average_ke_all_particles;
		if (standard_deviation > 0.00001) {
			z /= standard_deviation;
		}
		p.z_value = z;
	}
}

/// <summary>
/// Find the colour of each particle based on the z value
/// </summary>
/// <param name="p"></param>
/// <returns></returns>
olc::Pixel ParticleController::get_particle_colour(Particle& p) {
	//the index ranges from 0 to 12 so the max z value needs to be added to the z value to get in this range
	float modified_z_value = std::max(p.z_value + MAX_Z_VALUE/2, 0.0f);
	//Make sure the value is in range 0 <= z <= max_z_value
	modified_z_value = std::min(modified_z_value, MAX_Z_VALUE);
	//Divide by 12 and round it to the nearest integer to find the index 
	int index = std::round(modified_z_value / (MAX_Z_VALUE/12));
	index = std::min(std::max(index, 0),12);
	return colours[index];
}

/// <summary>
/// Caclulate average speed of particles 
/// </summary>
/// <returns></returns>
float ParticleController::calc_rms() {
	float total = 0;
	for (Particle& p : particles) {
		total += pow(p.get_velocity().mag(), 2);
	}
	//if particles.size() = 0 then the returned value would be infinity
	if (particles.size() > 0) {
		total /= particles.size();
		return pow(total, 0.5);
	}
	else {
		return 0;
	}
}