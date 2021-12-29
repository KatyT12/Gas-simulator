#include <Simulation.h>




Simulation::Simulation() {
	sAppName = "Simulation"; //Setting the title of the window
	last_frame = std::chrono::high_resolution_clock::now(); //The time of the applications intitialization
	currentMode = testMode(container.get_controller()); //Setting the current interface (Would be the default)
	
}

bool Simulation::OnUserCreate() {
	container.get_controller()->add_particle(PARTICLE_TYPE::LIGHT); //Add a particle to the simulation
	return true; //Simulation has successfully been created
}

bool Simulation::OnUserUpdate(float fElapsedTime) {
	auto current_frame = std::chrono::high_resolution_clock::now(); //record time before the iteration
	time_between_frames  = (float)std::chrono::duration_cast<std::chrono::nanoseconds>(current_frame - last_frame).count() / 1000000000;
	
	//Clear the window with a dark grey background
	Clear(olc::DARK_GREY); 
	container.get_controller()->update(); // Update the simulation

	//Draw the simulation
	DrawContainer(); 
	DrawGui();
	DrawParticles();

	//Check if any buttons have been clicked
	CheckButtonPress();
	
	last_frame = std::chrono::high_resolution_clock::now(); //record time after the iteration

	return true;
}

void Simulation::CheckButtonPress() {
	if (GetMouse(0).bPressed) { // If the left mouse button was pressed
		
		olc::vi2d pos = GetMousePos(); //record mouse position
			for (Button& b : currentMode.buttons) { //For every button in the gui check if the coordinates of the mouse are on the button
				if (pos.x > b.get_position().x && pos.x < b.get_position().x + b.get_size().x) {
					if (pos.y > b.get_position().y && pos.y < b.get_position().y + b.get_size().y) {
						b.pressed(); //If that button was clicked run its corresponding method
					}
				}
			}
	}
}


void Simulation::DrawContainer() {
	//Recieve the necessary fields of the container to draw it
	olc::vi2d pos = container.get_position();
	int height = container.get_height();
	int width = container.get_width();

	FillRect(pos, { width,height }, olc::BLACK); //Fill it's area with the colour black
	DrawRect(pos, { width,height }, olc::WHITE); //Draw a white rectange around it (so the container has a clear border)
}

void Simulation::DrawGui() {
	//filling a rectangle that the interface will be in with a colour
	FillRect({ currentMode.position}, { currentMode.size }, currentMode.colour);
	//Draw each button
	for (Button& b : currentMode.buttons) {
		DrawButton(b);
	}
}

void Simulation::DrawButton(Button b) {
	FillRect(b.get_position(), b.get_size(), b.get_color()); //Fill a rectangle which will represent the button
	//Draw the text on the button
	DrawString({b.get_position().x + b.get_size().x/4,b.get_position().y + b.get_size().y/2}, b.get_text(), olc::BLACK);
}


void Simulation::DrawParticles() {
	//Draw each particle
	
	for (const Particle& p : container.get_controller()->particles) {
		DrawParticle(p);
	}
}
void Simulation::DrawParticle(Particle p) {
	//get the position of the particle
	
	olc::vi2d pos = get_screen_coords({p.get_position()});
	//Draw the particle
	FillCircle(pos, p.get_radius() , olc::GREEN);
	FillCircle(pos + ((float)p.get_radius() * p.get_velocity().norm()), 2, olc::RED);
	
}

//olc::vf2d position = add_container_margins({ int(rand() % int(CONTAINER_WIDTH*WINDOW_WIDTH - SMALL_PARTICLE_SIZE)),int(rand() % int(height - SMALL_PARTICLE_SIZE)) });
//olc::vf2d position = add_container_margins({ int(rand() % int(CONTAINER_WIDTH * WINDOW_WIDTH - LARGE_PARTICLE_SIZE)),int(rand() % int(height - LARGE_PARTICLE_SIZE)) });