#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define DEBUG 1
// version 3 :
// 1. empty pattern
// 2. exact match
// 3. anchor ^
// 4. anchor $
// 5. metachar .
// 6. closure *
// 7. greedy, non greedy *
// 8. greedy +
// 9. question mark ?
// 10. character brackets
// 11. \w \d
// 12. escape sequences

typedef struct RE
{
	int type;  /* CHAR=1, STAR=2, PLUS=3, QUESTION_MARK=4, ANCHOR ^ =5,ANCHOR $=6 [] = 7, /. = 8 brackets get replaced by [  */
	char ch;   /* the character itself */
	char *ccl; /* for [...] instead */
			   // int     nccl;   /* true if class is negated [^...] */
} RE;

int match_here(RE **pat, char *text);
int match_star_non_greedy(RE *pat_str, RE **pat, char *text);
int match_star_greedy(RE *pat_str, RE **pat, char *text);
int match_bracket(RE *pat_str, char ch);

int start = 0;
int end = 0;
int length = 0;

RE **regex_compile(char *pat)
{

	int len = strlen(pat);
	RE **arr = (RE **)malloc(sizeof(RE *) * (len + 1));

	int c = 0;
	for (int i = 0; i < len + 1; i++)
	{
		// printf("c : %c\n",pat[c]);

		arr[i] = (RE *)malloc(sizeof(RE));

		if (pat[c] == '\0' || pat[c] == '\n')
		{
			arr[i]->ccl = NULL;
			arr[i]->ch = '\0';
			arr[i]->type = 0;
			break;
		}
		else if (pat[c] == '\\')
		{
			if (pat[c + 1] == 'w')
			{
				arr[i]->ccl = "a-zA-Z0-9_";
				arr[i]->type = 7;
				arr[i]->ch = '[';
			}
			else if (pat[c + 1] == 'd')
			{
				arr[i]->ccl = "0-9";
				arr[i]->type = 7;
				arr[i]->ch = '[';
			}
			else if(pat[c+1]=='.'){
				arr[i]->ccl = NULL;
				arr[i]->ch = '.';
				arr[i]->type = 8;
			}
			else
			{
				arr[i]->ccl = NULL;
				arr[i]->ch = pat[c + 1];
				arr[i]->type = 1;
			}
			c++;
		}
		else if (pat[c] == '*')
		{
			arr[i]->ccl = NULL;
			arr[i]->ch = '*';
			arr[i]->type = 2;
		}
		else if (pat[c] == '+')
		{
			arr[i]->ccl = NULL;
			arr[i]->ch = '+';
			arr[i]->type = 3;
		}
		else if (pat[c] == '?')
		{
			arr[i]->ccl = NULL;
			arr[i]->ch = '?';
			arr[i]->type = 4;
		}
		else if (pat[c] == '^')
		{
			arr[i]->ccl = NULL;
			arr[i]->ch = '^';
			arr[i]->type = 5;
		}
		else if (pat[c] == '$')
		{
			arr[i]->ccl = NULL;
			arr[i]->ch = '$';
			arr[i]->type = 6;
		}
		else if (pat[c] == '[')
		{
			// printf("inside\n");
			int m = 0;
			int f = c;
			for (int j = c; pat[j] != ']'; j++)
			{
				// printf("char: %c ",pat[j]);
				c++;
				m++;
			}
			arr[i]->ccl = (char *)malloc(sizeof(char) * (m + 2));
			//c is at ] now
			// c++;
			int r = 0;
			for (int x = f + 1; x < f + m; x++)
			{
				arr[i]->ccl[r] = pat[x];

				// printf("%c ",arr[i]->ccl[r]);
				r++;
			}
			arr[i]->ccl[m + 1] = '\0';
			arr[i]->ch = '[';
			arr[i]->type = 7;
		}
		else
		{
			if (pat[c] != '\0')
			{
				arr[i]->type = 1;
				arr[i]->ch = pat[c];
				arr[i]->ccl = NULL;
			}
		}
		c++;
	}
	return arr;
}

int lengthRE(RE **arr)
{
	int c = 0;
	while (1)
	{
		if (arr[c]->type == 0)
		{
			break;
		}
		c++;
	}
	// c++;
	return c;
}

void printRE(RE **arr, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("type: %d\n", arr[i]->type);
		printf("ch: %c\n", arr[i]->ch);
		printf("ccl: %s\n", arr[i]->ccl);
		printf("\n");
	}
}

int match(RE **pat, char *text)
{
	start = 0;
	end = 0;
	length = strlen(text);
	// printf("%d end: \n",length);

	if (pat[0]->ch == '^' && pat[0]->type == 5)
	{
		return match_here(pat + 1, text);
	}
	do
	{
		int temp = end;
		if (match_here(pat, text))
			return 1;
		start++;
		end = temp;
	} while (*text++ != '\0');
	return 0;
}

