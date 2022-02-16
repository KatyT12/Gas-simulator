#pragma once

#include "olcPixelGameEngine.h" //The class PixelGameEngine is defined in this header file

#include <Container.h>
#include <Utils.h>
#include <Button.h> 
#include <Particle.h>
#include <Gui.h>
#include <testGui.h>

//Header files from the standard library
#include <chrono> //Will be used for timing each frame
#include <vector> //Contains the vector type which is a dynamic array 

extern float time_between_frames;

class Simulation : public olc::PixelGameEngine //Public inheritance 
{
public:
	Container container;
	Simulation();
public:
	//Overriding functions of the PixelGameEngine
	bool OnUserCreate() override;  // Runs once when the simulation is created
	bool OnUserUpdate(float fElapsedTime) override; //Runs every update of the simulation (every frame)
private:
	//Drawing methods
	void DrawContainer();
	void DrawGui();
	void DrawButton(Button b);
	void DrawTextDisplays();
	void DrawParticles();
	void DrawParticle(Particle p);
	void DrawHelpDialog();
	void DrawGuiRectangle(olc::vi2d pos, olc::vi2d size, olc::vi2d border_pos, olc::vi2d border_size,olc::Pixel colour);

	void CheckButtonPress(uint32_t action);

	std::chrono::time_point<std::chrono::steady_clock> last_frame; // The variable to hold the time point of the last frame
	Gui currentMode; //The current interface in use
};
