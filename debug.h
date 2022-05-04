#ifndef __DEBUG_H__
#define __DEBUG_H__

inline void dummy(...) {}
#ifdef DEBUG
#include <stdio.h>
#include <stdlib.h>
#define debug(...) do {printf(__VA_ARGS__); fflush(stdin);} while(0)
#else
#define debug(...) dummy(__VA_ARGS__)
#endif

#endif