//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Mouse.h                                                                     //
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

namespace Mouse
{
    class MouseServer;
    class MouseClient;

    enum ButtonState
    {
        MB_INACTIVE = 0,
        MB_DOWN,
        MB_HELDDOWN,
        MB_UP
    };

    typedef struct Point
    {
        Point() : x(0), y(0)
        {}
        Point(int x, int y) : x(x), y(y)
        {}
        int x;
        int y;
    }
    Point;

    class MouseClient
    {
    private:
        MouseServer &ms;
    protected:
    public:
        MouseClient(MouseServer &ms);
        ~MouseClient();
        ButtonState GetRightButton() const;
        ButtonState GetLeftButton() const;
        ButtonState GetMiddleButton() const;
        const Point& GetPosition() const;
    };

    class MouseServer
    {
    private:
        friend MouseClient;
        Point position;
        ButtonState rb;
        ButtonState lb;
        ButtonState mb;
    protected:
    public:
        MouseServer();
        ~MouseServer();
        void RightButtonDown();
        void RightButtonUp();
        void LeftButtonDown();
        void LeftButtonUp();
        void MiddleButtonDown();
        void MiddleButtonUp();
        void MoveTo(Point &position);
        void MoveTo(int x, int y);
        void UpdateState();
    };
};