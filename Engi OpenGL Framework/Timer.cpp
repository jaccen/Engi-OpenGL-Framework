//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Timer.cpp                                                                   //
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

#include "Timer.hpp"

using namespace std::chrono;

Timer::Timer()
{
    watch = high_resolution_clock::now();
}


Timer::~Timer()
{}

void Timer::Reset()
{
    watch = high_resolution_clock::now();
}

const unsigned long long Timer::Elapsed_s() const
{
    return duration_cast<seconds>(high_resolution_clock::now() - watch).count();
}

const unsigned long long Timer::Elapsed_ms() const
{
    return duration_cast<milliseconds>(high_resolution_clock::now() - watch).count();
}

const unsigned long long Timer::Elapsed_us() const
{
    return duration_cast<microseconds>(high_resolution_clock::now() - watch).count();
}

const unsigned long long Timer::Elapsed_ns() const
{
    return duration_cast<nanoseconds>(high_resolution_clock::now() - watch).count();
}