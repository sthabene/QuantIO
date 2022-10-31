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
--]