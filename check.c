#include<stdio.h>

int main(){
    char *pat = "abcdef";
    int c = 0;
    for(int j=0;pat[j]!='f';j++){
        c++;
    }
    printf("%c",pat[c]);
    
}