#ifndef __MX_SYM__H_
#define __MX_SYM__H_

#include "masclist.h"
#define MAX_BUCKET 1024

#ifdef __cplusplus
extern "C" {
#endif



typedef struct _parseStack {
	char *name, *value;
	int val;
} parseStack;

typedef struct _functionStack {
	char *fname;
	List *spot;
} functionStack;


extern parseStack exp_stack[MAX_BUCKET];
extern parseStack fstack[MAX_BUCKET];

int masc_call_push(parseStack p);
parseStack *masc_call_pop();

void masc_init_stack();
int  masc_push( parseStack c);
parseStack *masc_pop();
int masc_clr();
parseStack *masc_get(unsigned int index);
void masc_print_stack();
unsigned int masc_hash_key(const char *key);
void masc_sym_init_buckets();

#ifdef __cplusplus
}
#endif




#endif
