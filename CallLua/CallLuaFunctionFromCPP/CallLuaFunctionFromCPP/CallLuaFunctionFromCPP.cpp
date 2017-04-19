// CallLuaFunctionFromCPP.cpp : Defines the entry point for the console application.
//
#include "stdio.h"

#include <cstdio>
#include <cassert>

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

/* the Lua interpreter */
lua_State* L;

char* buf = nullptr;
size_t bufSize = 0;
const char * reader(lua_State *L, void *ud, size_t *sz)
{
	if (bufSize > 0)
	{
		*sz = bufSize;
		bufSize = 0;
		return reinterpret_cast<char*>(ud);
	}
	*sz = 0;
	return nullptr;
}

int luaadd(int x, int y)
{
	int sum;

	/* the function name */
	int t = lua_getglobal(L, "add");

	if (lua_isnil(L, -1))
	{
		printf("cannot find 'add'\n");
		return 0;
	}

	/* the first argument */
	lua_pushnumber(L, x);

	/* the second argument */
	lua_pushnumber(L, y);

	/* call the function with 2 arguments, return 1 result */
	lua_call(L, 2, 1);

	/* get the result */
	sum = (int)lua_tointeger(L, -1);
	lua_pop(L, 1);

	return sum;
}


int main(int argc, char *argv[])
{
	int sum;
	int ret = LUA_OK;

	while (true)
	{
		/* initialize Lua */
		L = luaL_newstate();

		/* load Lua base libraries */
		//luaL_openlibs(L);

		/* load the script */
		{
			FILE *fp = fopen("add.lua", "rb");
			assert(fp != nullptr);

			assert(0 == fseek(fp, 0, SEEK_END));
			long size = ftell(fp);
			assert(0 == fseek(fp, 0, SEEK_SET));

			//assert(size > 0);

			char* buf = new char[size];
			size_t total = 0;
			do {
				size_t size_read = fread(buf + total, 1, size - total, fp);
				total += size_read;
			} while (total < size);

			bufSize = size;
			assert(LUA_OK == (ret = lua_load(L, reader, buf, "test_add_lua", 0)));

			fclose(fp);
			assert(LUA_OK == (ret = lua_pcall(L, 0, LUA_MULTRET, 0)));
		}

		/* call the add function */
		sum = luaadd(10, 15);

		delete[] buf;
		buf = nullptr;

		/* print the result */
		printf("The sum is %d\n", sum);

		/* cleanup Lua */
		lua_close(L);
	}

	/* pause */
	printf("Press enter to exit...");
	getchar();

	return 0;
}
