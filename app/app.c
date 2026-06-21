#include <window/os_window.h>

#include <stdio.h>

#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

int main(int argc, char* argv[]){

    // TODO: Handle given arguments

    // TODO: If no given arguments show help

    // Create a new Lua state
    lua_State *L = luaL_newstate();

    if (L == NULL)
    {
        printf("Failed to create Lua state!\n");
        return -1;
    }

    // Load the Lua libraries (optional but useful for standard functions like print)
    luaL_openlibs(L);

    // Run a Lua script
    if (luaL_dofile(L, "main.lua") != LUA_OK)
    {
        printf("Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1); // Remove error message from the stack
    }

    // Close the Lua state
    lua_close(L);

    return 0;
}
