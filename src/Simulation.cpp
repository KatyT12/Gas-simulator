#include <Simulation.h>

Simulation::Simulation() {
	sAppName = "Simulation"; //Setting the title of the window
	last_frame = std::chrono::high_resolution_clock::now(); //The time of the applications intitialization
	currentMode = testMode(container.get_controller(),&container); //Setting the current interface (Would be the default)
}

bool Simulation::OnUserCreate() {
	container.get_controller()->add_particle(PARTICLE_TYPE::LIGHT); //Add a particle to the simulation
	return true; //Simulation has successfully been created
}

bool Simulation::OnUserUpdate(float fElapsedTime) {
	
	//Clear the window with a dark grey background
	Clear(olc::Pixel(200,200,200)); 
	
	if (!container.get_controller()->paused) {
		container.get_controller()->update(); // Update the simulation
	}
	

	//Draw the simulation
	DrawContainer(); 
	DrawGui();
	DrawParticles();

	if (GetMouse(0).bPressed) {
		CheckButtonPress(1);
	}
	if (GetMouse(0).bReleased) {
		CheckButtonPress(0);
	}
	
	container.get_controller()->set_time_between_frames(fElapsedTime);

	return true;
}

void Simulation::CheckButtonPress(uint32_t action) {
	olc::vi2d pos = GetMousePos();
	for (Button& b : currentMode.buttons) {//For every button in the gui check if the coordinates of the mouse are on the button
		if (pos.x > b.get_position().x && pos.x < b.get_position().x + b.get_size().x) {
			if (pos.y > b.get_position().y && pos.y < b.get_position().y + b.get_size().y) {
				if (action) { 
					b.pressed(); //If that button was clicked run its corresponding method
					b.clicked = true;
				}
				else b.clicked = false;
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
	olc::vi2d pos = currentMode.position;
	olc::vi2d size = currentMode.size;

	std::string mode_string = "Current Mode: " + currentMode.name;
	DrawString({ int(((0.55 * WINDOW_WIDTH) - 200) / 2 + 0.4 * WINDOW_WIDTH), 10 }, mode_string, olc::BLACK);
	//FillRect({ pos.x - 4,pos.y - 4 }, {size.x+8,size.y+8}, olc::BLACK);
	FillRect(pos, size, currentMode.colour);
	DrawRect({ pos.x - 2,pos.y - 2 }, { size.x + 4,size.y + 3 }, olc::BLACK);
	DrawRect({ pos.x - 1,pos.y  }, { size.x +2 ,size.y +1 }, olc::BLACK);
	for (Button& b : currentMode.buttons) {
		DrawButton(b);
	}
	DrawTextDisplays();
	DrawHelpDialog();
}


void Simulation::DrawButton(Button b) {
	olc::vi2d pos = b.get_position();
	olc::vi2d size = b.get_size();

	olc::Pixel colour = b.get_color();
	if (b.clicked) {
		colour = b.get_color() - olc::Pixel(50, 50, 50);
	}

	FillRect({ pos.x - 1,pos.y - 1 }, { size.x + 4,size.y + 4 }, olc::BLACK);
	FillRect(pos, size, colour);
	
	DrawRect(pos, size, olc::BLACK);
	DrawString({ pos.x + (size.x - int(b.get_text().length() * 8)) / 2,pos.y + size.y / 2 }, b.get_text(), b.get_text_colour(), 1);

}

/// <summary>
/// Draw each text display onto the screen
/// </summary>
void Simulation::DrawTextDisplays() {
	for (TextDisplay& td : currentMode.text_displays) {
		//Calculate length needed to fit text
		olc::vi2d size = { (int(td.title.length() + td.func().length() + td.unit.length()) + 1) * 8 + 20,30 };
		//Draw dimensions
		FillRect({ td.pos.x - 2,td.pos.y - 2 }, { size.x + 4,size.y + 4 }, olc::BLACK);
		FillRect(td.pos, size, olc::Pixel(240,240,240));
		
		// Concatenate the text that will be displated
		std::string str = td.title + ": " + td.func() + td.unit;
		//Draw the text
		DrawString({ td.pos.x + (size.x - int((int)str.length() * 8)) / 2,td.pos.y + size.y / 2 }, str, olc::BLACK);

	}
}

void Simulation::DrawHelpDialog() {
	if (container.get_controller()->help_dialog) {
		FillRect({ 20,20 }, { 500,300 }, olc::GREEN);
		FillRect({ 24,24 }, { 492,292 }, olc::WHITE);
		//Draw text
		DrawString({ 30,30 }, currentMode.help_text,olc::BLACK,1);
	}
}

void Simulation::DrawParticles() {
	//Draw each particle
	
	for (const Particle& p : container.get_controller()->particles) {
		DrawParticle(p);
	}
}
void Simulation::DrawParticle(Particle p) {
	//get the position of the particle
	
	olc::vi2d pos = get_screen_coords({p.get_position()},container.get_height());
	//Draw the particle
	FillCircle(pos, p.get_radius() , olc::GREEN);
}

