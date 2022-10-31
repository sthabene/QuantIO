#include <string>
#include <vector>
#include <iostream>

extern "C" {
#include "lua/include/lua.h"
#include "lua/include/lualib.h"
#include "lua/include/lauxlib.h"
}

#define MAX_COLOR 255

struct Color {
	std::string name;
	unsigned char red;
	unsigned char green;
	unsigned char blue;
};

std::vector<Color> colorTable = {
 {"WHITE", MAX_COLOR, MAX_COLOR, MAX_COLOR},
 {"RED", MAX_COLOR, 0, 0},
 {"GREEN", 0, MAX_COLOR, 0},
 {"BLUE", 0, 0, MAX_COLOR},
 {"", 0, 0, 0}
};


//HELPER FUNCTIONS
//To check the stack
static void stackDump(lua_State* L) {
	int i;
	int top = lua_gettop(L); /* depth of the stack */
	for (i = 1; i <= top; i++) { /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
		case LUA_TSTRING: { /* strings */
			printf("'%s'", lua_tostring(L, i));
			break;
		}
		case LUA_TBOOLEAN: { /* Booleans */
			printf(lua_toboolean(L, i) ? "true" : "false");
			break;
		}
		case LUA_TNUMBER: { /* numbers */
			printf("%g", lua_tonumber(L, i));
			break;
		}
		default: { /* other values */
			printf("%s", lua_typename(L, t));
			break;
		}
		}
		printf(" "); /* put a separator */
	}
	printf("\n"); /* end the listing */
}
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
//Gets the global int variable, returning an error if it can't
int getglobint(lua_State* L, const char* var) {
	int isnum, result;
	lua_getglobal(L, var);
	result = (int)lua_tointegerx(L, -1, &isnum);
	if (!isnum)
		error(L, "'%s' should be a number\n", var);
	lua_pop(L, 1); /* remove result from the stack */
	return result;
}
//Gets the color field by name
int getcolorfield(lua_State* L, const char* key) {
	int result, isnum;
	lua_pushstring(L, key); /* push key */
	lua_gettable(L, -2); /* get background[key] */
	result = (int)(lua_tonumberx(L, -1, &isnum) * MAX_COLOR);
	if (!isnum)
		error(L, "invalid component '%s' in color", key);
	lua_pop(L, 1); /* remove number */
	return result;
}

//Sets the table fields and pushes the index and field value on the stack
void setcolorfield(lua_State* L, const char* index, int value) {
	lua_pushstring(L, index); /* key */
	lua_pushnumber(L, (double)value / MAX_COLOR); /* value */
	lua_settable(L, -3); //takes the table index as an argument and pops the key and the value
}

//Defines a single color, creates a table and sets the appropriate fields and assigns table to the global variable
void setcolor(lua_State* L, struct Color* ct) {
	lua_newtable(L); //Creates an empty table and pushes it on the stack
	setcolorfield(L, "red", ct->red); //Set table field, by setting index and number
	setcolorfield(L, "green", ct->green); //Set another table field 
	setcolorfield(L, "blue", ct->blue); //Set another table field
	lua_setglobal(L, ct->name.c_str()); /* 'name' = table */
		//pops the table and sets it as the value of the global with the given name
}

int main() {

	lua_State* L = luaL_newstate(); //We need to start a lua virtual machine
	luaL_openlibs(L); //Opens a set of libraries that a typical prgram will use

	load(L, "src//colors.lua");

	//int i = 0;
	//while (colorTable[i].name != "") {
	//	setcolor(L, &colorTable[i++]); 
	//}

	/*lua_getglobal(L, "WHITE");
	
	std::cout << getcolorfield(L, "red") << "\n";
	std::cout << getcolorfield(L, "green") << "\n";
	std::cout << getcolorfield(L, "blue") << "\n";

	lua_getglobal(L, "RED");

	std::cout << getcolorfield(L, "red") << "\n";
	std::cout << getcolorfield(L, "green") << "\n";
	std::cout << getcolorfield(L, "blue") << "\n";

	lua_getglobal(L, "ORANGE");

	std::cout << getcolorfield(L, "red") << "\n";
	std::cout << getcolorfield(L, "green") << "\n";
	std::cout << getcolorfield(L, "blue") << "\n";*/


	Color background;

	lua_getglobal(L, "background");
	if (lua_isstring(L, -1)) { 
		const char* name = lua_tostring(L, -1); /* get string */
		int i; 
		for (i = 0; colorTable[i].name != ""; i++) {
			if (strcmp(name, colorTable[i].name.c_str()) == 0)
				break;
		}
		if (colorTable[i].name == "") {/* string not found? */
			error(L, "invalid color name (%s)", colorTable);
		}
		else { 
			background.name = colorTable[i].name;
			background.red = colorTable[i].red;
			background.green = colorTable[i].green;
			background.blue = colorTable[i].blue;
		}
	}
	else if (lua_istable(L, -1)) {
		background.name = "No name";
		background.red = getcolorfield(L, "red");
		background.green = getcolorfield(L, "green");
		background.blue = getcolorfield(L, "blue");
	}
	else {
		error(L, "invalid value for 'background'");
	}

	//Print color
	std::cout << background.name << "\n";
	std::cout << (int)background.red << "\n";
	std::cout << (int)background.green << "\n";
	std::cout << (int)background.blue << "\n";

	lua_close(L); //We need to close the lua_virtual machine

	return 0;
}






