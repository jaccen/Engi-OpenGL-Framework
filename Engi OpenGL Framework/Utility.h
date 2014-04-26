//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Utility.h                                                                   //
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

#define SafeDelete(object) \
    if (object != nullptr) { \
    delete object; \
    object = nullptr; \
    }

#define SafeDeleteArray(object) \
    if (object != nullptr) { \
    delete[] object; \
    object = nullptr; \
    }

namespace Utility
{
    static bool IsLittleEndian()
    {
        unsigned int i = 1;
        char *c = (char*) &i;
        return (*c == 1);
    }

    static void SwapBytes(unsigned &integer)
    {
        integer = ((integer >> 24) & 0x000000FF) | ((integer >> 8) & 0x0000FF00) | ((integer << 8) & 0x00FF0000) | ((integer << 24) & 0xFF000000);
    }
};