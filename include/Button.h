#pragma once

#include <olcPixelGameEngine.h>
#include <ParticleController.h>


#include <string> //To display the text on the button
#include <functional> //To use the std::function type


class Button {
private:
	//Visual properties of the button
	olc::vi2d position;
	olc::vi2d size;
	olc::Pixel colour;
	std::string text;
	olc::Pixel text_colour = olc::BLACK;

	//Procedure that will be called when the button is clicked
	std::function<void(void)> func;
public:
	bool clicked = false;
	Button(){}

	Button(olc::vi2d position, olc::vi2d size, olc::Pixel colour, std::string text)
	:position(position), size(size), colour(colour), text(text){}
	Button(olc::vi2d position, olc::vi2d size, olc::Pixel colour, std::string text,olc::Pixel text_colour)
		:position(position), size(size), colour(colour), text(text), text_colour(text_colour) {}
	//Sets the function that the button will run. This should be done right after the instance is constructed
	void set_pressed_func(std::function<void(void)> func) {
		this->func = func;
	}
	
	inline olc::vi2d get_size() { return size; }
	inline olc::vi2d get_position() { return position; }
	inline olc::Pixel get_color() { return colour; }
	inline std::string get_text() { return text; }
	inline olc::Pixel get_text_colour() { return text_colour; }

	//runs the buttons given function when it is pressed
	void pressed() {
		func();
	}

};
