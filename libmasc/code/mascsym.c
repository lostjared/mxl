#include "mascsym.h"
#include<string.h>
#include<stdio.h>

parseStack exp_stack[MAX_BUCKET];
static int exp_index = 0;
static int fst_index = 0;

parseStack fstack[MAX_BUCKET];

int masc_call_push(parseStack p);
parseStack *masc_call_pop();


unsigned int masc_hash_key(const char *src)
{
	unsigned int key = 0, i;

	for(i = 0; src[i] != 0; i++)
	{
		key += src[i];
		key <<= 4;
	}


	return key%MAX_BUCKET;

}


void masc_sym_init_buckets()
{
	memset(exp_stack, 0, sizeof(exp_stack));
	memset(fstack, 0, sizeof(fstack));

}

void masc_init_stack()
{

	masc_clr();

}

int masc_call_push( parseStack p )
{
	fstack[fst_index++] = p;
	return fst_index;

}

int  masc_push( parseStack c)
{
	exp_stack[exp_index++] = c;
	return exp_index;
}

parseStack *masc_call_pop()
{
	if(fst_index > 0)
	return &fstack[--fst_index];
	return 0;


}

parseStack *masc_pop()
{

	if(exp_index > 0) {
		

		return &exp_stack[--exp_index];
	} 

	return 0;
}

int masc_clr()
{
	memset(exp_stack, 0, sizeof(exp_stack));
	memset(fstack, 0, sizeof(fstack));

	exp_index = 0;
	fst_index = 0;

	return 0;
}

parseStack *masc_get(unsigned int index)
{

	return &exp_stack[index];
}

void masc_print_stack()
{
	 int i;
	for(i = 0; i < exp_index; i++)
	{

		printf("stack value %s == %d\n", exp_stack[i].name, exp_stack[i].val);
	}

}

