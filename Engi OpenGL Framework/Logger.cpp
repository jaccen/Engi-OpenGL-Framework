//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1												//
//  WindowsMain.cpp																//
//  																			//
//  This file is part of Engi OpenGL Framework									//
//  																			//
//  This program is free software: you can redistribute it and/or modify		//
//      it under the terms of the GNU General Public License as published by	//
//      the Free Software Foundation, either version 3 of the License, or		//
//      (at your option) any later version.										//
//  																			//
//      This program is distributed in the hope that it will be useful,			//
//      but WITHOUT ANY WARRANTY; without even the implied warranty of			//
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			//
//      GNU General Public License for more details.							//
//  																			//
//      You should have received a copy of the GNU General Public License		//
//      along with this program.  If not, see <http://www.gnu.org/licenses/>	//
//------------------------------------------------------------------------------//

#include "Logger.h"
#include "Utility.h"

using namespace std;

Logger::Logger(const std::string file, bool append)
{
	append ?
		this->file = new ofstream(file, ios::out | ios::app) :
		this->file = new ofstream(file, ios::out);

	m = new mutex();
}

Logger::~Logger()
{
	file->close();
	SafeDelete(file);
	SafeDelete(m);
}

void Logger::LogInformation(const std::string data)
{
	m->lock();
	*file << data;
	m->unlock();
}