int match_here(RE **pat, char *text)
{
	// empty pattern
	// printf("pattern : %c\n",pat[0]->ch);
	if (pat[0]->type == 0)
		return 1;
	if (pat[1]->ch == '*' && pat[1]->type == 2)
	{
		// printf("inside *\n");
		if (pat[2]->ch == '?' && pat[2]->type == 4)
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
	if (pat[1]->ch == '+' && pat[1]->type == 3)
	{
		// printf("inside +\n");

		if (*text == pat[0]->ch || (pat[0]->ch == '.' && pat[0]->type==1) || (pat[0]->type ==7 && match_bracket(pat[0],*text)))
		{
			if (pat[2]->ch == '?' && pat[2]->type == 4)
			{
				end++;
				return match_star_non_greedy(pat[0], pat + 3, ++text);
			}
			else
			{
				end++;
				// printf("here\n");
				return match_star_greedy(pat[0], pat + 2, ++text);
			}
		}
	}
	if (pat[1]->ch == '?' && pat[1]->type == 4)
	{
		// printf("inside ?\n");

		if (pat[0]->ch == text[0] || (pat[0]->ch == '.' && pat[0]->type==1))
		{
			end++;
			// printf("GgG");
			return match_here(pat + 2, text + 1);
		}
		else
		{
			return match_here(pat + 2, text);
		}
	}
	if ((pat[0]->ch == '$' && pat[0]->type == 6) && (pat[1]->ch == '\0' && pat[1]->type == 0))
	{
		// printf("inside *$\n");

		// end = strlen(text);
		// printf("len : %d\n",length);
		end = length - start;
		return *text == '\0';
	}
	if (*text != '\0' && (pat[0]->ch == '.' || pat[0]->ch == *text || (pat[0]->type == 7 && match_bracket(pat[0], *text) == 1)))
	{
		end++;
		// printf("inside normal, char = %c\n", pat[0]->ch);

		// printf("e : %d\n",end);
		// printf("pat : %c\n",pat[0]->ch);
		return match_here(pat + 1, text + 1);
	}
	return 0;
}

int match_star_non_greedy(RE *pat_str, RE **pat, char *text)
{
	int temp;
	do
	{
		// end = temp;
		temp = end;
		if (match_here(pat, text))
		{
			// printf("\ntext : %s\n",text);
			// printf("end : %d\n",end);
			// end--;
			return 1;
		}
		// if(end!=temp){}
		end = temp;
		end++;
	} while (*text != '\0' && (*text++ == pat_str->ch || pat_str->ch == '.' || (pat_str->type == 7 && match_bracket(pat_str, *text))));
	return 0;
}

int match_star_greedy(RE *pat_str, RE **pat, char *text)
{
	char *t;
	int pos = 0;

	for (t = text; *t != '\0' && (*t == pat_str->ch || pat_str->ch == '.' || (pat_str->type == 7 && match_bracket(pat_str, *t))); t++)
	{
		pos++;
	}
	end = end + pos;
	int temp;
	do
	{ /* * matches zero or more */
		// printf("yee\n");
		temp = end;
		if (match_here(pat, t))
		{
			return 1;
		}
		end = temp - 1;
	} while (t-- > text);
	return 0;
}

int match_bracket(RE *pat_str, char ch)
{
	int start_b, end_b;
	for (int i = 0; i < strlen(pat_str->ccl); i++)
	{
		if (i < strlen(pat_str->ccl) - 1 && pat_str->ccl[i + 1] == '-')
		{
			start_b = pat_str->ccl[i];
			end_b = pat_str->ccl[i + 2];
			if (ch >= start_b && ch <= end_b)
			{
				return 1;
			}
		}
		else if (pat_str->ccl[i] == ch)
		{
			return 1;
		}
	}
	return 0;
}

int main()
{
	char text[4000];
	int M;
	char *pattern;
	pattern = NULL;
	size_t bufsize = 1000;
	int res;
	scanf("%[^\n]s", text);
	scanf("%d", &M);
	getline(&pattern, &bufsize, stdin);

	for (int i = 0; i < M; i++)
	{
		// scanf("%s", pattern);
		getline(&pattern, &bufsize, stdin);
		RE **re = regex_compile(pattern);
		// printf("%d",lengthRE(re));
		// printRE(re,lengthRE(re)+1);
		pattern[strcspn(pattern, "\n")] = 0;
		res = match(re, text);
		// printf("%s\n", pattern);

		if (res)
		{
			end = end + start - 1;
			// printf("%s\n",pattern);
			if (end == -1)
			{
				end = 0;
			}
			printf("%d %d %d\n", res, start, end);
		}
		else
		{
			// printf("%s\n",pattern);
			end = end + start - 1;
			// printf("%d %d %d\n", res, start, end);

			printf("0\n");
		}
	}
}