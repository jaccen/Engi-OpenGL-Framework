//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Mouse.cpp                                                                   //
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

#include "Mouse.hpp"

using namespace Mouse;

MouseClient::MouseClient(MouseServer &ms) : ms(ms)
{}

MouseClient::~MouseClient()
{}

ButtonState MouseClient::GetRightButton() const
{
    return ms.rb;
}

ButtonState MouseClient::GetLeftButton() const
{
    return ms.lb;
}

ButtonState MouseClient::GetMiddleButton() const
{
    return ms.mb;
}

const Point& MouseClient::GetPosition() const
{
    return ms.position;
}

MouseServer::MouseServer()
{
    position = Point();
    rb = MB_INACTIVE;
    lb = MB_INACTIVE;
    mb = MB_INACTIVE;
}

MouseServer::~MouseServer()
{}

void MouseServer::RightButtonDown()
{
    rb = MB_DOWN;
}

void MouseServer::RightButtonUp()
{
    rb = MB_UP;
}

void MouseServer::LeftButtonDown()
{
    lb = MB_DOWN;
}

void MouseServer::LeftButtonUp()
{
    lb = MB_UP;
}

void Mouse::MouseServer::MiddleButtonDown()
{
    mb = MB_DOWN;
}

void Mouse::MouseServer::MiddleButtonUp()
{
    mb = MB_UP;
}

void MouseServer::MoveTo(Point &position)
{
    this->position = position;
}

void Mouse::MouseServer::MoveTo(int x, int y)
{
    this->position = Point(x, y);
}

void Mouse::MouseServer::UpdateState()
{
    if (rb == MB_DOWN) rb = MB_HELDDOWN;
    else if (rb == MB_UP) rb = MB_INACTIVE;

    if (lb == MB_DOWN) lb = MB_HELDDOWN;
    else if (lb == MB_UP) lb = MB_INACTIVE;
}