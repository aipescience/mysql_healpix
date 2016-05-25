/*****************************************************************
 * *******                  UDF_SPHERE                     *******
 *****************************************************************
 * THD parameter definitions
 * 
 *****************************************************************
 */

#include <mysql/plugin.h>

#ifndef __MYSQL_HEALPIX_H__
#define __MYSQL_HEALPIX_H__

unsigned long mysql_healpix_outmode( MYSQL_THD thd );

unsigned int mysql_healpix_outprec( MYSQL_THD thd );

#endif
