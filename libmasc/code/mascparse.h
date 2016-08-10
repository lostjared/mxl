#ifndef __MASC__PARSE___H_
#define __MASC__PARSE___H_
#include "masclist.h"
#include "maschash.h"


#ifdef __cplusplus
extern "C" {
#endif





 enum { SYM_BOPEN = 0, SYM_BCLOSE, SYM_POPEN, SYM_PCLOSE, SYM_BROPEN, SYM_BRCLOSE,
SYM_SEMI, SYM_PERIOD, SYM_COMMA, SYM_MUL, SYM_DIV, SYM_ADD, SYM_MINUS, SYM_INC, SYM_DEC,SYM_ADEQ, SYM_MINEQ, SYM_MULEQ,
SYM_DIVEQ, SYM_AND, SYM_OR, SYM_NOT, SYM_ANDEQ, SYM_OREQ, SYM_XOR, SYM_XOREQ, SYM_COM_OPEN, SYM_COM_CLOSE,
SYM_LAND, SYM_LOR, SYM_EQUAL, SYM_EQUALEQ, SYM_NOTEEQ, SYM_COLON, SYM_PTR};

static char *symbol_array[] = { "{", "}", "(", ")","[", "]",";", "." , "," , "*", "/", "+", "-", "++", "--",
"+=", "-=", "*=", "/=", "&", "|", "!", "&=", "|=","^", "^=","/*", "*/", "&&", "||", "=", "==", "!=",":","->",
0
};

static char *keyword_array[] = {"def", "if", "else", "elif", "while", "do", "for", "foreach", "import", "switch", 0 };
enum { KEY_DEFINE = 0, KEY_IF, KEY_ELSE, KEY_WHILE, KEY_DO, KEY_FOR, KEY_FORE, KEY_IMPORT, KEY_SWITCH, KEY_IDENTIFIER };

#define MAX_C 3

typedef struct _built_in {

	char *name;
	int (*func)(void *);

} built_in;


extern int masc_built_in(char *func_name);
extern int built_read(void *);
extern int built_write(void *);
extern int built_close(void *);
extern int built_init(void *);


static built_in funcs[] = { {"read", built_read}, {"write", built_write}, {"init", built_init}, {"close", built_close}, {0,0}};

enum { B_READ, B_WRITE, B_INIT, B_CLOSE };


typedef struct _parseTree {

	Token *token;
	unsigned int c_num;
	struct _parseTree *left, *right;
	struct _parseTree *child[MAX_C];

} parseTree;


extern void parseAnaylize(List *lst);
extern int parseType(List *lst);
extern int parseKey(List *lst);
extern parseTree *createParseTree(List *lst);
extern List *parseGetOffset(List *lst, int it);
extern  int parsePrintTree(parseTree *t);
extern int function_call(parseTree *tree, const char *name);
extern int initProgram();




#ifdef __cplusplus
}
#endif




#endif

