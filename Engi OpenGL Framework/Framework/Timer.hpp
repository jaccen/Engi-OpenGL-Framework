// -----------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Timer.h                                                                     //
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

#include <chrono>

class Timer
{
private:
    std::chrono::high_resolution_clock::time_point watch;
public:
    Timer();
    ~Timer();
    void Reset();
    const unsigned long long Elapsed_s() const;
    const unsigned long long Elapsed_ms() const;
    const unsigned long long Elapsed_us() const;
    const unsigned long long Elapsed_ns() const;
};