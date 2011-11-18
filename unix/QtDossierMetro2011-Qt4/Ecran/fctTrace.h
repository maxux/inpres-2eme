#ifndef ECRAN_H
#define ECRAN_H

#ifdef SUN
#include <sys/varargs.h>
#else
#include <stdarg.h>
#endif

#include <stdio.h>

void Trace(const char *pszTrace, ... );
void TraceErr(int,const char*,const char *pszTrace, ... );

#endif
