
#include "masclist.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

void Token_print(Token *token)
{
	if(token != 0)
	{

		Token_print(token->left);
		Token_print(token->right);
		printf("Token ID: %s \n", token->str);

	}

}

void Token_free(Token *token)
{

	if(token != 0)
	{
		Token_free(token->left);
		Token_free(token->right);

		if(token->str != 0)
		free(token->str);
		free(token);
	}


}

Token *Token_add(Token *root, char *str, unsigned int length, unsigned int type)
{
	Token **counter = 0;
	Token *item = 0;


	if(root == 0)
	{

		root = (Token*)malloc ( sizeof(Token) );
		root->str = strdup(str);
		root->num = length;
		root->type = type;
		root->left = root->right = 0;
		return root;
	}

	counter = &root;


	while( (item = *counter) != 0 )
	{

		int cmp = 0;
		cmp = strcmp(item->str, str);
		if(cmp == 0) return item;
		counter = (cmp > 0) ? &item->left : &item->right;
	}
	

	item  = (Token*) malloc ( sizeof(Token) );
	item->left = item->right = 0;;
	item->str = strdup(str);
	item->num = length;
	item->type = type;
	

	*counter = item;

	return item;


}

List *List_add(Token *token, List *list)
{

	List  *p = 0, **ptr;


	if(list == 0)
	{

		list = (List*) malloc ( sizeof(List) );
		list->next = 0;
		list->token = token;
		return list;

	}


	ptr = &list;


	while((p = *ptr) != 0)
	{
		ptr = &p->next;
	}
	
	p = (List*) malloc ( sizeof(List) );
	p->next = 0;
	p->token = token;
	*ptr = p;
	return p;
		
}

void List_print(List *lst)
{

	List *p;

	for(p=lst; p!=0; p=p->next)
	{
		printf("Listed: %s\n", p->token->str);
	}



}

void List_free(List *l)
{
	if(l != 0)
	{

		if(l->next != 0)
		List_free(l->next);

		free(l);

	}

}


List *List_find(List *lst, const char *src)
{
/*	List *l, **s  = &lst;

	while((l = *s) != 0)
	{
		if(strcmp(lst->token->str, src) == 0) 
		return l;

	}
*/

	return 0;
}


Token *Token_find(Token *token, const char *key)
{
	Token *tok, **pool;
	pool = &token;

	while( (tok = *pool) != 0)
	{

		int cmp;
		cmp = strcmp(tok->str, key);
		if(cmp == 0) return tok;
		pool = (cmp > 0) ? &tok->left : &tok->right;

	}


	return 0;
}
