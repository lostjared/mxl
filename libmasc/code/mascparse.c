#include "mascparse.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "masc++.h"
#include "mascsym.h"



static unsigned int parseTrans(List *lst)
{

	switch(lst->token->type)
	{
	case MID_SYMBOL:

		if((lst->token->num = parseType(lst)) != -1)
		{

		}	 
		else 
		{

			fprintf(stderr, "Error operator %s not recgonized \n", lst->token->str);

		}

		lst->token->value = 0;
		return 0;

	break;
	case MID_LETTER:
	{

		int key = parseKey(lst);

		if(key != -1)
		{
			lst->token->num = key;

			if(key == KEY_DEFINE)
			{
				List *identifier = lst->next;

				if(identifier->token->type == MID_LETTER && parseKey(identifier) == -1 && masc_built_in(identifier->token->str) == -1)
				{
					/* add to hash starting point */
					Bucket *b;

					
					/*func_var[hash_key].spot = identifier;*/
					b = maschash_get(identifier->token->str);
					b->pos = identifier;
					b->name = identifier->token->str;
					identifier->token->num = KEY_IDENTIFIER;
						

				}
			}

			
			lst->token->value = 0;
			

		}
		else
		{
			lst->token->num = KEY_IDENTIFIER;			
			

		}	
		

	}
	break;
	case MID_DIGIT:
	
		lst->token->value = atoi(lst->token->str);

		return 0;

	break;
	case MID_STRING:

	

		break;


	}

	return 0;


}

static void parse(List *lst)
{

	unsigned int index;
	index = parseTrans(lst);



}


int parseType(List *lst)
{
	unsigned int i;

	for(i = 0; symbol_array[i] != 0; i++)
	{
		if(strcmp(symbol_array[i], lst->token->str) == 0)
		return i;
	}

	return -1;

}

int parseKey(List *lst)
{
	unsigned int i;

	for(i = 0; keyword_array[i] != 0; i++)
		if(strcmp(keyword_array[i], lst->token->str) == 0)
			return i;

	return -1;
}

void parseAnaylize(List *l)
{
	List **index = &l, *item = l;

	while((item = *index) != 0)
	{

		parse(item);
		index = &item->next;
	}

}


void addChild(parseTree *tree, parseTree *item)
{
	tree->child[tree->c_num++] = item;
	
}

int ListMatch(unsigned int type, List *lst)
{
		
		if(lst != 0 && type == lst->token->type)
			return 1;
		return 0;
}

void error_exit(int x)
{

	if(x <= 0)
	{
		fprintf(stderr, "Error Exiting");
		exit(-1);
	}

}

List *expression(parseTree *tree, List *lst);
List *expression_prime(parseTree *tree, List *lst);

List *expression_factor(parseTree *tree, List *lst)
{
	if(lst->token->num == SYM_POPEN)
	{
		lst = lst->next;
		if(lst)
		{
			lst = expression(tree, lst);
			if(lst->token->num == SYM_PCLOSE) 
				return lst->next;

		}

	}



	return lst;
}

List *listnext(List *l)
{
	
	if(l->next)
	return l->next;


	printf("Syntax Error\n");
	exit(-2);
	return 0;


}

List *expression_tprime(parseTree *tree, List *lst)
{

	return lst;

}

char *call_str = 0;

int masc_call(char *func_name)
{
	int pos = masc_built_in(func_name);
	if(pos != -1)
	{
		if(pos == B_WRITE) {
		

			return funcs[pos].func((void*)call_str);
		} else
		return funcs[pos].func(0);
	}


	return 0;
}





int masc_built_in(char *func_name)
{
	int i=0;

	while ( funcs[i].name != 0 )
	{
		if(strcmp(func_name, funcs[i].name) == 0) return i;
		i++;
	}

	return -1;

}

