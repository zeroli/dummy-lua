#ifndef LUA_MEM_H_
#define LUA_MEM_H_

#include "luastate.h"

void* luaM_realloc(struct lua_State* L, void* ptr, size_t osize, size_t nsize);

#endif  // LUA_MEM_H_
