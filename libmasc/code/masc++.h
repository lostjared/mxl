#ifndef __MASC__H___
#define __MASC__H___

/* Variables */

#ifdef __cplusplus
extern "C" {
#endif


extern char char_map[0xFF];

typedef enum { MID_NULL = 0, MID_SKIP, MID_LETTER, MID_DIGIT,  MID_SYMBOL, MID_STRING, MID_CHAR, MID_HEX } mascType;

typedef unsigned int u32;
typedef unsigned char u8;
typedef unsigned short u16;


/* structures */

typedef struct _mascToken {
	char *token;
	u32 pos;
	u32 tok_pos;
	u32 line, offset;
	u32 source_length;
} mascToken;


/* Functions */

extern void mascFillMap(char *char_map);
extern void mascInit(mascToken *token);
extern void mascTokenReset(mascToken *token);
extern void mascTokenAdd(mascToken *token, char c);
extern int  mascLex(mascToken *token, const char *source);
extern char mascPeek(mascToken *token, const char *source);


#ifdef __cplusplus
}
#endif



#endif

