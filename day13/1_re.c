#include <stdio.h>
#include "1_re.h"

#if 0
// version 0 : 
// 1. empty pattern
int match_here(char *pat, char *text);

int match(char *pat, char *text)
{
	if(match_here(pat, text))
		return 1;
	return 0;
}

int match_here(char *pat, char *text)
{
	// empty pattern
	if(pat[0] == '\0')
		return 1;
	return 0;	
}
#endif

#if 0
// version 1 : 
// 1. empty pattern
// 2. exact match
int match_here(char *pat, char *text);

int match(char *pat, char *text)
{
	do
	{
		if(match_here(pat, text))
			return 1;
	} 	while(*text++ != '\0');
	return 0;
}

int match_here(char *pat, char *text)
{
	// empty pattern
	if(*pat == '\0')
		return 1;
	if(*text != '\0' && *pat == *text)
	{
		return match_here(pat + 1, text + 1);
	}	
	return 0;	
}
#endif


#if 0
// version 2 : 
// 1. empty pattern
// 2. exact match
// 3. anchor ^
int match_here(char *pat, char *text);

int match(char *pat, char *text)
{
	if(*pat == '^')
	{
		return match_here(pat + 1, text);
	}
	do
	{
		if(match_here(pat, text))
			return 1;
	} 	while(*text++ != '\0');
	return 0;
}
#endif

#if 0
// version 3 : 
// 1. empty pattern
// 2. exact match
// 3. anchor ^
// 4. anchor $
int match_here(char *pat, char *text);

int match(char *pat, char *text)
{
	if(*pat == '^')
	{
		return match_here(pat + 1, text);
	}
	do
	{
		if(match_here(pat, text))
			return 1;
	} 	while(*text++ != '\0');
	return 0;
}
int match_here(char *pat, char *text)
{
	// empty pattern
	if(*pat == '\0')
		return 1;
	if(pat[0] == '$' && pat[1] == '\0')
		return *text == '\0';	
	if(*text != '\0' && *pat == *text)
	{
		return match_here(pat + 1, text + 1);
	}	
	return 0;	
}
#endif
// version 3 : 
// 1. empty pattern
// 2. exact match
// 3. anchor ^
// 4. anchor $
// 5. metachar .
// 6. closure *
#if 1
#define DEBUG 1
int match_here(char *pat, char *text);
int match_star(char ch, char* pat, char* text);

int match(char *pat, char *text)
{
	if(*pat == '^')
	{
		return match_here(pat + 1, text);
	}
	do
	{
		if(match_here(pat, text))
			return 1;
	} 	while(*text++ != '\0');
	return 0;
}
int match_here(char *pat, char *text)
{
	// empty pattern
	if(*pat == '\0')
		return 1;
	if(pat[1] == '*')
		return match_star(pat[0], pat+2, text);
	if(pat[0] == '$' && pat[1] == '\0')
		return *text == '\0';	
	if(*text != '\0' && (*pat == '.' || *pat == *text))
	{
		return match_here(pat + 1, text + 1);
	}	
	return 0;	
}

int match_star(char ch, char* pat, char* text)
{
	do
	{
		if(DEBUG)
			putchar(*text);

		if(match_here(pat, text))
			return 1;
	} while(*text != '\0' && (*text++ == ch || ch == '.'));
	return 0;
}
#endif

