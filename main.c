#include <stdio.h>
#include <stdlib.h>
#include "./String/myString.h"



struct Cell{
    struct Cell* Left;
    struct Cell* Right;
    struct Cell* Top;
    struct Cell* Bot;
    union {
        int ivalue;
        struct string* svalue;
    };
    char valueType;
};
struct Cell* initCell(char valueType,void* value, int len, struct Cell* Left, struct Cell* Right, struct Cell* Top, struct Cell* Bot){
    struct Cell* NewCell = malloc(sizeof(struct Cell));
    NewCell->Left = Left;
    if(Left!=NULL)Left->Right = NewCell;
    NewCell->Right = Right;
    if(Right!=NULL)Right->Left = NewCell;
    NewCell->Top = Top;
    if(Top!=NULL)Top->Bot = NewCell;
    NewCell->Bot = Bot;
    if(Bot!=NULL)Bot->Top = NewCell;
    NewCell->valueType = valueType;
    if(valueType=='i') NewCell->ivalue = *((int *)value);
    if(valueType=='c'){
        NewCell->valueType = 's';
        struct string* string1;
        string1 = malloc(sizeof(struct string));
        inits(string1,len+1,' ',' ');
        stringTakeValue(string1, (char*)value, len);
        NewCell->svalue = string1;
    }
    if(valueType=='s'){
        NewCell->svalue = ((struct string*)value);
    }
    return NewCell;
}

struct Cell* swapCell(struct Cell*Cell1, struct Cell*Cell2){
    if(Cell1->valueType=='i'){
        int buf = Cell1->ivalue;
        Cell1->valueType = Cell2->valueType;
        if(Cell1->valueType=='i')
            Cell1->ivalue = Cell2->ivalue;
        if(Cell1->valueType=='s')
            Cell1->svalue = Cell2->svalue;
        Cell2->valueType = 'i';
        Cell2->ivalue = buf;
    }
    if(Cell1->valueType=='s'){
        struct string* buf = Cell1->svalue;
        Cell1->valueType = Cell2->valueType;
        if(Cell1->valueType=='i')
            Cell1->ivalue = Cell2->ivalue;
        if(Cell1->valueType=='s')
            Cell1->svalue = Cell2->svalue;
        Cell2->valueType = 's';
        Cell2->svalue = buf;
    }
};

int PrintCell(struct Cell*Cell1){
    if(Cell1==NULL)return -1;
    if(Cell1->valueType=='i')printf("%d ", Cell1->ivalue);
    if(Cell1->valueType=='s')printString(Cell1->svalue);
    if(Cell1->valueType=='n')printf(" none ");
}
int deleteCell(struct Cell*Cell1){
    if(Cell1!=NULL){
        if(Cell1->valueType=='s'&&Cell1->svalue!=NULL){
            free(Cell1->svalue);
        }
        free(Cell1);
    }
}

struct Exel{
    struct Cell* First;
    struct Cell* Last;
};

struct Exel* initExel(){
    struct Exel* Exel1 = malloc(sizeof(struct Exel));
    Exel1->First=Exel1->Last=initCell('n', NULL, 0,NULL,NULL,NULL,NULL);
    return Exel1;
};
struct Exel* deleteExel(struct Exel* Exel1){
    while(Exel1->First!=NULL){
        struct Cell *BufCell1 = Exel1->First;
        Exel1->First = Exel1->First->Bot;
        while(BufCell1!=NULL){
            struct Cell *BufCell2 = BufCell1;
            BufCell1 = BufCell1->Right;
            if(BufCell2!=NULL)deleteCell(BufCell2);
        }
    }
};
struct Cell * GetCellByPos(struct Cell * root, int x, int y){
    if(x!=0||y!=0){
        if(x<0 && root->Left!=NULL)root = GetCellByPos(root->Left, x+=1, y);
        else if(x>0 && root->Right!=NULL)root = GetCellByPos(root->Right, x-=1, y);
        else if(y<0 && root->Top!=NULL)root = GetCellByPos(root->Top, x, y+=1);
        else if(y>0 && root->Bot!=NULL)root = GetCellByPos(root->Bot, x, y-=1);
    }
    return root;
};

int AddCellValue(struct Cell * root, struct string * value){
    if(root->valueType=='s')DeleteString(root->svalue);
    if(stringToInt(value)==0&&*(value->matrix)==value->tabulation){
        root->valueType='n';
        return 0;
    }
    if(*(value->matrix)>='0'&&*(value->matrix)<='9'){
                root->valueType='i';
                root->ivalue = stringToInt(value);
                DeleteString(value);

    }
    else{
            root->valueType='s';
            root->svalue = value;
    }
    return 1;
}

