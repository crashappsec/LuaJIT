/*
** Stubs for code that references stdio in libc
** Copyright (C) 2023 Crash Override, Inc.
*/

#define lj_stdio_c
#define LUA_CORE

#include "lua.h"
#include "lj_def.h"
#include "lj_arch.h"

#if defined(LUAJIT_DISABLE_STDIO_FILE)

#include <signal.h>
#include <sys/syscall.h>

int lj_stdio_panic(lua_State *L, void *panic_ud)
{
  syscall(SYS_kill, syscall(SYS_getpid), SIGABRT);
  return 0;
}

long lj_stdio_print(lua_State *L, const void *p, size_t sz, void *print_ud)
{
  syscall(SYS_kill, syscall(SYS_getpid), SIGABRT);
  return -1;
}

#else

int lj_stdio_panic(lua_State *L, void *panic_ud)
{
  const char *s = lua_tostring(L, -1);
  fputs("PANIC: unprotected error in call to Lua API (", stderr);
  fputs(s ? s : "?", stderr);
  fputc(')', stderr); fputc('\n', stderr);
  fflush(stderr);
  return 0;
}

long lj_stdio_print(lua_State *L, const void *p, size_t sz, void *print_ud)
{
  return fwrite(p, 1, sz, stdout);
}

#endif
