#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "debug.h"

bool debugEnabled = false;

void debug(const char * format, ...) {
    if( debugEnabled ) {
        va_list arglist;
        va_start( arglist, format );
        vprintf( format, arglist );
        va_end( arglist );
        fflush(stdout);
    }
}
