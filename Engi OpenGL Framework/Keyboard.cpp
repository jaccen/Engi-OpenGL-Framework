//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Keyboard.cpp                                                                //
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

#include "Keyboard.hpp"
#include "Utility.h"

using namespace Keyboard;

KeyboardClient::KeyboardClient(KeyboardServer &kbds) : kbds(kbds)
{}

KeyboardClient::~KeyboardClient()
{}

KeyState KeyboardClient::GetKeyState(unsigned char key) const
{
    return kbds.keys[key];
}

bool Keyboard::KeyboardClient::IsPressed(unsigned char key) const
{
    return kbds.keys[key] == KEY_DOWN || kbds.keys[key] == KEY_HELDDOWN;
}

KeyboardServer::KeyboardServer(unsigned max_buffer_size) : max_buffer_size(max_buffer_size)
{
    keys = new KeyState[256];
    memset(keys, 0, sizeof(KeyState) * 256);
    buffer = new std::queue<KeyStruct>();
    updates = new std::queue<unsigned char>();
    m = new std::mutex();
}

KeyboardServer::~KeyboardServer()
{
    SafeDeleteArray(keys);
    SafeDelete(buffer);
    SafeDelete(updates);
    SafeDelete(m);
}

void KeyboardServer::OnKeyDown(unsigned char key)
{
    if (buffer->size() <= max_buffer_size)
    {
        m->lock();
        buffer->push(KeyStruct(key, KEY_DOWN));
        m->unlock();
    }
}

void KeyboardServer::OnKeyUp(unsigned char key)
{
    if (buffer->size() <= max_buffer_size)
    {
        m->lock();
        buffer->push(KeyStruct(key, KEY_UP));
        m->unlock();
    }
}

void KeyboardServer::UpdateState()
{
    // Updating all key states
    while (!updates->empty())
    {
        unsigned char key = updates->front();
        switch (keys[key])
        {
        case KEY_DOWN:
            keys[key] = KEY_HELDDOWN;
            break;
        case KEY_UP:
            keys[key] = KEY_INACTIVE;
            break;
        default:
            break;
        }
        updates->pop();
    }

    // Unloading buffer
    m->lock();
    while (!buffer->empty())
    {
        KeyStruct ks = buffer->front();
        if (!(keys[ks.key] == KEY_HELDDOWN && ks.state == KEY_DOWN))
        {
            updates->push(ks.key);
            keys[ks.key] = ks.state;
        }
        buffer->pop();
    }
    m->unlock();
}