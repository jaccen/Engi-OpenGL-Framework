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

#include "GL\glew.h"
#include "GL\GL.h"
#include "GL\GLU.h"

#include "SOIL.h"

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

namespace GraphicsUtility
{
    static GLuint LoadTexture(const char* file, GLint wrap = GL_REPEAT)
    {
        // Load pixel data from image
        int width, height;
        unsigned char *image = SOIL_load_image(file, &width, &height, NULL, SOIL_LOAD_RGBA);
        if (image)
        {
            // Create one OpenGL texture
            GLuint textureID;
            glGenTextures(1, &textureID);
            // "Bind" the newly created texture : all future texture functions will modify this texture
            glBindTexture(GL_TEXTURE_2D, textureID);
            // Specifying how to interpolate pixels
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            // Specifying how to wrap the texture
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            // Give the image to OpenGL
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
            // Identifier of the texture
            return textureID;
        }
        // Something went wrong
        return 0;
    }
};

namespace Bytes
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