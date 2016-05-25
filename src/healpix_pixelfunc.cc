#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mysql.h>
#include "chealpix.h"

#define MYSQL_UDF_CHAR_FUNC( NAME ) \
 	my_bool NAME##_init( UDF_INIT* initid, UDF_ARGS* args, char* message ); \
 	void NAME##_deinit( UDF_INIT* initid ); \
 	char *NAME( UDF_INIT* initid, UDF_ARGS* args, char *result, unsigned long *length, char* is_null, char* error );

#define MYSQL_UDF_REAL_FUNC( NAME ) \
 	my_bool NAME##_init( UDF_INIT* initid, UDF_ARGS* args, char* message ); \
 	void NAME##_deinit( UDF_INIT* initid ); \
 	double NAME( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error );

#define MYSQL_UDF_INT_FUNC( NAME ) \
 	my_bool NAME##_init( UDF_INIT* initid, UDF_ARGS* args, char* message ); \
 	void NAME##_deinit( UDF_INIT* initid ); \
 	long long NAME( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error );


extern "C" {
    MYSQL_UDF_REAL_FUNC( pix2ang );
    MYSQL_UDF_REAL_FUNC( pix2vec );
    MYSQL_UDF_INT_FUNC( ang2pix );
    MYSQL_UDF_INT_FUNC( vec2pix );
    MYSQL_UDF_INT_FUNC( neighbours );
    MYSQL_UDF_INT_FUNC( nside2npix );
    MYSQL_UDF_INT_FUNC( npix2nside );
}

#define MYSQL_UDF_CHK_PARAM_CHAR( PARAMNUM, ERRTEXT ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case STRING_RESULT: \
					break; \
				default: \
					strcpy(message, ERRTEXT ); \
					return 1;\
			} \
		}

#define MYSQL_UDF_CHK_PARAM_INT( PARAMNUM, ERRTEXT ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case INT_RESULT: \
					break; \
				default: \
					strcpy(message, ERRTEXT ); \
					return 1;\
			} \
		}

#define MYSQL_UDF_CHK_PARAM_DECIMAL( PARAMNUM, ERRTEXT ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case DECIMAL_RESULT: \
					break; \
				default: \
					strcpy(message, ERRTEXT ); \
					return 1;\
			} \
		}

#define MYSQL_UDF_CHK_PARAM_REAL_OR_DECIMAL( PARAMNUM, ERRTEXT ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case REAL_RESULT: \
					break; \
                case DECIMAL_RESULT: \
                    break; \
				default: \
					strcpy(message, ERRTEXT ); \
					return 1;\
			} \
		}

#define MYSQL_UDF_CHKCONV_PARAM_TOREAL( PARAMNUM, ERRTEXT ) \
		if(args->args[ PARAMNUM ] != NULL) { \
	    	switch (args->arg_type[ PARAMNUM ]) { \
	    		case INT_RESULT: \
	    			args->arg_type[ PARAMNUM ] = REAL_RESULT; \
	    			*(double*)args->args[ PARAMNUM ] = (double)*(long long*)args->args[ PARAMNUM ]; \
	    			break; \
	    		case DECIMAL_RESULT: \
	    			args->arg_type[ PARAMNUM ] = REAL_RESULT; \
	    			*(double*)args->args[ PARAMNUM ] = atof(args->args[ PARAMNUM ]); \
	    			break; \
	    		case REAL_RESULT: \
	    			break; \
	    		default: \
					strcpy(message, ERRTEXT ); \
					return 1; \
	    	} \
	    }

static const double pi=3.141592653589793238462643383279502884197;

///////////////////////////////////////////////////////////////////////////////

my_bool ang2pix_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {

    char pixtype_ring[] = "ring";
    char pixtype_nest[] = "nest";

    if (args->arg_count == 3 or args->arg_count == 4) {
        MYSQL_UDF_CHK_PARAM_INT(0, "First parameter must be an int.");
        MYSQL_UDF_CHKCONV_PARAM_TOREAL(1, "Second parameter must be a float.");
        MYSQL_UDF_CHKCONV_PARAM_TOREAL(2, "Third parameter must be a float.");
        
        if (args->arg_count == 4) {
            MYSQL_UDF_CHK_PARAM_CHAR(3, "Fourth parameter must either be 'nest' or 'ring'.");

            if (strcmp(args->args[3], pixtype_ring) != 0 && strcmp(args->args[3], pixtype_nest) != 0) {
                strcpy(message, "Pixel ordering can only be 'ring' or 'nest'.");
                return 1;
            }
        }

        args->arg_type[1] = REAL_RESULT;
        args->arg_type[2] = REAL_RESULT;
    }
    else {
		strcpy(message, "Wrong number of arguments: ang2pix() requires three or four parameters.");
		return 1;
	}

	initid->decimals = 31;
	initid->maybe_null = 1;
	initid->max_length = 1024;
	return 0;
}

