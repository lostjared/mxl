#include "maschash.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

Bucket buckets[MAX_BUX];


unsigned int maschash_key(const char *src)
{
	unsigned int i,key = 0;

	for(i=0; src[i] != 0; i++)
	{
		key += src[i];

	}

	return key % MAX_BUX;
}

Bucket *maschash_get(char *name)
{
	unsigned int key = 0;
	Bucket *bucketz = 0, **b = 0;



	key = maschash_key(name);

	bucketz = &buckets[key];
	if(buckets[key].name == 0)
	{
		buckets[key].name = name;
		return &buckets[key];
	}

	if(strcmp(buckets[key].name, name) == 0)
	{
		return &buckets[key];
	}

	*b = bucketz;

	while((bucketz = *b) != 0)
	{
		if(strcmp(bucketz->name, name) == 0)
			return bucketz;

		b = &bucketz->next;

	}

	bucketz = (Bucket*) calloc(1,  sizeof(Bucket) );
	bucketz->name = name;
	*b = bucketz;

	return bucketz;

}

void maschash_init(void)
{
	memset(buckets, 0, sizeof(buckets));

}

