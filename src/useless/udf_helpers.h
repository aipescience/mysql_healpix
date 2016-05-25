/*****************************************************************
 * *******                  UDF_SPHERE                     *******
 *****************************************************************
 * some macros for making UDF life easier
 * 
 *****************************************************************
 */

#ifndef __MYSQL_SPHERE_UDFHELPERS__
#define __MYSQL_SPHERE_UDFHELPERS__

#include "types.h"
// #include "serialiser.h"

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

#define MYSQL_UDF_CHK_PARAM_REAL( PARAMNUM, ERRTEXT ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case REAL_RESULT: \
					break; \
				default: \
					strcpy(message, ERRTEXT ); \
					return 1;\
			} \
		}

#define MYSQL_UDF_DYNCHKCONV_PARAM_TOREAL( PARAMNUM ) \
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
					*is_null = 1; \
					return NULL;\
	    	} \
	    }

#define MYSQL_UDF_DYNCHK_PARAM_CHAR( PARAMNUM ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case STRING_RESULT: \
					break; \
				default: \
					*is_null = 1; \
					return NULL;\
			} \
		}

#define MYSQL_UDF_DYNCHK_PARAM_INT( PARAMNUM ) \
		if(args->args[ PARAMNUM ] != NULL) { \
			switch (args->arg_type[ PARAMNUM ]) { \
				case INT_RESULT: \
					break; \
				default: \
					*is_null = 1; \
					return NULL;\
			} \
		}
#define PROTECT(...) __VA_ARGS__
#define MYSQL_UDF_CHK_SPHERETYPE( PARANUM, BUFFEROBJ, VALIDTYPES, ERROR ) \
		if(args->args[ PARANUM ] != NULL) { \
			BUFFEROBJ->isDynParams[ PARANUM ] = 0; \
			BUFFEROBJ->argTypes[ PARANUM ] = decode(args->args[ PARANUM ], args->lengths[ PARANUM ], (void**)&BUFFEROBJ->memBufs[ PARANUM ]); \
 \
			MYSQL_SPHERE_TYPES types[] = VALIDTYPES; \
			int sphereTypesArrayLen = sizeof(types) / sizeof(MYSQL_SPHERE_TYPES); \
			int sphereTypesCounter = 0; \
			int sphereTypesFound = 0; \
			for(sphereTypesCounter = 0; sphereTypesCounter < sphereTypesArrayLen; sphereTypesCounter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types[sphereTypesCounter]) { \
		    		sphereTypesFound = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypesFound == 0) { \
				strcpy(message, ERROR); \
				delete BUFFEROBJ; \
				return 1; \
			} \
		} else { \
			BUFFEROBJ->isDynParams[ PARANUM ] = 1; \
		}\

#define MYSQL_UDF_DYNCHK_SPHERETYPE( PARANUM, BUFFEROBJ, VALIDTYPES ) \
    	if(BUFFEROBJ->isDynParams[PARANUM] == true) { \
    		if(BUFFEROBJ->memBufs[PARANUM] != NULL) { \
    			free(BUFFEROBJ->memBufs[PARANUM]); \
    			if(BUFFEROBJ->resBuf != NULL) { \
    				free(BUFFEROBJ->resBuf); \
    			} \
    		} \
 \
			BUFFEROBJ->argTypes[ PARANUM ] = decode(args->args[ PARANUM ], args->lengths[ PARANUM ], (void**)&BUFFEROBJ->memBufs[ PARANUM ]); \
 \
			MYSQL_SPHERE_TYPES types[] = VALIDTYPES; \
			int sphereTypesArrayLen = sizeof(types) / sizeof(MYSQL_SPHERE_TYPES); \
			int sphereTypesCounter = 0; \
			int sphereTypesFound = 0; \
			for(sphereTypesCounter = 0; sphereTypesCounter < sphereTypesArrayLen; sphereTypesCounter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types[sphereTypesCounter]) { \
		    		sphereTypesFound = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypesFound == 0) { \
				*is_null = 1; \
				return 0; \
			} \
    	} \

