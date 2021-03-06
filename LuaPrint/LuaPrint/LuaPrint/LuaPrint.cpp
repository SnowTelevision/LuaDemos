// LuaPrint.cpp : Defines the entry point for the console application.
//

#include "stdio.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

/* the Lua interpreter */
lua_State* L;

int main(int argc, char *argv[])
{
	/* initialize Lua */
	L = luaL_newstate();

	/* load Lua base libraries */
	luaL_openlibs(L);

	/* run the script */
	luaL_dofile(L, "test.lua");

	/* cleanup Lua */
	lua_close(L);

	/* pause */
	printf("Press enter to exit...");
	getchar();

	return 0;
}