/*
** $Id: lzio.c,v 1.14 2001/03/26 14:31:49 roberto Exp $
** a generic input stream interface
** See Copyright Notice in lua.h
*/



#include <stdio.h>
#include <string.h>

#define LUA_PRIVATE
#include "lua.h"

#include "lzio.h"

extern int   CryPakFEof(FILE* handle);
extern size_t CryPakFRead(void* data, size_t length, size_t elems, FILE* handle);

/* ----------------------------------------------------- memory buffers --- */

static int zmfilbuf (ZIO* z) {
  (void)z;  /* to avoid warnings */
  return EOZ;
}


ZIO* zmopen (ZIO* z, const char* b, size_t size, const char *name) {
  if (b==NULL) return NULL;
  z->n = size;
  z->p = (const unsigned char *)b;
  z->filbuf = zmfilbuf;
  z->u = NULL;
  z->name = name;
  return z;
}

/* ------------------------------------------------------------ strings --- */

ZIO* zsopen (ZIO* z, const char* s, const char *name) {
  if (s==NULL) return NULL;
  return zmopen(z, s, strlen(s), name);
}

/* -------------------------------------------------------------- FILEs --- */

static int zffilbuf (ZIO* z) {
  size_t n;
  if (CryPakFEof((FILE *)z->u)) return EOZ;
  n = CryPakFRead(z->buffer, 1, ZBSIZE, (FILE *)z->u);
  if (n==0) return EOZ;
  z->n = n-1;
  z->p = z->buffer;
  return *(z->p++);
}


ZIO* zFopen (ZIO* z, FILE* f, const char *name) {
  if (f==NULL) return NULL;
  z->n = 0;
  z->p = z->buffer;
  z->filbuf = zffilbuf;
  z->u = f;
  z->name = name;
  return z;
}


/* --------------------------------------------------------------- read --- */
size_t zread (ZIO *z, void *b, size_t n) {
  while (n) {
    size_t m;
    if (z->n == 0) {
      if (z->filbuf(z) == EOZ)
        return n;  /* return number of missing bytes */
      else {
        ++z->n;  /* filbuf removed first byte; put back it */
        --z->p;
      }
    }
    m = (n <= z->n) ? n : z->n;  /* min. between n and z->n */
    memcpy(b, z->p, m);
    z->n -= m;
    z->p += m;
    b = (char *)b + m;
    n -= m;
  }
  return 0;
}
