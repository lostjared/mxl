#ifndef ___MASC_HASH_BUCKET__H_
#define ___MASC_HASH_BUCKET__H_


#include "masclist.h"
#define MAX_BUX 1024


#ifdef __cplusplus
extern "C" {
#endif




typedef struct _Bucket
{
	List *pos;
	char *name;
	char *strval;
	int  ival;	
	struct _Bucket *next;
} Bucket;


extern Bucket buckets[MAX_BUX];


unsigned int maschash_key(const char *name);
void maschash_init(void);
Bucket *maschash_get(char *name);


#ifdef __cplusplus
}
#endif








#endif

