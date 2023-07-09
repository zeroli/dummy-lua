#ifndef LUA_MEM_H_
#define LUA_MEM_H_

#include "luastate.h"

#define luaM_free(L, ptr, osize) luaM_realloc(L, ptr, osize, 0)
#define luaM_reallocvector(L, ptr, o, n, t) \
  ((ptr) = (t*)luaM_realloc(L, ptr, o * sizeof(t), n * sizeof(t)))

void* luaM_realloc(struct lua_State* L, void* ptr, size_t osize, size_t nsize);

#endif  // LUA_MEM_H_