long long ang2pix( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {

    long long nside = *((long long*) args->args[0]);
    double theta = *(double*) args->args[1];
    double phi = *(double*) args->args[2];

    if (theta < 0.0 || theta - pi > 0.0) {
        return -1;
    }

    if (args->arg_count == 4) {
        if (strcmp(args->args[3], "ring") == 0) {
            return (long long) ang2pix_ring (nside, theta, phi);
        }
        if (strcmp(args->args[3], "nest") == 0) {
            return (long long) ang2pix_nest (nside, theta, phi);
        }
    }
    else {
        return (long long) ang2pix_ring (nside, theta, phi);
    }
}

my_bool pix2ang_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {

    char pixtype_ring[] = "ring";
    char pixtype_nest[] = "nest";

    if (args->arg_count == 3 or args->arg_count == 4) {
        MYSQL_UDF_CHK_PARAM_INT(0, "First parameter must be an int.");
        MYSQL_UDF_CHK_PARAM_INT(1, "Second parameter must be an int.");
        MYSQL_UDF_CHK_PARAM_INT(2, "Third parameter must be an int.");

        long long angidx = *((long long*) args->args[2]);

        if (angidx != 0 && angidx != 1) {
            strcpy(message, "Third parameter must either be 0 (theta) or 1 (phi).");
            return 1;
        }

        if (args->arg_count == 4) {
            MYSQL_UDF_CHK_PARAM_CHAR(3, "Fourth parameter must either be 'nest' or 'ring'.");

            if (strcmp(args->args[3], pixtype_ring) != 0 && strcmp(args->args[3], pixtype_nest) != 0) {
                strcpy(message, "Pixel ordering can only be 'ring' or 'nest'.");
                return 1;
            }
        }
    }
    else {
		strcpy(message, "Wrong number of arguments: pix2ang() requires three or four parameters.");
		return 1;
	}

	initid->decimals = 31;
	initid->maybe_null = 1;
	initid->max_length = 1024;
	return 0;
}

double pix2ang( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {

    long long nside = *((long long*) args->args[0]);
    long long ipix = *((long long*) args->args[1]);
    long long angidx = *((long long*) args->args[2]);

    double phi;
    double theta;

    if (args->arg_count == 4) {
        if (strcmp(args->args[3], "ring") == 0) {
            pix2ang_ring(nside, ipix, &theta, &phi);
        }
        if (strcmp(args->args[3], "nest") == 0) {
            pix2ang_nest(nside, ipix, &theta, &phi);
        }
    }
    else {
        pix2ang_ring(nside, ipix, &theta, &phi);
    }
    if (angidx == 0) {
        return theta;
    }
    else {
        return phi;
    }
}

///////////////////////////////////////////////////////////////////////////////

my_bool vec2pix_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {

    char pixtype_ring[] = "ring";
    char pixtype_nest[] = "nest";

    if (args->arg_count == 4 or args->arg_count == 5) {
        MYSQL_UDF_CHK_PARAM_INT(0, "First parameter must be an int.");
        MYSQL_UDF_CHKCONV_PARAM_TOREAL(1, "Second parameter must be a float.");
        MYSQL_UDF_CHKCONV_PARAM_TOREAL(2, "Third parameter must be a float.");
        MYSQL_UDF_CHKCONV_PARAM_TOREAL(3, "Fouth parameter must be a float.");

        if (args->arg_count == 5) {
            MYSQL_UDF_CHK_PARAM_CHAR(4, "Fifth parameter must either be 'nest' or 'ring'.");

            if (strcmp(args->args[4], pixtype_ring) != 0 && strcmp(args->args[4], pixtype_nest) != 0) {
                strcpy(message, "Pixel ordering can only be 'ring' or 'nest'.");
                return 1;
            }
        }

        args->arg_type[1] = REAL_RESULT;
        args->arg_type[2] = REAL_RESULT;
        args->arg_type[3] = REAL_RESULT;
    }
    else {
		strcpy(message, "Wrong number of arguments: ang2pix() requires three or four parameters.");
		return 1;
	}

	initid->decimals = 31;
	initid->maybe_null = 1;
	initid->max_length = 1024;
	return 0;
}

long long vec2pix( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {

    long long nside = *((long long*) args->args[0]);
    double vec[3] = {*(double*) args->args[1], *(double*) args->args[2], *(double*) args->args[3]};

    if (args->arg_count == 5) {
        if (strcmp(args->args[4], "ring") == 0) {
            return (long long) vec2pix_ring(nside, vec);
        }
        if (strcmp(args->args[4], "nest") == 0) {
            return (long long) vec2pix_nest(nside, vec);
        }
    }
    else {
        return (long long) vec2pix_ring(nside, vec);
    }
}

my_bool pix2vec_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {

    char pixtype_ring[] = "ring";
    char pixtype_nest[] = "nest";

    if (args->arg_count == 3 or args->arg_count == 4) {
        MYSQL_UDF_CHK_PARAM_INT(0, "First parameter must be an int.");
        MYSQL_UDF_CHK_PARAM_INT(1, "Second parameter must be an int.");
        MYSQL_UDF_CHK_PARAM_INT(2, "Third parameter must be an int.");

        long long angidx = *((long long*) args->args[2]);

        if (angidx != 0 && angidx != 1 && angidx != 2) {
            strcpy(message, "Third parameter must either be 0 (x) or 1 (y) or 2 (z).");
            return 1;
        }

        if (args->arg_count == 4) {
            MYSQL_UDF_CHK_PARAM_CHAR(3, "Fourth parameter must either be 'nest' or 'ring'.");

            if (strcmp(args->args[3], pixtype_ring) != 0 && strcmp(args->args[3], pixtype_nest) != 0) {
                strcpy(message, "Pixel ordering can only be 'ring' or 'nest'.");
                return 1;
            }
        }
    }
    else {
		strcpy(message, "Wrong number of arguments: pix2vec() requires three or four parameters.");
		return 1;
	}

	initid->decimals = 31;
	initid->maybe_null = 1;
	initid->max_length = 1024;
	return 0;
}

double pix2vec( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {

    long long nside = *((long long*) args->args[0]);
    long long ipix = *((long long*) args->args[1]);
    long long angidx = *((long long*) args->args[2]);

    double vec[3];

    if (args->arg_count == 4) {
        if (strcmp(args->args[3], "ring") == 0) {
            pix2vec_ring(nside, ipix, vec);
        }
        if (strcmp(args->args[3], "nest") == 0) {
            pix2vec_nest(nside, ipix, vec);
        }
    }
    else {
        pix2vec_ring(nside, ipix, vec);
    }
    return vec[angidx];
}

///////////////////////////////////////////////////////////////////////////////

my_bool neighbours_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {

    char pixtype_ring[] = "ring";
    char pixtype_nest[] = "nest";

    if (args->arg_count == 3 or args->arg_count == 4) {
        MYSQL_UDF_CHK_PARAM_INT(0, "First parameter must be an int.");
        MYSQL_UDF_CHK_PARAM_INT(1, "Second parameter must be an int.");
        MYSQL_UDF_CHK_PARAM_INT(2, "Third parameter must be an int.");

        long long nidx = *((long long*) args->args[2]);

        if (nidx < 0 || nidx > 7) {
            strcpy(message, "Third parameter must be between 0 and 7.");
            return 1;
        }

        if (args->arg_count == 4) {
            MYSQL_UDF_CHK_PARAM_CHAR(3, "Fourth parameter must either be 'nest' or 'ring'.");

            if (strcmp(args->args[3], pixtype_ring) != 0 && strcmp(args->args[3], pixtype_nest) != 0) {
                strcpy(message, "Pixel ordering can only be 'ring' or 'nest'.");
                return 1;
            }
        }
    }
    else {
		strcpy(message, "Wrong number of arguments: neighbours() requires three or four parameters.");
		return 1;
	}

	initid->decimals = 31;
	initid->maybe_null = 1;
	initid->max_length = 1024;
	return 0;
}

long long neighbours( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {

    long long nside = *((long long*) args->args[0]);
    long long ipix = *((long long*) args->args[1]);
    long long nidx = *((long long*) args->args[2]);

    long result[8];

    if (args->arg_count == 4) {
        if (strcmp(args->args[3], "ring") == 0) {
            neighbours_ring(nside, ipix, result);
        }
        if (strcmp(args->args[3], "nest") == 0) {
            neighbours_nest(nside, ipix, result);
        }
    }
    else {
        neighbours_ring(nside, ipix, result);
    }
    return (long long) result[nidx];
}

///////////////////////////////////////////////////////////////////////////////

my_bool nside2npix_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {
    if (args->arg_count == 1) {
        MYSQL_UDF_CHK_PARAM_INT(0, "nside2npix() requires the first parameter to be an int.");
    }
    else {
		strcpy(message, "Wrong number of arguments: nside2npix() requires one parameter.");
		return 1;
	}

    return 0;
}

long long nside2npix( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {
    return (long long) nside2npix_(*((long long*) args->args[0]));
}

my_bool npix2nside_init( UDF_INIT* initid, UDF_ARGS* args, char* message ) {
    if (args->arg_count == 1) {
        MYSQL_UDF_CHK_PARAM_INT(0, "npix2nside() requires the first parameter to be an int.");
    }
    else {
		strcpy(message, "Wrong number of arguments: npix2nside() requires one parameter.");
		return 1;
	}

    return 0;
}

long long npix2nside( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error ) {
    return (long long) npix2nside_(*((long long*) args->args[0]));
}
