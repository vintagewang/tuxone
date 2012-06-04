#ifndef __LIBTUX_USERLOG_HEADER__
#define __LIBTUX_USERLOG_HEADER__
#include <atmi.h>

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdio.h>
extern int   LIBTUX_EXPORT userlog(char* fmt, ...);

#if defined(__cplusplus)
}
#endif

#endif
