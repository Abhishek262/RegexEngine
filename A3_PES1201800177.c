#include <stdio.h>
#include <string.h>
#define DEBUG 1
// version 3 :
// 1. empty pattern
// 2. exact match
// 3. anchor ^
// 4. anchor $
// 5. metachar .
// 6. closure *

int match_here(char *pat, char *text);
int match_star_non_greedy(char ch, char *pat, char *text);
int match_star_greedy(int c, char *pat, char *text);

int start = 0;
int end = 0;
int length = 0;

int match(char *pat, char *text)
{
	start = 0;
	end = 0;
	length = strlen(text);
	// printf("%d end: \n",length);

	if (*pat == '^')
	{
		return match_here(pat + 1, text);
	}
	do
	{
		if (match_here(pat, text))
			return 1;
		start++;
	} while (*text++ != '\0');
	return 0;
}

int match_here(char *pat, char *text)
{
	// empty pattern
	if (*pat == '\0')
		return 1;
	if (pat[1] == '?')
	{
		if (pat[0] == text[0])
		{
			end++;
			return match_here(pat + 2, text + 1);
		}
		else
		{
			return match_here(pat + 2, text);
		}
	}
	if (pat[1] == '*')
	{
		if (pat[2] == '?')
		{
			// printf("non greedy\n");
			return match_star_non_greedy(pat[0], pat + 3, text);
		}
		else
		{
			// printf("greedy\n");
			return match_star_greedy(pat[0], pat + 2, text);
		}
	}
	if (pat[1] == '+')
	{
		if (*text == pat[0])
		{
			end++;
			return match_star_greedy(pat[0], pat + 2, ++text);
		}
	}
	if (pat[0] == '$' && pat[1] == '\0')
	{
		// end = strlen(text);
		// printf("len : %d\n",length);
		end = length - start + 1;
		return *text == '\0';
	}
	if (*text != '\0' && (*pat == '.' || *pat == *text))
	{
		end++;
		return match_here(pat + 1, text + 1);
	}
	return 0;
}

int match_star_non_greedy(char ch, char *pat, char *text)
{
	do
	{
		// if (DEBUG)
		// 	putchar(*text);

		if (match_here(pat, text))
			return 1;
		end++;
	} while (*text != '\0' && (*text++ == ch || ch == '.'));
	return 0;
}

int match_star_greedy(int c, char *pat, char *text)
{
	char *t;
	int pos = 0;

	for (t = text; *t != '\0' && (*t == c || c == '.'); t++)
	{
		pos++;
	}
	end = end + pos;
	do
	{ /* * matches zero or more */
		// printf("yee\n");
		if (match_here(pat, t))
			return 1;
		end--;
	} while (t-- > text);
	return 0;
}

int main()
{
	char text[4000];
	int M;
	char pattern[1000];
	int res;
	scanf("%[^\n]s", text);
	scanf("%d", &M);

	for (int i = 0; i < M; i++)
	{
		scanf("%s", pattern);
		// printf("%s\n%s",pattern,text);
		res = match(pattern, text);

		if (res)
		{
			end = end + start - 1;
			printf("%d %d %d\n", res, start, end);
		}
		else
		{
			printf("0\n");
		}
	}
}