#include <ParticleController.h>


//Constructor
ParticleController::ParticleController() :particle_count(0) {}

//Update simulation
void ParticleController::update() {
	check_collisions(); //Check and handle particle collisions
	for (Particle& p : particles) {
		p.move(time_between_frames,height); //Move each particle
	}
}

bool ParticleController::add_particle(PARTICLE_TYPE type) {

	if (particle_count + 1 <= MAX_PARTICLES_NUM) {
		particle_count += 1;
		srand(time(NULL)); //The time is used so it is different everytime the application is run

		olc::vf2d velocity = { 50,50 };

		//radius & mass changes depending on particle type so an if statement is needed
		if (type == PARTICLE_TYPE::HEAVY) {
			olc::vf2d position = { (float)(rand() % int(100 - rescale_length(2 * LARGE_PARTICLE_SIZE) - 1) + rescale_length(LARGE_PARTICLE_SIZE)), float(rand() % int(100 - rescale_height(2 * LARGE_PARTICLE_SIZE,height)) + rescale_height(LARGE_PARTICLE_SIZE,height)) };
			particles.emplace_back(Particle(0.001, LARGE_PARTICLE_SIZE, position, type));
		}
		else if (type == PARTICLE_TYPE::LIGHT) {
			olc::vf2d position = { (float)(rand() % int(100 - rescale_length(2 * SMALL_PARTICLE_SIZE) - 2) + rescale_length(SMALL_PARTICLE_SIZE)) + 1, float(rand() % int(100 - rescale_height(2 * SMALL_PARTICLE_SIZE,height) - 2) + rescale_height(SMALL_PARTICLE_SIZE,height) + 1) };
			particles.emplace_back(Particle(0.001, SMALL_PARTICLE_SIZE, position, type));

		}

		particles.back().set_velocity(velocity);
		particles.back().set_kinetic_energy(pow(velocity.mag(),2) * 0.5 * particles.back().get_mass());
		//For debugging
		particles.back().id = particle_count;

		return true; //A particle was successfully added to the simulation
	}
	else {
		return false; //A particle couldn't be added to the simulation
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
	/*
		std::cout << "Mass of particle " << b->id << ": " << b->get_mass() << " Mass of particle " << a->id << ": " << a->get_mass() << "\n";
		std::cout << "Particle " << a->id << " energy before: " << collision.a->get_kinetic_energy() << "\n";
		std::cout << "Particle " << b->id << " energy before: " << b->get_kinetic_energy() << "\n";
		std::cout << "Sum before: " << b->get_kinetic_energy() + a->get_kinetic_energy() << "\n\n";*/

		//Temporary variable to store particle A before it gets modified
		olc::vf2d temp = a->get_velocity();
		a->handle_collision(b, time_between_frames, b->get_velocity());
	    b->handle_collision(a, time_between_frames, temp);

		/*
		std::cout << "Particle " << a->id << " energy after: " << collision.a->get_kinetic_energy() << "\n";
		std::cout << "Particle " << b->id << " energy after: " << b->get_kinetic_energy() << "\n";
		std::cout << "Sum after: " << b->get_kinetic_energy() + a->get_kinetic_energy() << "\n\n\n\n";
		*/
		//std::cout << "Collision between particle " << collision.a->id << " and particle " << collision.b->id << " Have collided\n";
	}
	
}


/*Particle id
  Particle Controller update
  Drawing numbers on particles
*/