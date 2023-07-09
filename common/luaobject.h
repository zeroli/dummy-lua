#ifndef LUA_OBJECT_H_
#define LUA_OBJECT_H_

#include "lua.h"

typedef struct lua_State lua_State;

typedef LUA_INTEGER lua_Integer;
typedef LUA_NUMBER lua_Number;
typedef unsigned char lu_byte;
typedef int (*lua_CFunction)(lua_State* L);
typedef void* (*lua_Alloc)(void* ud, void* ptr, size_t oldsize, size_t nsize);

// lua number types
#define LUA_NUMINT (LUA_TNUMBER | (0 << 4))
#define LUA_NUMFLT (LUA_TNUMBER | (1 << 4))

// lua function types
#define LUA_TLCL (LUA_TFUNCTION | (0 << 4))
#define LUA_TLCF (LUA_TFUNCTION | (1 << 4))
#define LUA_TCCL (LUA_TFUNCTION | (2 << 4))

// string types
#define LUA_LNGSTR (LUA_TSTRING | (0 << 4))  // long string
#define LUA_SHRSTR (LUA_TSTRING | (1 << 4))  // short string

#define CommonHeader struct GCObject* next; lu_byte tt_; lu_byte marked
#define LUA_GCSTEPMUL 200

struct GCObject {
    CommonHeader;
};

// union value type
typedef union lua_Value {
    struct GCObject* gc;
    void* p;
    int b;
    lua_Integer i;
    lua_Number n;
    lua_CFunction f;
} Value;

// tagged union value
typedef struct lua_TValue {
    Value value_;
    int tt_;
} TValue;

typedef struct TString {
    int test_field1;
    int test_field2;
} TString;

#endif  // LUA_OBJECT_H_
