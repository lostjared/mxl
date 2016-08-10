#ifndef __MASCLIST__H__
#define __MASCLIST__H__


#ifdef __cplusplus
extern "C" {
#endif



typedef int (*callback)(int value);

typedef struct _Token {

	char *str;
	int num;
	int value;
	unsigned int type;
	struct _Token *left, *right;
	callback cb;

} Token;

typedef struct _List {

	Token *token;
	struct _List *next;
} List;

extern Token *Token_add(Token *prev, char *str, unsigned int length, unsigned int type);
extern void Token_print(Token *token);
extern void Token_free(Token *token);
extern  Token *Token_find(Token *token, const char *key);

extern List *List_add(Token *token, List *list);
extern void List_free(List *lst);
extern void List_print(List *lst);
extern List *List_find(List *lst, const char *src);

extern List *list_root;

#ifdef __cplusplus
}
#endif



#endif