List *expression_function(char *func_name, parseTree *tree, List *lst)
{
	List *next = listnext(lst);
	

	if(next->token->num == SYM_PCLOSE)
	{
		int return_value = 0;
		lst = next;
		lst = listnext(lst);
		if(masc_built_in(func_name) != -1) {
			parseStack p;
			return_value = masc_call(func_name);
			p.name = func_name;
			p.value = "";
			p.val = return_value;
			masc_push( p );

		}
		else	
		{
			Bucket *b;

			b = maschash_get(func_name);

			function_call(tree, func_name);



		}
		return lst;
	}
	else if(next->token->type == MID_STRING)
	{
		char *str = next->token->str;
		int return_value = 0;

		if(masc_built_in(func_name) != -1 && strcmp(func_name, "write") == 0)
		{
			call_str = str;
			return_value = masc_call(func_name);
			next = next->next;

			if(next->token->num == SYM_PCLOSE)
				next = next->next;
			else if(next->token->num == SYM_COMMA)
			{	

				while ( next->token->num == SYM_COMMA )
				{
					next = listnext(next);
					{
						if(next->token->num == KEY_IDENTIFIER)
						{
							Bucket *b;
							b = maschash_get(next->token->str);
							printf("%d", b->ival);
						}
						else if(next->token->type == MID_STRING)
						{
							printf("%s", next->token->str);

						}
						else if(next->token->type == MID_DIGIT)
						{
							printf("%s", next->token->str);
						}


					}
					next = listnext(next);


				}



				
				if(next->token->num == SYM_PCLOSE)

					next = listnext(next);
				

			}

			return next;
		
		}
		else
		{
			parseStack p;
			p.name = "string";
			p.value = next->token->str;
			masc_call_push(p);
		}		
		

	}


	return lst;
}

List *expression_fact(parseTree *tree, List *lst)
{

	if(lst->token->num == KEY_IDENTIFIER)
	{
		Bucket *b = maschash_get(lst->token->str);
		char *func_name = lst->token->str;

		

		lst = listnext(lst);
		
		if(lst && lst->token->num == SYM_POPEN)
		{
			
			lst = expression_function(func_name,tree, lst);
			if(lst->token->num == SYM_SEMI)
				return lst;
	
			
		} 
		else {

			parseStack p;

			p.name = b->name;
			p.val = b->ival;
			p.value = "";
			masc_push ( p );
		
		
		}

	} else if(lst->token->type == MID_DIGIT)
	{
		parseStack p;
		p.name = p.value = "";
		p.val = atoi(lst->token->str);
		masc_push(p);
		lst = listnext(lst);

	} else if(lst->token->num == SYM_POPEN)
	{

		lst = expression_factor(tree, lst);
		
	}

		
	return lst;
}

List *expression_term(parseTree *tree, List  *lst)
{

	char uop = 0;

	lst = expression_fact(tree, lst);
	
	while ( (lst->token->num == SYM_MUL) || (lst->token->num == SYM_DIV) )
	{
		uop = (lst->token->num == SYM_MUL) ? '*' : '/';
		lst = listnext(lst);
		lst = expression_fact(tree, lst);
		{
			parseStack *p1, *p2, cur;
			p2 = masc_pop();
			p1 = masc_pop();

			if(p1  && p2) {

				cur.name = cur.value = "";
				cur.val = ( uop == '*' ) ? p1->val*p2->val : p1->val/p2->val;
				masc_push( cur );
			}
		}
	}
	
	return lst;
}


List *expression(parseTree *tree, List *lst)
{
	char uop = 0;

	if((lst->token->num == SYM_ADD || lst->token->num == SYM_MINUS))
	{
		uop = ( lst->token->num == SYM_ADD ) ? '+' : '-';
		lst = listnext(lst);
	}

	lst = expression_term(tree, lst);
	
	while  ((lst->token->num == SYM_MINUS) || (lst->token->num == SYM_ADD))
	{
		uop = ( lst->token->num == SYM_ADD ) ? '+' : '-';
	
		lst = listnext(lst);

		lst = expression_term(tree, lst);

		{
			parseStack *p1, *p2, cur;
			cur.name = cur.value = "";
			p2 = masc_pop();
			p1 = masc_pop();
			if(p1 && p2) {
				cur.val = ( uop != 0 &&  uop == '+' ) ? p1->val+p2->val : p1->val-p2->val;
				masc_push( cur );
			}

		}


	}

	return lst;

}

