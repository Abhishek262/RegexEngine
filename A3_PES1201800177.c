#include<stdio.h>

#define DEBUG 1
int match_here(char *pat, char *text);
int match_star_non_greedy(char ch, char* pat, char* text);
int match_star_greedy(int c, char *pat, char *text);

// int start = 0;
// int end = 0;

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

int match_star_non_greedy(char ch, char* pat, char* text)
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

int match_star_greedy(int c, char *pat, char *text)
{
	char *t;

	for (t = text; *t != '\0' && (*t == c || c == '.'); t++)
		;
	do {	/* * matches zero or more */
		if (matchhere(pat, t))
			return 1;
	} while (t-- > text);
	return 0;
}

int main(){
    char text[4000];
    int M;
    int pattern;
    scanf("%[^\n]s",text);
    scanf("%d",&M);
    for(int i=0;i<M;i++){
        scanf("%[^\n]s",pattern);
        printf("%d\n",match(pattern,text));
    }


}