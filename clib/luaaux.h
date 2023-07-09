#ifndef LUA_AUX_H_
#define LUA_AUX_H_

#include "../common/luastate.h"

struct lua_State* luaL_newstate();
void luaL_close(struct lua_State* L);

void luaL_pushinteger(struct lua_State* L, int integer);
void luaL_pushnumber(struct lua_State* L, float number);
void luaL_pushlightuserdata(struct lua_State* L, void* userdata);
void luaL_pushnil(struct lua_State* L);
void luaL_pushcfunction(struct lua_State* L, lua_CFunction f);
void luaL_pushboolean(struct lua_State* L, bool boolean);
void luaL_pcall(struct lua_State* L, int narg, int nresult);

bool luaL_checkinteger(struct lua_State* L, int idx);
lua_Integer luaL_tointeger(struct lua_State* L, int idx);
lua_Number luaL_tonumber(struct lua_State* L, int idx);
void* lauL_touserdata(struct lua_State* L, int idx);
bool luaL_toboolean(struct lua_State* L, int idx);
int luaL_isnil(struct lua_State* L, int idx);

void luaL_pop(struct lua_State* L);
int luaL_stacksize(struct lua_State* L);

#endif  // LUA_AUX_H_