#define MYSQL_UDF_CHK_SPHERETYPE_COM( PARANUM, BUFFEROBJ, VALIDTYPES1, VALIDTYPES2, ERROR ) \
		if(args->args[ PARANUM ] != NULL) { \
			BUFFEROBJ->isDynParams[ PARANUM ] = 0; \
			BUFFEROBJ->argTypes[ PARANUM ] = decode(args->args[ PARANUM ], args->lengths[ PARANUM ], (void**)&BUFFEROBJ->memBufs[ PARANUM ]); \
 \
			MYSQL_SPHERE_TYPES types1[] = VALIDTYPES1; \
			int sphereTypes1ArrayLen = sizeof(types1) / sizeof(MYSQL_SPHERE_TYPES); \
			int sphereTypes1Counter = 0; \
			int sphereTypes1Found = 0; \
			MYSQL_SPHERE_TYPES types2[] = VALIDTYPES2; \
			int sphereTypes2ArrayLen = sizeof(types2) / sizeof(MYSQL_SPHERE_TYPES); \
			int sphereTypes2Counter = 0; \
			int sphereTypes2Found = 0; \
			for(sphereTypes1Counter = 0; sphereTypes1Counter < sphereTypes1ArrayLen; sphereTypes1Counter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types1[sphereTypes1Counter]) { \
		    		sphereTypes1Found = 1; \
		    		break; \
		    	} \
			} \
			for(sphereTypes2Counter = 0; sphereTypes2Counter < sphereTypes2ArrayLen; sphereTypes2Counter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types2[sphereTypes2Counter]) { \
		    		sphereTypes2Found = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypes1Found == 0 && sphereTypes2Found == 0) { \
				strcpy(message, ERROR); \
				delete BUFFEROBJ; \
				return 1; \
			} \
		} else { \
			BUFFEROBJ->isDynParams[ PARANUM ] = 1; \
		}\

#define MYSQL_UDF_DYNCHK_SPHERETYPE_COM( PARANUM, BUFFEROBJ, VALIDTYPES1, VALIDTYPES2 ) \
    	if(BUFFEROBJ->isDynParams[PARANUM] == true) { \
    		if(BUFFEROBJ->memBufs[PARANUM] != NULL) { \
    			free(BUFFEROBJ->memBufs[PARANUM]); \
    			BUFFEROBJ->memBufs[PARANUM] = NULL;\
    			if(BUFFEROBJ->resBuf != NULL) { \
    				free(BUFFEROBJ->resBuf); \
    			} \
    		} \
 \
			BUFFEROBJ->argTypes[ PARANUM ] = decode(args->args[ PARANUM ], args->lengths[ PARANUM ], (void**)&BUFFEROBJ->memBufs[ PARANUM ]); \
 \
			MYSQL_SPHERE_TYPES types1[] = VALIDTYPES1; \
			int sphereTypes1ArrayLen = sizeof(types1) / sizeof(MYSQL_SPHERE_TYPES); \
			int sphereTypes1Counter = 0; \
			int sphereTypes1Found = 0; \
			MYSQL_SPHERE_TYPES types2[] = VALIDTYPES2; \
			int sphereTypes2ArrayLen = sizeof(types2) / sizeof(MYSQL_SPHERE_TYPES); \
			int sphereTypes2Counter = 0; \
			int sphereTypes2Found = 0; \
			for(sphereTypes1Counter = 0; sphereTypes1Counter < sphereTypes1ArrayLen; sphereTypes1Counter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types1[sphereTypes1Counter]) { \
		    		sphereTypes1Found = 1; \
		    		break; \
		    	} \
			} \
			for(sphereTypes2Counter = 0; sphereTypes2Counter < sphereTypes2ArrayLen; sphereTypes2Counter++) { \
		    	if(BUFFEROBJ->argTypes[ PARANUM ] == types2[sphereTypes2Counter]) { \
		    		sphereTypes2Found = 1; \
		    		break; \
		    	} \
			} \
 \
			if(sphereTypes1Found == 0 && sphereTypes2Found == 0) { \
				*is_null = 1; \
				return 0; \
			} \
    	} \

#define MYSQL_UDF_DEINIT_BUFFER() \
	buffer * memBuf = (buffer*)initid->ptr; \
	if(memBuf != NULL) { \
		delete memBuf; \
	} \

