#include <stdio.h>
#include <string.h>

int match_bracket(char *string, char ch)
{
   char *temp;
   int start_b, end_b;
   for(int i=0;i<strlen(string);i++){
      if(string[i+1]=='-'){
         start_b = string[i];
         end_b = string[i+2];
         if(ch >= start_b && ch<= end_b){
            return 1;
         }
      }
      else if(string[i]==ch){
         return 1;
      }
      
   }
   return 0;
}

int main()
{

   printf("%d\n",match_bracket("ghiA-Z1-9abcd",'1'));
   printf("%d\n",match_bracket("A-Z1-9abcd",'a'));
   printf("%d\n",match_bracket("A-Z1-9abcd",'d'));
   printf("%d\n",match_bracket("ghiA-Z1-9abcd",'g'));
   printf("%d\n",match_bracket("A-Z1-9abcd",'0'));
   printf("%d\n",match_bracket("A-Z1-9abcd",'6'));
   printf("%d\n",match_bracket("A-Z1-9abcd",'9'));
   printf("%d\n",match_bracket("A-Z1-9abcd",'+'));


}
