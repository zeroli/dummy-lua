#ifndef LUA_STATE_H_
#define LUA_STATE_H_

#include "luaobject.h"

#define LUA_EXTRASPACE sizeof(void*)
#define G(L) ((L)->l_G)

#define STEPMULADJ 200
#define GCSTEPMUL 200
#define GCSTEPSIZE 1024 // 1kb
#define GCPAUSE 100

typedef TValue* StkId;

struct CallInfo {
    StkId func;
    StkId top;
    int nresult;
    int callstatus;
    // next/previous form doubly-linked list for the entire call chain
    struct CallInfo* next;
    struct CallInfo* previous;
};

typedef struct lua_State {
    CommonHeader;  // gc header
    // stack shared with client C
    StkId stack;
    StkId stack_last;
    // global stack top which is ready for filling
    StkId top;
    int stack_size;
    struct lua_longjmp* errorjmp;
    int status;
    // multiple `lua_State`s in one process form a doubly-linked list
    struct lua_State* previous;
    struct CallInfo base_ci;
    // current call frame being executed
    struct CallInfo* ci;
    int nci;
    // global state of this process
    struct global_State* l_G;
    ptrdiff_t errorfunc;
    int ncalls;

    struct GCObject* gclist;
} lua_State;

typedef struct global_State {
    struct lua_State* mainthread;
    lua_Alloc frealloc;
    void* ud;
    lua_CFunction panic;

    // gc fields
    lu_byte gcstate;
    lu_byte currentwhite;
    struct GCObject* allgc;  // gc root for linked-list
    struct GCObject** sweepgc;
    struct GCObject* gray;
    struct GCObject* grayagain;
    lu_mem totalbytes;
    l_mem GCdebt;  // GCdebt will be negative
    lu_mem GCmemtrav;  // per gc step traverse memory bytes
    lu_mem GCestimate;  // after finish a gc cylcle, it records total mem bytes
    int GCstepmul;  // ???
} global_State;

union GCUnion {
    struct GCObject gc;
    lua_State th;
};

struct lua_State* lua_newstate(lua_Alloc alloc, void* ud);
void lua_close(struct lua_State* L);

// manipulate stack
void setivalue(StkId target, int integer);
void setfvalue(StkId target, lua_CFunction f);
void setfltvalue(StkId target, float number);
void setbvalue(StkId target, bool b);
void setnilvalue(StkId target);
void setpvalue(StkId target, void* p);
void setobj(StkId target, StkId value);

// client data in c => stack
void increase_top(struct lua_State* L);
void lua_pushcfunction(struct lua_State* L, lua_CFunction f);
void lua_pushinteger(struct lua_State* L, int integer);
void lua_pushnumber(struct lua_State* L, float number);
void lua_pushboolean(struct lua_State* L, bool b);
void lua_pushnil(struct lua_State* L);
void lua_pushlightuserdata(struct lua_State* L, void* p);

// stack => client c
lua_Integer lua_tointegerx(struct lua_State* L, int idx, int* isnum);
lua_Number lua_tonumberx(struct lua_State* L, int idx, int* isnum);
bool lua_toboolean(struct lua_State* L, int idx);
int lua_isnil(struct lua_State* L, int idx);

// stack
void lua_settop(struct lua_State* L, int idx);
int lua_gettop(struct lua_State* L);
void lua_pop(struct lua_State* L);

#endif  // LUA_STATE_H_
