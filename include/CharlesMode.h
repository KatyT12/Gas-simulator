#pragma once
#include <Gui.h>
#include <Container.h>

//Extends Gui class
static class CharlesMode : public Gui {
public:


	//The controller and container is a needed to access the simulation
	CharlesMode(ParticleController* controller, Container* container) : Gui(olc::Pixel(152, 135, 229), controller, container, { 309,370,150 }) {


		name = "Charles-Law  Mode";
		help_text += "The current mode is the Charles-law mode."
			" Charle's law\n\nstates that as temperature increases,gases will expand\n\n"
			"at a constant pressure.\n\n\n\n"
			"This would apply for all ideal gases however you may notice\n\n"
			"The pressure does change in this mode. This is because the \n\npressure is"
			" calculated from the collisions per second which\n\ncannot be fully controlled."
			" These simulated gas particles,\n\njust like real life gas particles "
			"are not ideal gases as\n\nideal gases are not real.";
		const_variable = CONSTANT::PARTICLE;

	}

	
	void adjustments(CONSTANT constant) {
		// If temperature has been changed, adjust volume
		if (constant == CONSTANT::TEMPERATURE) {
			float volume = controller->adjust_volume(CONSTANT::TEMPERATURE);
			container->change_height(volume);
		}
		// If volume has been changed, adjust temperature
		if (constant == CONSTANT::VOLUME) {		
			int temperature_change = controller->adjust_temperature(CONSTANT::VOLUME);
			controller->increment_temperature(temperature_change);
		}
	}
};


