player = { Name = "Sibusiso", Surname = "Thabene", Age = 22}
a = 22 + math.sin(33)

function addStuff(a, b)
	return a + b * 100
end

function doAThing(a, b) 
	c = hostFunction(a, b + 1)
	return c
end

--[[



bool checkLua(lua_State* L, int r) {
	if (r != LUA_OK) {
		std::cout << lua_tostring(L, -1) << std::endl;
		return false;
	}
	return true;
}

int lua_hostFunction(lua_State* L) {
	float a = (float)lua_tonumber(L, 1);
	float b = (float)lua_tonumber(L, 2);
	float c = a + b;
	lua_pushnumber(L, c);
	return 1; //We return the number of arguments that will be passed to lua, so we are pushing only one result
}

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


	//if (checkLua(L, luaL_dofile(L, "src//testing.lua"))) { //dostring runs the command
	//	lua_getglobal(L, "a"); //This is because a is a global variable, and here we are putting it on top, at -1
	//	if (lua_isnumber(L, -1)) { //Here we expect the variable to be a number, so we check this, and its at the top -1
	//		float a_in_cpp = (float)lua_tonumber(L, -1);
	//		std::cout << a_in_cpp << std::endl;
	//	}
	//}

	//if (checkLua(L, luaL_dofile(L, "src//testing.lua"))) { //dostring runs the command
	//	lua_getglobal(L, "addStuff"); //This is because a is a global variable, and here we are putting it on top, at -1
	//	if (lua_isfunction(L, -1)) { //Here we expect the variable to be a number, so we check this, and its at the top -1
	//		lua_pushnumber(L, 3.5f); //pushes a number to the lua stack
	//		lua_pushnumber(L, 7.1f); //pushes another number to the lua stack

	//		if (checkLua(L, lua_pcall(L, 2, 1, 0))) { //pcall 2 = #of arguments, 1 = #return 
	//			//And because we've told lua that the function has two arguments, it will take the ones in the stack
	//			std::cout << (float)lua_tonumber(L, -1) << std::endl;
	//		}
	//	}
	//}

	//lua_register(L, "hostFunction", lua_hostFunction); //Registers and links the c++ function with the lua function

	//if (checkLua(L, luaL_dofile(L, "src//testing.lua"))) { //dostring runs the command
	//	lua_getglobal(L, "doAThing"); //This is because a is a global variable, and here we are putting it on top, at -1
	//	if (lua_isfunction(L, -1)) { //Here we expect the variable to be a number, so we check this, and its at the top -1
	//		lua_pushnumber(L, 3.0f); //pushes a number to the lua stack
	//		lua_pushnumber(L, 7.0f); //pushes another number to the lua stack

	//		if (checkLua(L, lua_pcall(L, 2, 1, 0))) { //pcall 2 = #of arguments, 1 = #return
	//			std::cout << (float)lua_tonumber(L, -1) << std::endl;
	//		}
	//	}
	//}

	lua_pushboolean(L, 1);
	lua_pushnumber(L, 10);
	lua_pushnil(L);
	lua_pushstring(L, "hello");
	stackDump(L);

	lua_pushboolean(L, 1);
	lua_pushboolean(L, 1);
	lua_pushstring(L, "last");
	stackDump(L); 
	lua_replace(L, 3); 
	stackDump(L);
	lua_settop(L, 8); 
	stackDump(L); 
	//lua_rotate(L, 4, 1); 
	//stackDump(L); /* will print: true 10 nil true 'hello' nil */
	lua_remove(L, -3); 
	stackDump(L); /* will print: true 10 nil 'hello' nil */
	lua_settop(L, -5); 
	stackDump(L); /* will print: true */


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







--]