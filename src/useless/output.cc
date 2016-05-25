#include "output.h"
#include <mysql/plugin.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char * mysql_sphere_version_() {
	char * buffer = ( char * ) malloc (255);
	sprintf ( buffer, "mysql_healpix 0.0.1");
	return ( buffer );
}
