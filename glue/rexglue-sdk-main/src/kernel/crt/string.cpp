/**
 * @file        kernel/crt/string.cpp
 *
 * @brief       Native string operation hooks -- replaces recompiled PPC
 *              implementations of strncmp, strchr, lstrlenA, etc.
 *
 * @copyright   Copyright (c) 2026 Tom Clay <tomc@tctechstuff.com>
 * @license     BSD 3-Clause License
 */
#include <cstring>

#include <rex/platform.h>

#if !REX_PLATFORM_WIN32
#include <strings.h>
#endif

#include <rex/hook.h>
#include <rex/string.h>

namespace rex::kernel::crt {

// ---------------------------------------------------------------------------
// C string operations
// ---------------------------------------------------------------------------

static int native_strncmp(const char* s1, const char* s2, size_t n) {
  return std::strncmp(s1, s2, n);
}

static char* native_strncpy(char* dst, const char* src, size_t n) {
#if REX_PLATFORM_WIN32
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  return std::strncpy(dst, src, n);
}

static char* native_strchr(const char* s, int c) {
  return const_cast<char*>(std::strchr(s, c));
}

static char* native_strstr(const char* haystack, const char* needle) {
  return const_cast<char*>(std::strstr(haystack, needle));
}

static char* native_strrchr(const char* s, int c) {
  return const_cast<char*>(std::strrchr(s, c));
}

static char* native_strtok(char* s, const char* delim) {
  // Non-reentrant per guest libc contract; concurrent guest threads will trample.
  static char* context = nullptr;
#if REX_PLATFORM_WIN32
  return strtok_s(s, delim, &context);
#else
  return strtok_r(s, delim, &context);
#endif
}

static int native_stricmp(const char* s1, const char* s2) {
#if REX_PLATFORM_WIN32
  return _stricmp(s1, s2);
#else
  return strcasecmp(s1, s2);
#endif
}

static int native_strcpy_s(char* dst, size_t dstsz, const char* src) {
#if REX_PLATFORM_WIN32
  return strcpy_s(dst, dstsz, src);
#else
  if (!dst || !src || dstsz == 0)
    return 22;  // EINVAL
  const size_t src_len = std::strlen(src);
  if (src_len + 1 > dstsz) {
    dst[0] = '\0';
    return 34;  // ERANGE
  }
  std::memcpy(dst, src, src_len + 1);
  return 0;
#endif
}

// ---------------------------------------------------------------------------
// Win32 string functions (lstr*)
// ---------------------------------------------------------------------------

static int native_lstrlenA(const char* s) {
  return s ? static_cast<int>(std::strlen(s)) : 0;
}

static char* native_lstrcpyA(char* dst, const char* src) {
  // Unbounded by guest contract.
#if REX_PLATFORM_WIN32
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  return std::strcpy(dst, src);
}

static char* native_lstrcpynA(char* dst, const char* src, int maxlen) {
  if (maxlen <= 0)
    return dst;
#if REX_PLATFORM_WIN32
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  std::strncpy(dst, src, static_cast<size_t>(maxlen) - 1);
  dst[maxlen - 1] = '\0';
  return dst;
}

static char* native_lstrcatA(char* dst, const char* src) {
  // Unbounded by guest contract.
#if REX_PLATFORM_WIN32
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
  return std::strcat(dst, src);
}

static int native_lstrcmpiA(const char* s1, const char* s2) {
#if REX_PLATFORM_WIN32
  return _stricmp(s1, s2);
#else
  return strcasecmp(s1, s2);
#endif
}

// ---------------------------------------------------------------------------
// C UTF-16 Widestring functions (int16_t*)
// ---------------------------------------------------------------------------

static uint32_t native_wcslen(const int16_t* str) {
  const int16_t* last = str;
  while (*last++)
    ;
  return last - str - 1;
}

static uint32_t native_wcscmp(const int16_t* lhs, const int16_t* rhs) {
  while (*lhs && (*lhs == *rhs)) {
    lhs++;
    rhs++;
  }
  return (int)(*lhs) - (int)(*rhs);
}

static uint32_t native_wcsncmp(const int16_t* lhs, const int16_t* rhs, int count) {
  for (; count > 0; count--, lhs++, rhs++) {
    if (*lhs != *rhs)
      return (int)(*lhs) - (int)(*rhs);

    if (*lhs == L'\0')
      return 0;
  }

  return 0;
}

static uint32_t native_wcscoll(const int16_t* lhs, const int16_t* rhs) {
  if (lhs && rhs)
    return native_wcscmp(lhs, rhs);
  return 22;  // EINVAL
}

static int16_t* native_wcschr(const int16_t* str, int16_t ch) {
  while (*str && *str != ch)
    str++;

  return (*str == ch) ? (int16_t*)str : 0;
}

static int16_t* native_wcsrchr(const int16_t* str, int16_t ch) {
  const int16_t* res = 0;

  for (; *str; str++)
    if (*str == ch)
      res = str;

  return (int16_t*)(ch ? res : str);
}

static int16_t* native_wcscpy(const int16_t* src, int16_t* dst) {
  int16_t* d = dst;

  while ((*d++ = *src++))
    ;

  return dst;
}

static int16_t* native_wcsncpy(int16_t* dest, const int16_t* src, int count) {
  int16_t* d = dest;
  while (count-- && (*d++ = *src++))
    ;
  while (count--)
    *d++ = 0;
  return dest;
}

static uint32_t native_wcsncpy_s(int16_t* dst, size_t dstsz, const int16_t* src, size_t count) {
  if (!dst || !src || dstsz == 0)
    return 22;  // EINVAL

  size_t i = 0;

  for (; i < count && i + 1 < dstsz && src[i]; i++)
    dst[i] = src[i];

  if (i < dstsz)
    dst[i] = 0;
  else
    dst[dstsz - 1] = 0;

  if (i < count && src[i] != 0)
    return 34;  // ERANGE

  return 0;
}

static int16_t* native_wcsstr(const int16_t* dest, const int16_t* src) {
  if (!*src)
    return (int16_t*)dest;

  for (; *dest; dest++) {
    const int16_t* d = dest;
    const int16_t* s = src;

    while (*d && *s && *d == *s) {
      d++;
      s++;
    }

    if (!*s)
      return (int16_t*)dest;
  }

  return 0;
}

}  // namespace rex::kernel::crt

