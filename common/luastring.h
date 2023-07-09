#ifndef LUA_STRING_H_
#define LUA_STRING_H_

#include "luastate.h"
#include "../vm/luagc.h"

#define sizelstring(l) (sizeof(TString) + (l + 1) * sizeof(char))
#define getstr(ts) (ts->data)

void luaS_init(struct lua_State* L);
int luaS_resize(struct lua_State* L, unsigned int nsize);
struct TString* luaS_newlstr(struct lua_State* L, const char* str, unsigned int l);
struct TString* luaS_new(struct lua_State* L, const char* str, unsigned int l);
void luaS_remove(struct lua_State* L, struct TString* ts);

void luaS_clearcache(struct lua_State* L);
int luaS_eqshrstr(struct lua_State* L, struct TString* a, struct TString* b);
int luaS_eqlngstr(struct lua_State* L, struct TString* a, struct TString* b);

unsigned int luaS_hash(struct lua_State* L, const char* str, unsigned int l, unsigned int h);
unsigned int luaS_hashlongstr(struct lua_State* L, struct TString* ts);
struct TString* luaS_createlongstr(struct lua_State* L, const char* str, size_t l);

#endif  // LUA_STRING_H_
