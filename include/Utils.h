#pragma once
#include <olcPixelGameEngine.h>

#define GRAVITY_CONSTANT 9.8f
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 750

#define CONTAINER_WIDTH 0.38
#define CONTAINER_X_POS 0.025

#define CONTAINER_MIN_Y_POS 0.02

#define SMALL_PARTICLE_SIZE 4
#define LARGE_PARTICLE_SIZE 8
#define SMALL_PARTICLE_MASS 0.001
#define LARGE_PARTICLE_MASS 0.003

#define GUI_POS_X 0.43
#define GUI_POS_Y 0.1
#define GUI_SIZE_X 0.54
#define GUI_SIZE_Y 0.885

#define PI 3.14159





enum PARTICLE_TYPE
{
	LIGHT,
	HEAVY
};

static olc::vi2d get_screen_coords(olc::vf2d position, int height) {
	olc::vi2d ret = { int(position.x / 100 * CONTAINER_WIDTH * WINDOW_WIDTH),int(position.y / (100 * (height / (0.4 * WINDOW_HEIGHT))) * height) };
	ret = { int(ret.x + CONTAINER_X_POS * WINDOW_WIDTH),int(ret.y + WINDOW_HEIGHT - (CONTAINER_MIN_Y_POS * WINDOW_HEIGHT + height)) };
	return ret;
}

static olc::vi2d add_container_margins(olc::vi2d position, int height) {
	olc::vi2d ret = { int(position.x + CONTAINER_X_POS * WINDOW_WIDTH),int(position.y + WINDOW_HEIGHT - (CONTAINER_MIN_Y_POS * WINDOW_HEIGHT + height)) };
	return ret;
}

static float rescale_length(float length) {
	 return (length * 100 / (CONTAINER_WIDTH * WINDOW_WIDTH));
}
static float rescale_height(float old_height, int height) {
	return old_height * ((100 * (height / (WINDOW_HEIGHT * 0.4))) / (height - CONTAINER_MIN_Y_POS * WINDOW_HEIGHT));
}

static olc::vi2d delocalize_position(olc::vf2d position, int height) {
	olc::vi2d ret = { int(position.x / 100 * CONTAINER_WIDTH * WINDOW_WIDTH),int(position.y / (100 * (height / (0.4 * WINDOW_HEIGHT))) * height) };
	return ret;
}
static olc::vf2d localize_position(olc::vi2d position, int height) {
	olc::vf2d ret = { float(position.x - (CONTAINER_X_POS * WINDOW_WIDTH)),float(position.y - (WINDOW_HEIGHT - (CONTAINER_MIN_Y_POS * WINDOW_HEIGHT + height))) };
	ret = { float(ret.x * 100 / (CONTAINER_WIDTH * WINDOW_WIDTH)),float(ret.y * (100 * (height / (0.4 * WINDOW_HEIGHT))) / height) };
	return ret;
}