REX_HOOK(rexcrt_strncmp, rex::kernel::crt::native_strncmp)
REX_HOOK(rexcrt_strncpy, rex::kernel::crt::native_strncpy)
REX_HOOK(rexcrt_strchr, rex::kernel::crt::native_strchr)
REX_HOOK(rexcrt_strstr, rex::kernel::crt::native_strstr)
REX_HOOK(rexcrt_strrchr, rex::kernel::crt::native_strrchr)
REX_HOOK(rexcrt_strtok, rex::kernel::crt::native_strtok)
REX_HOOK(rexcrt__stricmp, rex::kernel::crt::native_stricmp)
REX_HOOK(rexcrt_strcpy_s, rex::kernel::crt::native_strcpy_s)
REX_HOOK(rexcrt_lstrlenA, rex::kernel::crt::native_lstrlenA)
REX_HOOK(rexcrt_lstrcpyA, rex::kernel::crt::native_lstrcpyA)
REX_HOOK(rexcrt_lstrcpynA, rex::kernel::crt::native_lstrcpynA)
REX_HOOK(rexcrt_lstrcatA, rex::kernel::crt::native_lstrcatA)
REX_HOOK(rexcrt_lstrcmpiA, rex::kernel::crt::native_lstrcmpiA)
REX_HOOK(rexcrt_wcslen, rex::kernel::crt::native_wcslen)
REX_HOOK(rexcrt_wcscmp, rex::kernel::crt::native_wcscmp)
REX_HOOK(rexcrt_wcsncmp, rex::kernel::crt::native_wcsncmp)
REX_HOOK(rexcrt_wcscoll, rex::kernel::crt::native_wcscoll)
REX_HOOK(rexcrt_wcschr, rex::kernel::crt::native_wcschr)
REX_HOOK(rexcrt_wcsrchr, rex::kernel::crt::native_wcsrchr)
REX_HOOK(rexcrt_wcscpy, rex::kernel::crt::native_wcscpy)
REX_HOOK(rexcrt_wcsncpy, rex::kernel::crt::native_wcsncpy)
REX_HOOK(rexcrt_wcsncpy_s, rex::kernel::crt::native_wcsncpy_s)
REX_HOOK(rexcrt_wcsstr, rex::kernel::crt::native_wcsstr)
