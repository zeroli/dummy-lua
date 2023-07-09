#include "luado.h"
#include "../common/luamem.h"

#define LUA_TRY(L, c, a) if (_setjmp((c)->b) == 0) { a }

#ifdef _WINDOWS_PLATFORM_
#define LUA_THROW(c) longjmp((c)->b, 1)
#else
#define LUA_THROW(c) _longjmp((c)->b, 1)
#endif

struct lua_longjmp {
    struct lua_longjmp* previous;
    jmp_buf b;
    int status;
};

void seterrobj(struct lua_State* L, int error) {
    lua_pushinteger(L, error);
}

void luaD_checkstack(struct lua_State* L, int need) {
    if (L->top + need > L->stack_last) {
        luaD_growstack(L, need);
    }
}

void luaD_growstack(struct lua_State* L, int size) {
    if (L->stack_size > LUA_MAXSTACK) {
        luaD_throw(L, LUA_ERRERR);
    }

    int stack_size = L->stack_size * 2;
    int need_size = cast(int, L->top - L->stack) + size + LUA_EXTRASTACK;
    if (stack_size < need_size) {
        stack_size = need_size;
    }
    if (stack_size > LUA_MAXSTACK) {
        stack_size = LUA_MAXSTACK + LUA_ERRORSTACK;
        LUA_ERROR(L, "stack overflow");
    }

    TValue* old_stack = L->stack;
    L->stack = luaM_realloc(L, L->stack, L->stack_size, stack_size * sizeof(TValue));
    L->stack_size = stack_size;
    L->stack_last = L->stack + stack_size - LUA_EXTRASTACK;
    int top_diff = cast(int, L->top - old_stack);
    L->top = restorestack(L, top_diff);

    // update call info frame, make func/top point to new stack
    struct CallInfo* ci;
    ci = &L->base_ci;
    while (ci) {
        int func_diff = cast(int, ci->func - old_stack);
        int top_diff = cast(int, ci->top - old_stack);
        ci->func = restorestack(L, func_diff);
        ci->top = restorestack(L, top_diff);

        ci = ci->next;
    }
}

void luaD_throw(struct lua_State* L, int error) {
    struct global_State* g = G(L);
    if (L->errorjmp) {
        L->errorjmp->status = error;
        LUA_THROW(L->errorjmp);
    } else {
        if (g->panic) {
            (*g->panic)(L);
        }
        abort();
    }
}

int luaD_rawrunprotected(struct lua_State* L, Pfunc f, void* ud) {
    int old_ncalls = L->ncalls;
    struct lua_longjmp lj;
    lj.previous = L->errorjmp;
    lj.status = LUA_OK;
    L->errorjmp = &lj;

    LUA_TRY(
        L,
        L->errorjmp,
        (*f)(L, ud);
    )

    L->errorjmp = lj.previous;
    L->ncalls = old_ncalls;
    return lj.status;
}

static struct CallInfo* next_ci(struct lua_State* L, StkId func, int nresult) {
    struct global_State* g = G(L);
    struct CallInfo* ci;
    ci = luaM_realloc(L, NULL, 0, sizeof(struct CallInfo));
    ci->next = NULL;
    ci->previous = L->ci;
    L->ci->next = ci;
    ci->nresult = nresult;
    ci->callstatus = LUA_OK;
    ci->func = func;
    ci->top = L->top + LUA_MINSTACK;
    L->ci = ci;

    return ci;
}

int luaD_precall(struct lua_State* L, StkId func, int nresult) {
    switch (func->tt_) {
        case LUA_TLCF:
        {
            lua_CFunction f = func->value_.f;
            // stack might grow, so need update func after check
            ptrdiff_t func_diff = savestack(L, func);
            luaD_checkstack(L, LUA_MINSTACK);
            func = restorestack(L, func_diff);
            // create/prepare new stack frame for function call
            next_ci(L, func, nresult);
            int n = (*f)(L);
            assert(L->ci->func + n <= L->ci->top);
            luaD_poscall(L, L->top - n, n);
            return 1;
        }
        break;
        default:
        break;
    }
}


int luaD_call(struct lua_State* L, StkId func, int nresult) {
    if (++L->ncalls > LUA_MAXCALLS) {
        luaD_throw(L, 0);
    }
    if (!luaD_precall(L, func, nresult)) {
        // TODO:
    }
    L->ncalls--;
    return LUA_OK;
}

static void reset_unuse_stack(struct lua_State* L, ptrdiff_t old_top) {
    struct global_State* g = G(L);
    StkId
}
