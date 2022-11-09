#include "luastate.hpp"

namespace QuantIO {
	Luastate::Luastate(lua_State* L) : L_(L) {
		luaL_openlibs(L_);
	}
	Luastate::~Luastate() {
		lua_close(L_);
	}
}