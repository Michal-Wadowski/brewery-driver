#ifndef __DEBUG_H__
#define __DEBUG_H__

inline void dummy(...) {}
#ifdef DEBUG
#include <stdio.h>
#define debug(...) printf(__VA_ARGS__)
#else
#define debug(...) dummy(__VA_ARGS__)
#endif

#endif