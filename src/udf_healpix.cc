#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include <mysql/plugin.h>
#include <mysql_version.h>

#define MYSQL_SERVER 1

struct st_mysql_daemon vars_plugin_info = {MYSQL_DAEMON_INTERFACE_VERSION};

mysql_declare_plugin(vars) {
	MYSQL_DAEMON_PLUGIN,
	&vars_plugin_info,
	"HEALPix",
	"Gal Matijevic",
	"HEALPix for MySQL",
	PLUGIN_LICENSE_GPL,
	NULL,
	NULL,
	0x0100,
	NULL,
    NULL,
	NULL
}
mysql_declare_plugin_end;