#define MYSQL_UDF_SPHERE_TWOPARAM_INIT( FUNCNAME, VALIDTYPES1, VALIDTYPES2 ) \
	buffer * buf; \
 \
	if(args->arg_count == 2) { \
		MYSQL_UDF_CHK_PARAM_CHAR(0, FUNCNAME "() requires the first parameter to be a MySQL sphere object."); \
 \
		MYSQL_UDF_CHK_PARAM_CHAR(1, FUNCNAME "() requires the second parameter to be a MySQL sphere object."); \
 \
		buf = new buffer(2); \
 \
		MYSQL_UDF_CHK_SPHERETYPE( 0, buf, PROTECT(VALIDTYPES1),  \
											FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
 \
		MYSQL_UDF_CHK_SPHERETYPE( 1, buf, PROTECT(VALIDTYPES2),  \
											FUNCNAME "() error decoding second parameter. Corrupted or not the correct type." ); \
	} else { \
		strcpy(message, "wrong number of arguments: " FUNCNAME "() requires two parameters"); \
		return 1; \
	} \
    \
	initid->decimals = 31; \
	initid->maybe_null = 1; \
	initid->max_length = 1024; \
	initid->ptr = (char*)buf; \
 \
	return 0; \

#define MYSQL_UDF_SPHERE_TWOPARAM_COM_INIT( FUNCNAME, VALIDTYPES1, VALIDTYPES2 ) \
	buffer * buf; \
 \
	if(args->arg_count == 2) { \
		MYSQL_UDF_CHK_PARAM_CHAR(0, FUNCNAME "() requires the first parameter to be a MySQL sphere object."); \
 \
		MYSQL_UDF_CHK_PARAM_CHAR(1, FUNCNAME "() requires the second parameter to be a MySQL sphere object."); \
 \
		buf = new buffer(2); \
 \
		MYSQL_UDF_CHK_SPHERETYPE_COM( 0, buf, PROTECT(VALIDTYPES1), PROTECT(VALIDTYPES2), \
											FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
 \
		MYSQL_UDF_CHK_SPHERETYPE_COM( 1, buf, PROTECT(VALIDTYPES2), PROTECT(VALIDTYPES1), \
											FUNCNAME "() error decoding second parameter. Corrupted or not the correct type." ); \
	} else { \
		strcpy(message, "wrong number of arguments: " FUNCNAME "() requires two parameters"); \
		return 1; \
	} \
    \
	initid->decimals = 31; \
	initid->maybe_null = 1; \
	initid->max_length = 1024; \
	initid->ptr = (char*)buf; \
 \
	return 0; \

#define MYSQL_UDF_SPHERE_ONEPARAM_INIT( FUNCNAME, VALIDTYPES ) \
	buffer * buf; \
	MYSQL_SPHERE_TYPES argType; \
 \
	if(args->arg_count == 1) { \
		MYSQL_UDF_CHK_PARAM_CHAR(0, FUNCNAME "() requires the first parameter to be a MySQL sphere object."); \
 \
    	buf = new buffer(1); \
 \
        MYSQL_UDF_CHK_SPHERETYPE( 0, buf, PROTECT(VALIDTYPES),  \
                                            FUNCNAME "() error decoding first parameter. Corrupted or not the correct type." ); \
	} else { \
		strcpy(message, "wrong number of arguments: " FUNCNAME "() requires one parameter"); \
		return 1; \
	} \
    \
	initid->decimals = 31; \
	initid->maybe_null = 1; \
	initid->max_length = 1024; \
	initid->ptr = (char*)buf; \
 \
	return 0; \


class buffer {
public:
	int len;
	void ** memBufs;
	bool * isDynParams;
	char * resBuf;

	buffer(int nlen) {
		len = nlen;
		memBufs = (void**)malloc(len * sizeof(void*));
		memset(memBufs, 0, len * sizeof(void*));
		isDynParams = (bool*)malloc(len * sizeof(bool));
		resBuf = NULL;
	}

	~buffer() {
		int i;
		if(memBufs != NULL) {
			for(i = 0; i < len; i++) {
				if(memBufs[i] != NULL) {
					free(memBufs[i]);
				}
			}

			free(memBufs);
		}

		if(resBuf != NULL) {
			free(resBuf);
		}

		if(isDynParams != NULL) {
			free(isDynParams);
		}
	}
};


#endif
