#include <iostream>
#include <stdio.h>
#include "lua.hpp"

static int l_print(lua_State* L) {
	const char* str = luaL_checkstring(L, 1);
	//std::cout << str << std::endl;
	std::cout << str;
	return 0;
}

int main(int argc, char* argv[]) {

	lua_State* L;

	L = luaL_newstate();
	luaL_openlibs(L);

	lua_register(L, "print", l_print);

	luaL_dostring(L, "while true do print(\"hello world\\n\") end");

	lua_close(L);

	getchar();

	return 0;
}
