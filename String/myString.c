#include <stdio.h>
#include <stdlib.h>
#include "myString.h"


struct string* inits(struct string *string1, int SizeW, char background, char tabulation){/*init all string positions and fill string by background*/
    string1->SizeW = SizeW;
    string1->background = background;
    string1->tabulation = tabulation;
    string1->matrix = NULL;
    while(string1->matrix == NULL)string1->matrix = calloc(string1->SizeW, sizeof(char));
    int i;
    for(i=0;i<string1->SizeW;i++)
        *(string1->matrix+i) = i!=string1->SizeW-1?background:string1->tabulation;
    return string1;
};


int DeleteString(struct string * s1){/*clears the string memory*/
    if(s1->matrix!=NULL)free(s1->matrix);
    s1->matrix=NULL;
    free(s1);
    return 0;
}


struct string* AddStrings(struct string * string1, struct string * string2, int Pos){/*the function puts string2 in string1 starting from position*/
    string1->SizeW=Pos+string2->SizeW>string1->SizeW?Pos+string2->SizeW:string1->SizeW;
    string1->matrix = realloc(string1->matrix, sizeof(char)*string1->SizeW);
    int i;
    for(i=Pos;i<Pos+string2->SizeW;i++)
        *(string1->matrix+i)=*(string2->matrix+i-Pos)==string2->background?string1->background:*(string2->matrix+i-Pos);
    *(string1->matrix+string1->SizeW-1)=string1->tabulation;

    return string1;
};


struct string* stringTakeValue(struct string *string1, char* value, int len){/*the function overwrites the old value with the background and places the new value*/
    struct string *string3 = NULL;
    while (string3 == NULL) string3 = malloc(sizeof (struct string));
    string3->SizeW=len+1;
    string3->background = ' ';
    string3->tabulation = string1->tabulation;
    int i;
    for(i=0;i<string1->SizeW;i++)
        *(string1->matrix+i) = i!=string1->SizeW-1?string1->background:string1->tabulation;
    string3->matrix = calloc(len,sizeof(char));
    for(i=0;i<len;i++)
        *(string3->matrix+i)= *(value+i);
    *(string3->matrix+len)=string1->tabulation;
    AddStrings(string1, string3, string1->SizeW-len-1);
    DeleteString(string3);
    return string1;
};


struct string* printString(struct string *string1){
    int i;
    for(i=0;i<string1->SizeW;i++)
        printf("%c",*(string1->matrix+i));
    return string1;
};

int stringToInt(struct string *string1)
{
    int res=0;
    int i=0;
    for(i=0; i<string1->SizeW; i++){
        if(*(string1->matrix+i)>=48&&*(string1->matrix+i)<=57)res=res*10+*(string1->matrix+i)-48;
    }
    return res;
}

struct string* InputString( char* message, char tabulation){
    printf("%s", message);
    struct string* s1;
    s1=malloc(sizeof(struct string));
    char* s1_1;
    s1_1=malloc(sizeof(char));
    char sb;
    int i=0;
    while(scanf("%c",&sb)>0&&sb!='\n'){
        s1_1=realloc(s1_1,sizeof(char)*++i);
        *(s1_1+i-1)=sb;
    }
    inits(s1,i+1,' ', tabulation);
    stringTakeValue(s1, s1_1, i);
    free(s1_1);
    return s1;
};

struct string* fInputString(FILE* file1, char tabulation){
    if(file1==NULL)return NULL;
    struct string* s1;
    s1=NULL;
    while (s1==NULL) s1 = malloc(sizeof(struct string));
    char* s1_1;
    s1_1=NULL;
    while (s1_1==NULL) s1_1 = malloc(sizeof(char));
    char sb;
    int i=0;
    while((sb=fgetc(file1))!=tabulation&&sb!=EOF){
        s1_1=realloc(s1_1,sizeof(char)*++i);
        *(s1_1+i-1)=sb;
    }
    if (sb == EOF) {
        free(s1_1);
        free(s1);
        return NULL;
    }
    inits(s1,i+1,' ', ' ');
    stringTakeValue(s1, s1_1, i);
    free(s1_1);
    return s1;
}

struct string* fprintString(FILE* file1, struct string *string1){
    if(file1==NULL)return NULL;
    int i;
    for(i=0;i<string1->SizeW;i++)
        if(*(string1->matrix+i)!=string1->tabulation)
            fprintf(file1,"%c",*(string1->matrix+i));
    return string1;
};

struct string* IntToString(int i){
    struct string* s1;
    s1=malloc(sizeof(struct string));
    char* s1_1;
    s1_1=malloc(sizeof(char));
    char sb;
    int j=0;
    while(i>0){
        sb = i%10+'0';
        i/=10;
        s1_1=realloc(s1_1,sizeof(char)*++j);
        *(s1_1+j-1)=sb;
    }
    while(i<(j+1)/2){
        int a = *(s1_1+j-1-i);
        *(s1_1+j-1-i) = *(s1_1+i);
        *(s1_1+i)= a;
        i++;
    }
    inits(s1,j+1,' ', ' ');
    stringTakeValue(s1, s1_1, j);
    free(s1_1);
    return s1;
};

struct string* sortStrings(struct string* str1, struct string* str2) {
    int minLen = (str1->SizeW < str2->SizeW) ? str1->SizeW : str2->SizeW;
    int i = 0;
    int flag = (str1->SizeW < str2->SizeW) ? 0 : 1;
    for (i = 0; i < minLen; i++) {
        int j = 0, k = 0;
        int charBuffer1 = (int) *(str1->matrix+i+j);
        int charBuffer2 = (int) *(str2->matrix+i+k);
        while (charBuffer1 == str1->background) {
            charBuffer1 = (int) *(str1->matrix+i+j);
            ++j;
        }
        while (charBuffer1 == str1->background) {
            charBuffer1 = (int) *(str2->matrix+i+k);
            ++k;
        }
        printf("%d %d\n", charBuffer1, charBuffer2);

        if (charBuffer1 > 'a' && charBuffer1 < 'z' || charBuffer1 > 'A' && charBuffer1 < 'Z') {
            charBuffer1 += 500;
        } else if (charBuffer1 > '0' && charBuffer1 < '9') {
            charBuffer1 += 1000;
        }

        if (charBuffer2 > 'a' && charBuffer2 < 'z' || charBuffer2 > 'A' && charBuffer2 < 'Z') {
            charBuffer2 += 500;
        } else if (charBuffer2 > '0' && charBuffer2 < '9') {
            charBuffer2 += 1000;
        }

        if (charBuffer1 > charBuffer2) return str1;
        else if(charBuffer2 > charBuffer1) return str2;
    }

    if (str1->SizeW == minLen) return str1;
    else return str2;
}
