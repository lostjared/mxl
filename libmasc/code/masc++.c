#include "masc++.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

char char_map[0xFF];




void mascFillMap(char *char_map)
{

	unsigned int i = 0;

	memset(char_map, 0, sizeof(char_map));
	for(i = 'a'; i <= 'z';  i++)
		char_map[i] = MID_LETTER;

	for(i = 'A'; i <= 'Z'; i++)
		char_map[i] = MID_LETTER;

	for(i = '0'; i <= '9'; i++)
		char_map[i] = MID_DIGIT;
	
	char_map['0'] = MID_DIGIT;
	

	char_map['{'] = char_map['}'] = char_map[';'] = char_map['.'] = MID_SYMBOL;
	char_map['('] = char_map[')'] = char_map['$'] = char_map['\"'] = MID_SYMBOL;
	char_map['+'] = char_map['-'] = char_map['&'] = char_map['*'] = MID_SYMBOL;
	char_map['/'] = char_map['!'] = char_map['#'] = char_map['='] = MID_SYMBOL;
	char_map[','] = char_map['^'] = char_map['|'] = char_map['<'] = char_map['>'] = MID_SYMBOL;
	char_map[':'] = char_map['['] = char_map[']'] = char_map['\''] =  MID_SYMBOL;
	char_map['@'] = MID_SYMBOL;
	char_map['_'] = MID_LETTER;
	char_map['.'] = MID_DIGIT;
}

void mascInit(mascToken *token)
{
	memset(token, 0, sizeof(token));
	mascFillMap(char_map);
	token->pos = 0;
	token->tok_pos = 0;
	token->line = 1;
	token->offset = 0;

}

void mascTokenReset(mascToken *token)
{
	free(token->token);
	token->token = (char*) malloc ( sizeof(char) * 255 );
	memset(token->token, 0, sizeof(token->token));
	token->tok_pos = 0;
}

void mascTokenAdd(mascToken *token, char c)
{
	if((token->tok_pos % 254) == 0)
	{
		token->token = (char*) realloc (token->token,token->tok_pos+255);
	}
	token->token[token->tok_pos++] = c;
	token->token[token->tok_pos] = 0;
}

char mascPeek(mascToken *token, const char *source)
{

	if(token->pos + 1 < token->source_length)
		return source[token->pos+1];

	return 0;
}
int mascLetter(mascToken *token, const char *source)
{

	u8 type;

	do
	{
		if(token->pos >= token->source_length && token->tok_pos != 0) return MID_LETTER;
		else if(token->pos >= token->source_length) return 0;
		type = char_map[(int)source[token->pos]];
		if(type == MID_LETTER || type == MID_DIGIT)
		{
			 mascTokenAdd(token, source[token->pos]);
			 token->pos++;
		}

	} while ( type == MID_LETTER || type == MID_DIGIT );
	

	return MID_LETTER;
}

int mascDigit(mascToken *token, const char *source)
{
	u8 type;
	
	
	
	if(source[token->pos] == '0' && mascPeek(token, source) == 'x') {
		
		token->pos += 2;
				
		while ( token->pos < token->source_length && (isdigit(source[token->pos]) || (tolower(source[token->pos]) >= 'a' && tolower(source[token->pos]) <= 'f'))) {
	
			if(token->pos >= token->source_length && token->tok_pos != 0) return MID_HEX;
				else if(token->pos >= token->source_length) return 0;
						
			mascTokenAdd(token, source[token->pos]);
			token->pos++;	
		}
		return MID_HEX;
	}
	

	do
	{

		if(token->pos >= token->source_length && token->tok_pos != 0) return MID_DIGIT;
		else if(token->pos >= token->source_length) return 0;
	

		type = char_map[(int)source[token->pos]];
		
		
		if(type == MID_DIGIT || source[token->pos] == '.')
		{
			 mascTokenAdd(token, source[token->pos]);
			 token->pos++;
		}
		
	} while ( type == MID_DIGIT || source[token->pos] == '.' );
	
	return MID_DIGIT;
}

int mascSymbol(mascToken *token, const char *source)
{
	char c;

	if(token->pos >= token->source_length) return 0;
	c = source[token->pos];
	switch(c)
	{
	case '[':
	case ']':
	case '(':
	case ')':
	case '{':
	case '}':
	case ',':
	case '$':
	case '.':
	case ';':
	case ':':
	case '@':
		mascTokenAdd(token, c);
		token->pos++;
		return MID_SYMBOL;

	case '#':
	{

		while(token->pos < token->source_length && source[token->pos++] != '\n');
		
		if(source[token->pos-1] == '\n') token->line++;
		
		return MID_SKIP;


	}
	break;

	case '/':
	case '+':
	case '-':
	case '=':
	case '*':
	case '!':
	case '>':
	case '<':
	case '&':
	case '|':
	case '^':
	case '%':

		
		if(c == '-' && mascPeek(token,source) == '>')
		{
			mascTokenAdd(token, c);
			mascTokenAdd(token, mascPeek(token, source));
			token->pos++;
		}
		else
		if(mascPeek(token, source) == '=')
		{
			mascTokenAdd(token, c);
			mascTokenAdd(token, mascPeek(token, source));
			token->pos++;
		}
		else if(mascPeek(token, source) == c) 
		{
			mascTokenAdd(token, c);
			mascTokenAdd(token, c);
			token->pos++;
		}
		else mascTokenAdd(token, c);

		token->pos++;
		return MID_SYMBOL;

	case '\"':
	case '\'':
	{
		char fc = c;
		char c;
	

		do
		{
			token->pos++;
			if(token->pos >= token->source_length) return 0;
			c = source[token->pos];
			if(c == '\\' && mascPeek(token, source) != 0) 
			{
				char nc;
				token->pos++;
				nc = source[token->pos];	

				if(nc == 'n')
				mascTokenAdd(token, '\n');
				if(nc == '\"')
				mascTokenAdd(token, '\"');
				if(nc == '\'')
				mascTokenAdd(token, '\'');
		
			}
			else if(c !=  fc) mascTokenAdd(token, c);
		}
		while( c != fc);
		token->pos++;
		return MID_STRING;
	
	}
	break;
	default:
		token->pos++;
	
	}

	return MID_SYMBOL;
}


int mascLex(mascToken *token, const char *source)
{
	u8 type;

	if(token->pos >= token->source_length) return 0;

	type = char_map[(unsigned int)source[token->pos]];


	if(type == MID_LETTER)
		return mascLetter(token, source);
	else if(type == MID_DIGIT)
		return mascDigit(token, source);
	else if(type == MID_SYMBOL)
		return mascSymbol(token, source);



	if(type == MID_NULL)
	{
		
		if(source[token->pos] == '\n') token->line++;
		
		do
		{
			token->pos++;
			if(token->pos >= token->source_length) return 0;
			type = char_map[(unsigned int)source[token->pos]];
			
			if(source[token->pos] == '\n') token->line++;
			
		}
		while ( type == MID_NULL || token->pos >= token->source_length );

		if(token->pos >= token->source_length)
			return 0;
	} 
	

	return MID_SKIP;
}