struct Exel* addRow(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = GetCellByPos(Exel1->First, 0, Pos-1);
    if(Pos<1){

        while(CellBuf!=NULL){
            CellBuf->Top = initCell('n', 0, 0, CellBuf->Left==NULL?NULL:CellBuf->Left->Top, NULL,NULL, CellBuf);
            CellBuf=CellBuf->Right;
        }
    }
    else{
        while(CellBuf!=NULL){
            CellBuf->Bot = initCell('n', 0, 0, CellBuf->Left==NULL?NULL:CellBuf->Left->Bot, NULL, CellBuf, CellBuf->Bot);
            CellBuf=CellBuf->Right;
        }
    }
    while(Exel1->First->Top!=NULL)Exel1->First=GetCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First=GetCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};
struct Exel* addCollom(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = GetCellByPos(Exel1->First, Pos-1, 0);
    if(Pos<1){

        while(CellBuf!=NULL){
            CellBuf->Left = initCell('n', 0, 0, NULL, CellBuf, CellBuf->Top==NULL?NULL:CellBuf->Top->Left, NULL);
            CellBuf=CellBuf->Bot;
        }
    }
    else{
        while(CellBuf!=NULL){
            CellBuf->Right = initCell('n', 0, 0, CellBuf, CellBuf->Right, CellBuf->Top==NULL?NULL:CellBuf->Top->Right, NULL);
            CellBuf=CellBuf->Bot;
        }
    }
    while(Exel1->First->Top!=NULL)Exel1->First=GetCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First=GetCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};
struct Exel* deleteRow(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = GetCellByPos(Exel1->First, 0, Pos-1);
    if(Pos<2){
        if(CellBuf->Bot!=NULL)Exel1->First=CellBuf->Bot;
        CellBuf=CellBuf->Bot;
        while(CellBuf!=NULL&&CellBuf->Top!=NULL){
            struct Cell * CellBuf1 = CellBuf->Top->Top;
            deleteCell(CellBuf->Top);
            CellBuf->Top=CellBuf1;
            CellBuf=CellBuf->Right;
        }
    }
    else{
        if(CellBuf->Top!=NULL)Exel1->Last=CellBuf->Top;
        CellBuf=CellBuf->Top;
        while(CellBuf!=NULL&&CellBuf->Bot!=NULL){
            struct Cell * CellBuf1 = CellBuf->Bot->Bot;
            deleteCell(CellBuf->Bot);
            CellBuf->Bot=CellBuf1;
            CellBuf=CellBuf->Right;
        }
    }
    while(Exel1->First->Top!=NULL)Exel1->First=GetCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First=GetCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};
struct Exel* deleteCollom(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = GetCellByPos(Exel1->First, Pos-1, 0);
    if(Pos<2){
        if(CellBuf->Right!=NULL)Exel1->First=CellBuf->Right;
        CellBuf=CellBuf->Right;
        while(CellBuf!=NULL&&CellBuf->Left!=NULL){
            struct Cell * CellBuf1 = CellBuf->Left->Left;
            deleteCell(CellBuf->Left);
            CellBuf->Left=CellBuf1;
            CellBuf=CellBuf->Bot;
        }
    }
    else{
        if(CellBuf->Left!=NULL)Exel1->Last=CellBuf->Left;
        CellBuf=CellBuf->Left;
        while(CellBuf!=NULL&&CellBuf->Right!=NULL){
            struct Cell * CellBuf1 = CellBuf->Right->Right;
            deleteCell(CellBuf->Right);
            CellBuf->Right=CellBuf1;
            CellBuf=CellBuf->Bot;
        }
    }
    while(Exel1->First->Top!=NULL)Exel1->First=GetCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First=GetCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last=GetCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};

int printExel(struct Exel* Exel1){
    struct Cell *BufCell1 = Exel1->First;
    while(BufCell1!=NULL){
        struct Cell *BufCell2 = BufCell1;
        BufCell1 = BufCell1->Bot;
        while(BufCell2!=NULL){
            struct Cell *BufCell3 = BufCell2;
            BufCell2 = BufCell2->Right;
            PrintCell(BufCell3);
        }
        printf("\n");
    }
}

int printExelToFile(struct Exel* Exel1, char* fileToPrint){
    FILE * f1 = NULL;
    f1 = fopen((const char*)fileToPrint, "w");
    struct Cell * BufCell1 = Exel1->First;
    while(BufCell1!=NULL){
        fprintf(f1,"-");
        struct Cell * BufCell2 = BufCell1;
        while(BufCell2!=NULL){
            if(BufCell2->valueType=='s')fprintString(f1,BufCell2->svalue);
            if(BufCell2->valueType=='i')fprintf(f1,"%d",BufCell2->ivalue);
            if(BufCell2->valueType=='n')fprintf(f1,"none");
            fprintf(f1,"-");
            BufCell2=BufCell2->Right;
        }
        BufCell1=BufCell1->Bot;
    }
    fclose(f1);
}

