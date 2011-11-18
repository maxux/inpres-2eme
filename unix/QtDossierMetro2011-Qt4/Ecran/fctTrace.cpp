#include "fctTrace.h"

void Trace(const char *pszTrace, ... )
{
char szBuffer[255];
va_list arg;
va_start(arg,pszTrace);
vsprintf(szBuffer,pszTrace,arg);
fprintf(stderr,"%s\n",szBuffer);
va_end(arg);
return ;
}

void TraceErr(int Ligne,const char*Nom,const char *pszTrace, ... )
{
char szBuffer[255];
va_list arg;
va_start(arg,pszTrace);
vsprintf(szBuffer,pszTrace,arg);
fprintf(stderr,"(Ligne %d de %s)\n %s\n",Ligne,Nom,szBuffer);
perror("");
va_end(arg);
return ;
}
