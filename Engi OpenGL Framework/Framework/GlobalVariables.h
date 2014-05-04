// -----------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  GlobalVariables.h                                                           //
//                                                                              //
//  This file is part of Engi OpenGL Framework                                  //
//                                                                              //
//  This program is free software: you can redistribute it and/or modify        //
//      it under the terms of the GNU General Public License as published by    //
//      the Free Software Foundation, either version 3 of the License, or       //
//      (at your option) any later version.                                     //
//                                                                              //
//      This program is distributed in the hope that it will be useful,         //
//      but WITHOUT ANY WARRANTY; without even the implied warranty of          //
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           //
//      GNU General Public License for more details.                            //
//                                                                              //
//      You should have received a copy of the GNU General Public License       //
//      along with this program.  If not, see <http://www.gnu.org/licenses/>    //
//------------------------------------------------------------------------------//

#pragma once

#include "Logger.hpp"

// TODO: configuration file
#define WIDTH 640                           // Client area width in pixels
#define HEIGHT 480                          // Client area height in pixels
#define FPS 60                              // Target FPS

extern bool bQuit;                          // Flag to exit the application
extern Logger logger;                       // Standard log file