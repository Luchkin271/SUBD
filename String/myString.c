#include <stdio.h>
#include <stdlib.h>
#include "myString.h"


struct string* inits(struct string *string1, int SizeW, char background, char tabulation){/*init all string positions and fill string by background*/
    string1->SizeW = SizeW;
    string1->background = background;
    string1->tabulation = tabulation;
    string1->matrix = calloc(string1->SizeW, sizeof(char));
    int i;
    for(i=0;i<string1->SizeW;i++)
        *(string1->matrix+i) = i!=string1->SizeW-1?background:string1->tabulation;
    return string1;
};


int DeleteString(struct string * s1){/*clears the string memory*/
    if(s1->matrix!=NULL)free(s1->matrix);
    s1->matrix=NULL;
    return 0;
}


struct string* AddStrings(struct string * string1, struct string * string2, int Pos){/*the function puts string2 in string1 starting from position*/
    string1->SizeW=Pos+string2->SizeW;
    string1->matrix = realloc(string1->matrix, sizeof(char)*string1->SizeW);
    int i;
    for(i=Pos;i<string1->SizeW;i++)
        *(string1->matrix+i)=*(string2->matrix+i-Pos)==string2->background?string1->background:*(string2->matrix+i-Pos);
    *(string1->matrix+string1->SizeW-1)=string1->tabulation;

    return string1;
};


struct string* stringTakeValue(struct string *string1, char* value, int len){/*the function overwrites the old value with the background and places the new value*/
    struct string string3;
    string3.SizeW=len+1;
    string3.background = ' ';
    string3.tabulation = string1->tabulation;
    int i;
    for(i=0;i<string1->SizeW;i++)
        *(string1->matrix+i) = i!=string1->SizeW-1?string1->background:string1->tabulation;
    string3.matrix = calloc(len,sizeof(char));
    for(i=0;i<len;i++)
        *(string3.matrix+i)= *(value+i);
    *(string3.matrix+len)=string1->tabulation;
    AddStrings(string1, &string3, string1->SizeW-len-1);
    DeleteString(&string3);
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
    printf("%d\n", i+1);
    inits(s1,i+1,' ', tabulation);
    stringTakeValue(s1, s1_1, i);
    free(s1_1);
    return s1;
};

struct string* fInputString(FILE* file1, char tabulation){
    if(file1==NULL)return NULL;
    struct string* s1;
    s1=malloc(sizeof(struct string));
    char* s1_1;
    s1_1=malloc(sizeof(char));
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
