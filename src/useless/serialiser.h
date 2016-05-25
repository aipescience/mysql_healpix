
#include "pixelfunc.h"

#ifndef __MYSQL_HEALPIX_SERIALISER__
#define __MYSQL_HEALPIX_SERIALISER__

char * serialise(SBox * box);
size_t getSerialisedLen(SBox * box);

MYSQL_SPHERE_TYPES decode(char * input, size_t inputLen, void ** output);

#endif
