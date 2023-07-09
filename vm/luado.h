#ifndef LUA_DO_H_
#define LUA_DO_H_

#include "../common/luastate.h"

typedef int (*Pfunc)(struct lua_State* L, void* ud);

void seterrobj(struct lua_State* L, int error);
void luaD_checkstack(struct lua_State* L, int need);
void luaD_growstack(struct lua_State* L, int size);
void luaD_throw(struct lua_State* L, int error);

int luaD_rawunprotected(struct lua_State* L, Pfunc f, void* ud);
int luaD_precall(struct lua_State* L, StkId func, int nresult);
int luaD_poscall(struc lua_State* L, StkId first_result, int nresult);
int luaD_call(struct lua_State* L, StkId func, int nresult);
int luaD_pcall(struct lua_State* L, Pfunc f, void* ud, ptrdiff_t oldtop, ptrdiff_t ef);

#endif  // LUA_DO_H_
