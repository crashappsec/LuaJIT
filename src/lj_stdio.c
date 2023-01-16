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

// Calls to these functions should never be made. Abort if they are so
// we can figure out why they're getting called.
// 
// As far as I can tell, the only references to these functions are via
// lj_ircall.h. Since the reference is part of one big macro that builds
// an enum, there's no easy way to conditionally exclude the ones we don't
// want, and so this is the solution.

#define lj_stdio_string(_name) #_name

#define lj_stdio_alias(_name) \
  extern __typeof(lj_stdio_##_name) _name __attribute__((__alias__(lj_stdio_string(lj_stdio_##_name))))

int lj_stdio_fputc(int c, void *stream)
{
  syscall2(SYS_kill, syscall0(SYS_getpid), SIGABRT);
  return 0;
}
lj_stdio_alias(fputc);

int lj_stdio_fflush(void *stream)
{
  syscall2(SYS_kill, syscall0(SYS_getpid), SIGABRT);
  return 0;
}
lj_stdio_alias(fflush);

long lj_stdio_fwrite(const void *ptr, size_t size, size_t nmemb, void *stream)
{
  syscall2(SYS_kill, syscall0(SYS_getpid), SIGABRT);
  return 0;
}
lj_stdio_alias(fwrite);

int lj_stdio_panic(lua_State *L, void *panic_ud)
{
  syscall2(SYS_kill, syscall0(SYS_getpid), SIGABRT);
  return 0;
}

long lj_stdio_print(lua_State *L, const void *p, size_t sz, void *print_ud)
{
  syscall2(SYS_kill, syscall0(SYS_getpid), SIGABRT);
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
