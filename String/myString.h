#ifndef MYSTRING_H_INCLUDED
#define MYSTRING_H_INCLUDED

struct string{
    int SizeW;
    char * matrix;
    char background;
    char tabulation;
};

struct string* inits(struct string*,int,char,char);
int DeleteString(struct string*);
struct string * AddStrings(struct string*,struct string*,int);
struct string* stringTakeValue(struct string*,char*,int);
struct string* printString(struct string*);
int stringToInt(struct string*);
struct string* InputString(char*,char);
struct string* fprintString(FILE*, struct string*);
struct string* fInputString(FILE*, char);
#endif // MYSTRING_H_INCLUDED