struct Exel* getExelFromFile(char* fileToGet) {
    FILE* f1 = NULL;
    f1 = fopen( (const char*)fileToGet, "r");
    struct Cell* bufString;
    struct Cell* CellBuf;
    struct string* bufString3;
    struct Exel* Exel1;
    Exel1 = initExel();
    bufString = Exel1->First;
    while ((bufString3 = fInputString(f1, '-')) != NULL) {
        printString(bufString3);
        if (bufString3->SizeW < 2) {
            CellBuf = bufString;
            bufString->Bot = initCell('n', 0, 0, bufString->Left==NULL?NULL:bufString->Left->Bot, NULL, bufString, bufString->Bot);
            bufString = bufString->Bot;
        } else {
            if (CellBuf->Left != NULL) {
                CellBuf->Right = initCell('n', 0, 0, CellBuf, CellBuf->Right, CellBuf->Top == NULL ? NULL : CellBuf->Top->Right, NULL);
                CellBuf = CellBuf->Right;
            }
            AddCellValue(CellBuf, bufString3);

        }
    }
    return Exel1;
}

int main()
{
    int secst = 0;
    printf("Hello in my SUBD\n");
    struct Exel* Exel1 = getExelFromFile("txt.txt");
    while(secst!=8){
        printExel(Exel1);
        printf("Take the number of action from list:\n 1)Add a Row\n 2)Add a Collom\n 3)Set Value\n 4)Set Values\n 5)Delete a row\n 6)Delete a Collom\n 7)Save Dok\n 8)End work\n");
        {
            struct string* s1;
            s1=InputString("", '/n');
            secst = stringToInt(s1);
            free(s1);
        }
        if(secst==1){
            int Nam=0;
            printf("Set the namber of row\n");
            {
            struct string* s1;
            s1=InputString("", '/n');
            Nam = stringToInt(s1);
            free(s1);
            }
            Exel1 = addRow(Exel1, Nam);
        }
        if(secst==2){
            int Nam=0;
            printf("Set the namber of collom\n");
            {
            struct string* s1;
            s1=InputString("", '/n');
            Nam = stringToInt(s1);
            free(s1);
            }
            Exel1 = addCollom(Exel1, Nam);
        }
        if(secst==3){
            int Nam=0,Nam2=0;
            struct string* s1;
            s1=InputString("Set the number of row\n", '/n');
            Nam = stringToInt(s1);
            free(s1);
            s1=InputString("Set the number of Collom\n", '/n');
            Nam2 = stringToInt(s1);
            do{
            free(s1);
            s1 = InputString("Set the value\n",' ');
            }while(AddCellValue(GetCellByPos(Exel1->First, Nam-1, Nam2-1),s1)==0);
        }
        if(secst==4){
            int Nam=0,Nam2=0,Nam3=0,Nam4=0, i,j;
            struct string* s1;
            s1=InputString("Set the row number of Start Cell\n", '/n');
            Nam = stringToInt(s1);
            free(s1);
            s1=InputString("Set the collom number of Start Cell\n", '/n');
            Nam2 = stringToInt(s1);
            free(s1);
            s1=InputString("Set the row number of Start Cell\n", '/n');
            Nam3 = stringToInt(s1);
            free(s1);
            s1=InputString("Set the collom number of Start Cell\n", '/n');
            Nam4 = stringToInt(s1);
            for(;Nam2<=Nam4;Nam2++){
                int j=Nam;
                for(;j<=Nam3;j++){
                    do{
                    free(s1);
                    s1 = InputString("Set the value\n",' ');
                    }while(AddCellValue(GetCellByPos(Exel1->First, j-1, Nam2-1),s1)==0);
                }
            }
        }
        if(secst==5){
            int Nam=0;
            struct string* s1;
            s1=InputString("Set the namber of row\n", '/n');
            Nam = stringToInt(s1);
            free(s1);
            Exel1 = deleteRow(Exel1, Nam);
        }
        if(secst==6){
            int Nam=0;
            printf("Set the namber of collom\n");
            {
            struct string* s1;
            s1=InputString("", '/n');
            Nam = stringToInt(s1);
            free(s1);
            }
            Exel1 = deleteCollom(Exel1, Nam);
        }
        if(secst==7){
            struct string* s1;
            s1=InputString("Set the number of row\n", '\0');
            printExelToFile(Exel1, s1->matrix);
        }
    }
    printExelToFile(Exel1, "txt1.txt");
    deleteExel(Exel1);
    return 0;
}
