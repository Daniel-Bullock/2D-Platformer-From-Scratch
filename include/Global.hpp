#ifndef _Global_H_
#define _Global_H_

#include <chrono>

const int TILE_SIZE = 16;

const int MAP_WIDTH = 40;

const int MAP_HEIGHT = 28;

const int WINDOW_WIDTH = 640;

const int WINDOW_HEIGHT = 360;


//Frame duration for ensuring frame independence
//constexpr std::chrono::microseconds FRAME_DURATION(300);
constexpr std::chrono::microseconds FRAME_DURATION(300);

#endif