#include <string>
#include <vector>
#include <iostream>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//HELPER FUNCTIONS
//Catches all errors
void error(lua_State* L, const char* fmt, ...) {
	va_list argp;
	va_start(argp, fmt);
	vfprintf(stderr, fmt, argp);
	va_end(argp);
	lua_close(L);
	exit(EXIT_FAILURE);
}
//Load a file
void load(lua_State* L, const char* fname) {
	if (luaL_loadfile(L, fname) || lua_pcall(L, 0, 0, 0))
		error(L, "cannot run config. file: %s", lua_tostring(L, -1));
}

double f(lua_State* L, double x, double y) {
	double z;
	int isnum;

	lua_getglobal(L, "f"); /* function to be called */
	lua_pushnumber(L, x); /* push 1st argument */
	lua_pushnumber(L, y); /* push 2nd argument */

	if (lua_pcall(L, 2, 1, 0) != LUA_OK) //passing 2 arguments and returning 1 result
		error(L, "error running function 'f': %s", lua_tostring(L, -1));

	z = lua_tonumberx(L, -1, &isnum);
	if (!isnum)
		error(L, "function 'f' should return a number");
	lua_pop(L, 1); /* pop returned value */
	return z;
}

static int sq(lua_State* L) {
	double d = luaL_checknumber(L, 1);
	lua_pushnumber(L, d*d);
	return 1; /* number of results */
}

int main() {

	lua_State* L = luaL_newstate(); //We need to start a lua virtual machine
	luaL_openlibs(L); //Opens a set of libraries that a typical prgram will use

	lua_pushcfunction(L, sq); //pushes a value of type function
	lua_setglobal(L, "sq"); //assigns the function to the global variable mysin

	std::string expression1 = "function dayCount (Day1, Day2, Month1, Month2, Year1, Year2, Date1, Date2) " 
		"if (Day1 == 31) then Day1 = 30 end \n"
		"if (Day2 == 31 and Day1 == 30) then Day2 = 30 end \n"
		"return 360 * (Year2 - Year1) + 30 * (Month2 - Month1) + (Day2 - Day1) end ";
	std::string expression2 = "r = dayCount(31, 30, 1, 1, 2021, 2023)";

	std::string expression3 = expression1 + ";" + expression2;

	if (luaL_dostring(L, expression3.c_str()) == LUA_OK) {
		lua_getglobal(L, "r"); /* function to be called */
		std::cout << lua_tonumber(L, -1) << "\n";
	}
	else {
		error(L, "Error: %s", lua_tostring(L, -1));
	}


	//load(L, "src//colors.lua");
	//lua_pushnumber(L, 22.0); /* push 1st argument */
	//if (lua_pcall(L, 1, 1, 0) != LUA_OK) //passing 1 argument and returning 1 result
	//	error(L, "error running function %s", lua_tostring(L, -1));
	//int isnum;
	//double z = lua_tonumberx(L, -1, &isnum);

	////lua_tonumber()

	//std::cout << z << "\n";

	lua_close(L); //We need to close the lua_virtual machine

	return 0;
}







