//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Math.hpp                                                                    //
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

// OpenGL Math
#include "glm\glm.hpp"
#include "glm\vec2.hpp"
#include "glm\vec4.hpp"
#include "glm\vec3.hpp"
#include "glm\matrix.hpp"
#include "glm\detail\func_trigonometric.hpp"
#include "glm\detail\type_mat4x3.hpp"

#include <vector>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679
#define M_PI180 (M_PI / 180.0)
#endif

namespace Precision
{
    template <typename T>
    static inline T roundif(T value, T limit)
    {
        long long v = (value + .5);
        if (value >= v)
            return value - v >= limit ? value : v;
        else
            return v - value >= limit ? value : v;
    }
};

namespace Matrix
{
    static glm::mat4 Identity()
    {
        return glm::mat4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    }

    static glm::mat4 Scale(float s)
    {
        return glm::mat4(s, 0, 0, 0,
                         0, s, 0, 0,
                         0, 0, s, 0,
                         0, 0, 0, 1);
    }

    static glm::mat4 RotateX(float rads)
    {
        float sin = glm::sin(rads);
        float cos = glm::cos(rads);
        return glm::mat4(1, 0, 0, 0,
                         0, cos, sin, 0,
                         0, -sin, cos, 0,
                         0, 0, 0, 1);
    }

    static glm::mat4 RotateY(float rads)
    {
        float sin = glm::sin(rads);
        float cos = glm::cos(rads);
        return glm::mat4(cos, 0, -sin, 0,
                         0, 1, 0, 0,
                         sin, 0, cos, 0,
                         0, 0, 0, 1);
    }

    static glm::mat4 RotateZ(float rads)
    {
        float sin = glm::sin(rads);
        float cos = glm::cos(rads);
        return glm::mat4(cos, sin, 0, 0,
                         -sin, cos, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    }

    static glm::mat4 Translate(float dx, float dy, float dz)
    {
        return glm::mat4(1, 0, 0, dx,
                         0, 1, 0, dy,
                         0, 0, 1, dz,
                         0, 0, 0, 1);
    }
};

namespace MathFunctions
{
    // Parametric equation of a sphere using spherical coordinates
    static glm::vec3 Sphere(float radius, float phi, float theta)
    {
        float cos_theta = glm::cos(theta);
        float cos_phi = glm::cos(phi);
        float sin_theta = glm::sin(theta);
        float sin_phi = glm::sin(phi);

        glm::vec3 result = glm::vec3(
            cos_phi * sin_theta,
            sin_phi * sin_theta,
            cos_theta
            );

        return result * radius;
    }

    template <typename T>
    inline static T atan2(T y, T x)
    {
        if (x > (T) 0) return glm::atan<float>(y / x);
        if (x < (T) 0)
        {
            if (y >= (T) 0) return glm::atan<float>(y / x) + M_PI;
            else return glm::atan<float>(y / x) - M_PI;
        }
        if (y > (T) 0) return (T) M_PI / (T) 2;
        if (y > (T) 0) return (T) -M_PI / (T) 2;
        return NAN;
    }
};

namespace LinearAlgebra
{
    static std::vector<glm::vec3> GenerateOrthonormalBase(glm::vec3 &u)
    {
        // There are infinite solutions for a zero vector, using NAN will make the error apparent
        if (u.x == 0.0 && u.y == 0.0 && u.z == 0.0)
            return {glm::vec3(NAN, NAN, NAN), glm::vec3(NAN, NAN, NAN), glm::vec3(NAN, NAN, NAN)};

        glm::vec3 *v;
        if (u.y == 0.0)
            v = &glm::vec3(-u.z, 0.0, u.x);
        else if (u.z == 0.0)
            v = &glm::vec3(-u.y, u.x, 0);
        else v = &glm::vec3(0, -u.z, u.y);
        glm::vec3 w = glm::cross(u, *v);

        return {glm::normalize(u), glm::normalize(*v), glm::normalize(w)};
    }
};