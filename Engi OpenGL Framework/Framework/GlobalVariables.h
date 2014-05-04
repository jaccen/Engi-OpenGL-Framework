#pragma once

#include "Logger.hpp"

// TODO: configuration file
#define WIDTH 640                                               // Client area width in pixels
#define HEIGHT 480                                              // Client area height in pixels
#define FPS 60                                                  // Target FPS

extern bool bQuit;                          // Flag to exit the application
extern Logger logger;                       // Standard log file