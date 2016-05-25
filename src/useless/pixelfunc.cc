#include "pixelfunc.h"
#include <mysql/plugin.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>


/*!
	If latitude of south-west corner is larger than north-east
	corner, swap both positions.
	\brief Checks the box (input)
	\param box pointer to spherical box
*/
static void sbox_check ( SBox * box ) {
	if ( FPgt( box->sw.lat, box->ne.lat ) ) {
		// swap
		SPoint sp;
		memcpy ( (void*)&sp, (void*)&box->sw, sizeof(SPoint) );
		memcpy ( (void*)&box->sw, (void*)&box->ne, sizeof(SPoint) );
		memcpy ( (void*)&box->ne, (void*)&sp, sizeof(SPoint) );
	}
	if ( FPeq( box->sw.lng, box->ne.lng ) && FPne( box->sw.lat, box->ne.lat ) ) {
		box->sw.lng = 0.0;
		box->ne.lng = PID;
	}
}
