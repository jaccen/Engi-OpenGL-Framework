//------------------------------------------------------------------------------//
//  Engi OpenGL Framework v0.0.1                                                //
//  Keybiard.h                                                                  //
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

#include <queue>
#include <mutex>

// Partially thread-safe, UpdateState() is NOT thread-safe if different threads call it, otherwise it's fine
namespace Keyboard
{
    class KeyboardServer;
    class KeyboardClient;

    enum KeyState
    {
        KEY_INACTIVE = 0,
        KEY_DOWN,
        KEY_HELDDOWN,
        KEY_UP
    };

    class KeyboardServer
    {
    private:
        friend KeyboardClient;
        typedef struct KeyStruct
        {
            KeyStruct(unsigned char key, KeyState state) : key(key), state(state) {}
            unsigned char key;
            KeyState state;
        };
        KeyState *keys;
        // Does not register new keys if the buffer is full
        const unsigned max_buffer_size;
        std::queue<KeyStruct> *buffer;
        std::queue<unsigned char> *updates;
        std::mutex *m;
    protected:
    public:
        KeyboardServer(unsigned max_buffer_size);
        ~KeyboardServer();
        // Adds into the buffer, provided it's not full
        void OnKeyDown(unsigned char key);
        void OnKeyUp(unsigned char key);
        // Updates all keys, then unloads the buffer
        void UpdateState();
    };

    class KeyboardClient
    {
    private:
        KeyboardServer &kbds;
    protected:
    public:
        KeyboardClient(KeyboardServer &kbds);
        ~KeyboardClient();
        KeyState GetKeyState(unsigned char key) const;
    };
};