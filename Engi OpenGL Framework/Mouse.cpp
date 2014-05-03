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

const Point& Mouse::MouseClient::GetDifference() const
{
    return ms.diff;
}

MouseServer::MouseServer() : rb(MB_INACTIVE), lb(MB_INACTIVE), mb(MB_INACTIVE), position(Point())
{}

Mouse::MouseServer::MouseServer(int x, int y) : rb(MB_INACTIVE), lb(MB_INACTIVE), mb(MB_INACTIVE), position(Point(x, y)), initial(Point(x, y))
{}

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

void Mouse::MouseServer::MoveTo(int x, int y)
{
    diff.x = this->position.x - x;
    diff.y = this->position.y - y;
    this->position = Point(x, y);
}

void Mouse::MouseServer::Movement(int x, int y)
{
    position.x = (diff.x = x);
    position.y = (diff.y = y);
}

void Mouse::MouseServer::UpdateState()
{
    if (rb == MB_DOWN) rb = MB_HELDDOWN;
    else if (rb == MB_UP) rb = MB_INACTIVE;

    if (lb == MB_DOWN) lb = MB_HELDDOWN;
    else if (lb == MB_UP) lb = MB_INACTIVE;

    diff.x = 0;
    diff.y = 0;
}