List *exp_statement( int *return_value, parseTree *tree, List *lst )
{
	parseStack *s;
	masc_clr();
	lst = expression(tree, lst);
	s = masc_pop();

	if(s)
	*return_value = s->val; else *return_value = 0;
	return lst;
}


List *arg_statement( parseTree *tree, List *lst)
{

	List *list_point = lst;
	List **ptr = &lst;


	while( (list_point = *ptr) != 0)
	{

		switch(list_point->token->type)
		{
		case MID_LETTER: {

			parseStack p;
	
			p.name = list_point->token->str;
			p.value = "";
			{
				Bucket *b;
				b = maschash_get(p.name);
				b->ival = 0;
				

			}
		}
		break;
		case MID_SYMBOL:

		

			if(list_point->token->num == SYM_COMMA || list_point->token->num == SYM_POPEN)
			{
				ptr = &list_point->next;
				continue;

			}
			if(list_point->token->num == SYM_PCLOSE)
			{
				list_point = list_point->next;
				return list_point;
			}

		break;
		}


		ptr =  &list_point->next;
	}

	return 0;

}

List *statements(parseTree *tree, List *pos)
{
	List *temp = 0, **ptr_temp = &pos;

	while( (temp =  *ptr_temp) != 0 )
	{

		if(temp->token->num == SYM_BCLOSE)
		{


			return temp->next;

		}

	
		if(temp->token->num == KEY_IDENTIFIER)
		{
			List *t = 0;
			Bucket *b = maschash_get(temp->token->str);

			t = parseGetOffset(temp, 1);
			b->name = temp->token->str;

			

			
			if(t->token->type == MID_SYMBOL && t->token->num == SYM_EQUAL)
			{

				int value=0;

				t = parseGetOffset(temp, 2);
				/* assign */
				t = exp_statement(&value, tree, t);

				ptr_temp = &t;
				b->ival = value;
				
				continue;


			}
			else if ( t->token->type == MID_SYMBOL && t->token->num == SYM_POPEN)
			{
				/* function call */
				int val = 0;
				t= exp_statement(&val, tree, temp);
				ptr_temp = &t;
				continue;

			

			}

		}

		if(temp->next != 0)
		ptr_temp = &temp->next;
		else return *ptr_temp;

	}
	return *ptr_temp;
}
	
int function_call(parseTree *root, const char *name)
{
	
	Bucket *b;

	b = maschash_get((char*)name);


	if(b->name != 0)
	{

		List *lz;
		lz = arg_statement(root, b->pos);
		lz = statements(root, lz);

		return 1;


	}
	

	

	return 0;
}


 int initProgram(List *lst)
 {

	parseTree *t = 0;
	

	
	if(function_call(t, "main"))
	{
		return 1;

	} else fprintf(stderr, "script error:\tmain function not found\n");



	return 0;
}

 List *parseGetOffset(List *lst, int it)
 {

	int i;
	for(i = 0; i < it && lst != 0; i++)
		if(lst->next != 0)
		lst = lst->next;

	return lst;
}

int parsePrintTree(parseTree *t)
{

	if(t && t->token != 0)
		printf("Token: %s\n", t->token->str);	

	
	if(t->right != 0)	
	parsePrintTree(t->right);

	if(t->left != 0)
	parsePrintTree(t->left);

	


	return 0;
}

int built_read(void *ptr)
{
	return getchar();
}
int built_write(void *ptr)
{
	printf("%s", (char*)ptr);
	return 0;
}
int built_close(void *ptr)
{
	exit(0);
	return 0;
}

int built_init(void *ptr)
{


	return 0;
}


