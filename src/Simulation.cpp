#include <Simulation.h>

Simulation::Simulation() {
	sAppName = "Simulation"; //Setting the title of the window
	last_frame = std::chrono::high_resolution_clock::now(); //The time of the applications intitialization
	currentMode =  new BoylesMode(container.get_controller(),&container); //Setting the current interface (Would be the default)
}

bool Simulation::OnUserCreate() {
	container.load_state(currentMode->default_state);
	return true; //Simulation has successfully been created
}

bool Simulation::OnUserUpdate(float fElapsedTime) {
	
	//Clear the window with a light grey background
	Clear(olc::Pixel(176,176,176)); 
	
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
	bool found = false;
	int i = 0;
	while (!found && i < currentMode->buttons.size()) {//For every button in the gui check if the coordinates of the mouse are on the button
		Button& b = currentMode->buttons[i];
		if (pos.x > b.get_position().x && pos.x < b.get_position().x + b.get_size().x) {
			if (pos.y > b.get_position().y && pos.y < b.get_position().y + b.get_size().y) {
				if (action) {
					b.pressed(); //If that button was clicked run its corresponding method
					b.clicked = true;
					currentMode->adjustments(b.constant);
				}
				else b.clicked = false;
			}
		}
		i++;
	}
}

void Simulation::DrawContainer() {
	//Recieve the necessary fields of the container to draw it
	olc::vi2d pos = container.get_position();
	olc::vi2d size = { container.get_width(),container.get_height() };
	FillRect(pos, size, olc::BLACK); //Fill it's area with the colour black
	DrawRect(pos, size, olc::WHITE); //Draw a white rectange around it (so the container has a clear border)
}

void  Simulation::DrawGuiRectangle(olc::vi2d pos, olc::vi2d size,olc::vi2d border_pos, olc::vi2d border_size, olc::Pixel colour) {
	FillRectDecal(border_pos, border_size, olc::BLACK);
	FillRectDecal(pos, size, colour);
}

void Simulation::DrawGui() {
	olc::vi2d pos = currentMode->position;
	olc::vi2d size = currentMode->size;

	std::string mode_string = "Current Mode: " + currentMode->name;
	DrawStringDecal({ int(((0.57 * WINDOW_WIDTH) - 200) / 2 + 0.4 * WINDOW_WIDTH), 10 }, mode_string, olc::BLACK);

	DrawGuiRectangle(pos, size, { pos.x - 4,pos.y - 4 }, { size.x + 8,size.y + 8 }, currentMode->colour);


	DrawGuiRectangle({ int(pos.x + 0.03 * size.x), int(pos.y + 0.6 * size.y) }, { int(size.x * 0.94), int(size.y * 0.37)
		}, { int(pos.x - 2 + 0.03 * size.x), int(pos.y - 2 + 0.6 * size.y) }, { int(4 + size.x * 0.94), int(size.y * 0.37) + 4 }, olc::Pixel(47, 101, 133));
	
	

	for (Button& b : currentMode->buttons) {
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


	DrawGuiRectangle(pos, size, { pos.x - 1,int(pos.y - 1) }, { size.x + 4,size.y + 4 }, colour);
	DrawRect(pos, size, olc::BLACK);
	olc::vi2d position = { pos.x + (size.x - int(b.get_text().length() * 8)) / 2,pos.y + size.y / 2 };
	DrawStringDecal(position, b.get_text(), b.get_text_colour(), olc::vf2d{ 1.0f,1.0f });
	
}

/// <summary>
/// Draw each text display onto the screen
/// </summary>
void Simulation::DrawTextDisplays() {
	for (TextDisplay& td : currentMode->text_displays) {
		//Calculate length needed to fit text
		olc::vi2d size = { (int(td.title.length() + td.func().length() + td.unit.length()) + 1) * 8 + 20,30 };
		//Draw dimensions
		DrawGuiRectangle(td.pos, size, { td.pos.x - 2, td.pos.y - 2 }, { size.x + 4,size.y + 4 }, olc::Pixel(240, 240, 240));
		// Concatenate the text that will be displated
		std::string str = td.title + ": " + td.func() + td.unit;
		//Draw the text
		olc::vi2d position = { td.pos.x + (size.x - int((int)str.length() * 8)) / 2,td.pos.y + size.y / 2 };
		DrawStringDecal(position, str, olc::BLACK);

	}
}

void Simulation::DrawHelpDialog() {
	if (container.get_controller()->help_dialog) {
		FillRectDecal({ 20,20 }, { 500,300 }, olc::BLACK);
		FillRectDecal({ 24,24 }, { 492,292 }, olc::WHITE);
		//Draw text
		DrawStringDecal({ 30,30 }, currentMode->help_text, olc::BLACK, {1.0f,1.0f});
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

