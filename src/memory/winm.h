
#ifndef MEMORY_WIN16M_H
#define MEMORY_WIN16M_H

#ifdef WIN16_FAKE_DS
typedef HLOCAL MEMORY_HANDLE;
#else
typedef HGLOBAL MEMORY_HANDLE;
#endif /* WIN16_FAKE_DS */

#ifdef ANCIENT_C
typedef char* MEMORY_PTR;
typedef const char* CONST_MEMORY_PTR;
#else
typedef void FAR* MEMORY_PTR;
typedef const void FAR* CONST_MEMORY_PTR;
#endif /* ANCIENT_C */


#endif /* !MEMORY_WIN16M_H */

