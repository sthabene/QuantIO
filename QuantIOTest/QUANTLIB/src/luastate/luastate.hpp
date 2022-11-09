#pragma once

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace QuantIO {
    class Luastate final { // final means it cannot be derived
    public:
        Luastate(lua_State* L); // make non creatable
        ~Luastate();

    public:
        lua_State* L_ = nullptr;
    };
};

//Lua Connection
//lua_State* L = luaL_newstate();
//QuantIO::Luastate luaState(luaL_newstate());