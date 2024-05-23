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
    int SizeW;
};
struct Cell* initCell(char valueType,void* value, int len, struct Cell* Left, struct Cell* Right, struct Cell* Top, struct Cell* Bot){
    struct Cell* NewCell = NULL;
    while(NewCell == NULL) NewCell = malloc(sizeof(struct Cell));
    NewCell->SizeW=0;
    if(Top!=NULL)NewCell->SizeW=Top->SizeW;
    else if(Bot!=NULL)NewCell->SizeW=Bot->SizeW;
    NewCell->Left = Left;
    if(Left!=NULL)Left->Right = NewCell;
    NewCell->Right = Right;
    if(Right!=NULL)Right->Left = NewCell;
    NewCell->Top = Top;
    if(Top!=NULL)Top->Bot = NewCell;
    NewCell->Bot = Bot;
    if(Bot!=NULL)Bot->Top = NewCell;
    NewCell->valueType = valueType;
    if(valueType=='i'&&NewCell->Top!=NULL){
        NewCell->ivalue = *((int *)value);
        len=1;
        while(*((int *)value)/=10)len+=1;
    }
    else if(valueType=='c'){
        NewCell->valueType = 's';
        struct string* string1;
        string1 = malloc(sizeof(struct string));
        inits(string1,len+1,' ',' ');
        stringTakeValue(string1, (char*)value, len);
        NewCell->svalue = string1;
    }
    else if(valueType=='s'){
        NewCell->svalue = ((struct string*)value);
        len = NewCell->svalue->SizeW;
    }
    else if(valueType=='n')len=5;
    struct Cell* Buff = NewCell;
    if(len>Buff->SizeW){
        while(Buff->Top!=NULL)Buff=Buff->Top;
        while(Buff!=NULL){
            Buff->SizeW = len;
            Buff=Buff->Bot;
        }
    }
    return NewCell;
}

struct Cell* swapCell(struct Cell*Cell1, struct Cell*Cell2){
    if(Cell1->valueType=='i'){
        int buf = Cell1->ivalue;
        Cell1->valueType = Cell2->valueType;
        if(Cell1->valueType=='i')
            Cell1->ivalue = Cell2->ivalue;
        else if(Cell1->valueType=='s')
            Cell1->svalue = Cell2->svalue;
        Cell2->valueType = 'i';
        Cell2->ivalue = buf;
    }
    else if(Cell1->valueType=='s'){
        struct string* buf = Cell1->svalue;
        Cell1->valueType = Cell2->valueType;
        if(Cell1->valueType=='i')
            Cell1->ivalue = Cell2->ivalue;
        else if(Cell1->valueType=='s')
            Cell1->svalue = Cell2->svalue;
        Cell2->valueType = 's';
        Cell2->svalue = buf;
    }
};

int PrintCell(struct Cell*Cell1){
    if(Cell1->SizeW<1){
        if(Cell1->valueType=='n')printf(" none ");
        return 0;
    }
    struct string* s1;
    s1=malloc(sizeof(struct string));
    inits(s1,Cell1->SizeW,' ', ' ');
    if(Cell1==NULL)return -1;
    if(Cell1->valueType=='i'){
        struct string* s2;
        s2=IntToString(Cell1->ivalue);
        AddStrings(s1,s2, Cell1->SizeW-s2->SizeW);
        free(s2);
    }
    if(Cell1->valueType=='s')AddStrings(s1,Cell1->svalue, (Cell1->SizeW-Cell1->svalue->SizeW+1)/2);
    if(Cell1->valueType=='n')stringTakeValue(s1,"none", 4);
    printString(s1);
    free(s1);
}
int deleteCell(struct Cell*Cell1){
    if(Cell1!=NULL){
        if(Cell1->valueType=='s'&&Cell1->svalue!=NULL){
            DeleteString(Cell1->svalue);
        }
        free(Cell1);
    }
}

struct Exel{
    struct Cell* First;
    struct Cell* Last;
};

struct Exel* initExel(){
    struct Exel* Exel1 = NULL;
    while(Exel1 == NULL) Exel1 = malloc(sizeof(struct Exel));
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
    free(Exel1);
};
struct Cell * getCellByPos(struct Cell * root, int x, int y){
    if(x!=0||y!=0){
        if(x<0 && root->Left!=NULL)root = getCellByPos(root->Left, x += 1, y);
        else if(x>0 && root->Right!=NULL)root = getCellByPos(root->Right, x -= 1, y);
        else if(y<0 && root->Top!=NULL)root = getCellByPos(root->Top, x, y += 1);
        else if(y>0 && root->Bot!=NULL)root = getCellByPos(root->Bot, x, y -= 1);
    }
    return root;
};

int AddCellValue(struct Cell * root, struct string * value){
    if(root->valueType=='s'){
        DeleteString(root->svalue);
    }
    if(stringToInt(value)==0&&*(value->matrix)==value->tabulation){
        root->valueType='n';
        printf("none\n");
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
    struct Cell* Buff = root;
    if(value->SizeW>Buff->SizeW){
        while(Buff->Top!=NULL)Buff=Buff->Top;
        while(Buff!=NULL){
            Buff->SizeW = value->SizeW;
            Buff=Buff->Bot;
        }
    }
    return 1;
}

struct Exel* addRow(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = getCellByPos(Exel1->First, 0, Pos - 1);
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
    while(Exel1->First->Top!=NULL)Exel1->First= getCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First= getCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};
struct Exel* addCollom(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = getCellByPos(Exel1->First, Pos - 1, 0);
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
    while(Exel1->First->Top!=NULL)Exel1->First= getCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First= getCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};
struct Exel* deleteRow(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = getCellByPos(Exel1->First, 0, Pos - 1);
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
            if(CellBuf1!=NULL)CellBuf1->Top = CellBuf;
            CellBuf=CellBuf->Right;
        }
    }
    while(Exel1->First->Top!=NULL)Exel1->First= getCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First= getCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};
struct Exel* deleteCollom(struct Exel* Exel1, int Pos){
    struct Cell * CellBuf = getCellByPos(Exel1->First, Pos - 1, 0);
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
            CellBuf->Right = CellBuf1;
            if(CellBuf1!=NULL)CellBuf1->Left = CellBuf;
            CellBuf=CellBuf->Bot;
        }
    }
    while(Exel1->First->Top!=NULL)Exel1->First= getCellByPos(Exel1->First, 0, -1);
    while(Exel1->First->Left!=NULL)Exel1->First= getCellByPos(Exel1->First, -1, 0);
    while(Exel1->Last->Bot!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 0, 1);
    while(Exel1->Last->Right!=NULL)Exel1->Last= getCellByPos(Exel1->Last, 1, 0);
    return Exel1;
};

int printExel(struct Exel* Exel1){
    int i =0, j=1, g=1;
    char N[4]={-55,-51,-69,-53};
    struct string *Num;
    Num = malloc(sizeof(struct string));
    struct Cell *BufCell1 = Exel1->First;
    while(BufCell1!=NULL){
        j++;
        BufCell1 = BufCell1->Bot;
    }
    Num = inits(Num,j+2,' ',' ');
    BufCell1 = Exel1->First;
    {
        printf("%c",N[0]);
        for(i=0;i<Num->SizeW;i++)printf("%c",N[1]);
        printf("%c",N[3]);
        struct Cell *BufCell2 = BufCell1;
        while(BufCell2!=NULL){
            for(i=0;i<BufCell2->SizeW;i++)printf("%c",N[1]);
            if(BufCell2->Right==NULL)printf("%c",N[2]);
            else printf("%c",N[3]);
            BufCell2 = BufCell2->Right;
        }
        printf("\n");
        struct string* s2;
        s2=IntToString(g++);
        AddStrings(Num,s2, Num->SizeW-s2->SizeW);
        free(s2);
    }
    char N2[4]={-52,-51,-71,-50};
    char N3[4]={-56,-51,-68,-54};
    while(BufCell1!=NULL){
        struct Cell *BufCell2 = BufCell1;
        printf("%c",-70);
        if(BufCell1->Top!=NULL){
            printString(Num);
            struct string* s2;
            s2=IntToString(g++);
            AddStrings(Num,s2, Num->SizeW-s2->SizeW);
            free(s2);
        }
        else {
            printf("Nam");
            for(i=0;i<Num->SizeW-3;i++)printf("%c",' ');
        }
        printf("%c",-70);
        while(BufCell2!=NULL){
            struct Cell *BufCell3 = BufCell2;
            BufCell2 = BufCell2->Right;
            PrintCell(BufCell3);
            printf("%c",-70);
        }
        printf("\n");
        BufCell2 = BufCell1;
        if(BufCell1->Bot!=NULL){
            printf("%c",N2[0]);
            for(i=0;i<Num->SizeW;i++)printf("%c",N2[1]);
            printf("%c",N2[3]);
            while(BufCell2!=NULL){
                for(i=0;i<BufCell2->SizeW;i++)printf("%c",N2[1]);
                if(BufCell2->Right==NULL)printf("%c",N2[2]);
                else printf("%c",N2[3]);
                BufCell2 = BufCell2->Right;
            }
        }
        else{
            printf("%c",N3[0]);
            for(i=0;i<Num->SizeW;i++)printf("%c",N3[1]);
            printf("%c",N3[3]);
            while(BufCell2!=NULL){
                for(i=0;i<BufCell2->SizeW;i++)printf("%c",N3[1]);
                if(BufCell2->Right==NULL)printf("%c",N3[2]);
                else printf("%c",N3[3]);
                BufCell2 = BufCell2->Right;
            }
        }
        BufCell1 = BufCell1->Bot;
        printf("\n");
    }
    DeleteString(Num);
}

int printExelToFile(struct Exel* Exel1, char* fileToPrint){
    FILE * f1 = NULL;
    f1 = fopen((const char*)fileToPrint, "w");
    struct Cell * BufCell1 = Exel1->First;
    while(BufCell1!=NULL){
        fprintf(f1,"/");
        struct Cell * BufCell2 = BufCell1;
        while(BufCell2!=NULL){
            if(BufCell2->valueType=='s')fprintString(f1,BufCell2->svalue);
            if(BufCell2->valueType=='i')fprintf(f1,"%d",BufCell2->ivalue);
            if(BufCell2->valueType=='n')fprintf(f1,"none");
            fprintf(f1,"/");
            BufCell2=BufCell2->Right;
        }
        BufCell1=BufCell1->Bot;
    }
    fclose(f1);
}

struct Exel* getExelFromFile(char* fileToGet) {
    FILE* f1 = NULL;
    f1 = fopen( (const char*)fileToGet, "r");
    struct Cell* CellBuf1;
    struct Cell* CellBuf;
    struct string* bufString3;
    struct Exel* Exel1;
    Exel1 = initExel();
    CellBuf1 = Exel1->First;
    while ((bufString3 = fInputString(f1, '/')) != NULL) {
        if (bufString3->SizeW > 1){
            if (CellBuf1->valueType!='n') {
                CellBuf->Right = initCell('n', 0, 0, CellBuf, CellBuf->Right, CellBuf->Top == NULL ? NULL : CellBuf->Top->Right, NULL);
                CellBuf = CellBuf->Right;
            }
            AddCellValue(CellBuf, bufString3);

        } else {
            if(CellBuf1->valueType!='n'){
                CellBuf1->Bot = initCell('n', 0, 0, CellBuf1->Left==NULL?NULL:CellBuf1->Left->Bot, NULL, CellBuf1, CellBuf1->Bot);
                CellBuf1 = CellBuf1->Bot;
            }
            CellBuf = CellBuf1;

        }
    }
    return Exel1;
}

int sortExel(struct Exel* Exel1, int pos) {
    struct Cell* firstCell = getCellByPos(Exel1->First->Bot, pos, 0);
    struct Cell* CellBuf1 = firstCell;
    int i,j,g = 0, strLen = 0;
    while (firstCell != NULL) {
        g++;
        firstCell = firstCell->Bot;
    }

    firstCell = Exel1->First;
    while (firstCell != NULL) {
        strLen++;
        firstCell = firstCell->Right;
    }

    for (i=0; i < g; i++) {
        for (j = 0; j < g-1; ++j) {
            struct Cell* CellBuf2 = getCellByPos(CellBuf1, 0, j);
            struct Cell* CellBuf3 = CellBuf2->Bot;
            if (CellBuf3->valueType == 'i') {
                if (CellBuf3->ivalue > CellBuf2->ivalue) {
                    for (int k = 0; k < strLen; ++k) {
                        struct Cell * CellBuf4 = getCellByPos(Exel1->First->Bot, k, j);
                        struct Cell * CellBuf5 = CellBuf4->Bot;
                        swapCell(CellBuf4, CellBuf5);

                    }
                }

            }
            else if (CellBuf3->valueType == 's' && CellBuf2->valueType == 's') {
                if (CellBuf3->svalue == sortStrings(CellBuf2->svalue, CellBuf3->svalue)) {
                    for (int k = 0; k < strLen; ++k) {
                        struct Cell * CellBuf4 = getCellByPos(Exel1->First->Bot, k, j);
                        struct Cell * CellBuf5 = CellBuf4->Bot;
                        swapCell(CellBuf4, CellBuf5);

                    }
                }
            }

        }
    }
}

int findByCell(struct Exel *Exel1, struct Cell *value, int column) {
    struct Cell * CellBuf1 = Exel1->First;
    struct Exel * ExelBuf = initExel();
    struct Cell * CellBuf3 = ExelBuf->First;
    int amogus = 0;

    for (int i = 0; CellBuf1 != NULL; ++i, CellBuf1 = CellBuf1->Right) {
        struct Cell * CellBuf2 = ExelBuf->First;
        if (amogus != 0) {
            addCollom(ExelBuf, i);
        }
        amogus++;
        CellBuf2 = getCellByPos(ExelBuf->First, i, 0);
        CellBuf2->valueType = CellBuf1->valueType;
        if (CellBuf2->valueType == 'i') CellBuf2->ivalue = CellBuf1->ivalue;
        if (CellBuf2->valueType == 's') {
            CellBuf2->svalue = malloc(sizeof (struct string));
            inits(CellBuf2->svalue, 1, ' ', ' ');
            AddStrings(CellBuf2->svalue, CellBuf1->svalue, 0);
            if(CellBuf1->SizeW>CellBuf2->SizeW){
                while(CellBuf2->Top!=NULL)CellBuf2=CellBuf2->Top;
                while(CellBuf2!=NULL){
                    CellBuf2->SizeW = CellBuf1->SizeW;
                    CellBuf2=CellBuf2->Bot;
                }
            }
        }

    }
    CellBuf1 = getCellByPos(Exel1->First->Bot, column, 0);


    for (int y = 0; CellBuf1 != NULL ; ++y, CellBuf1 = CellBuf1->Bot) {
        if (CellBuf1->valueType == value->valueType) {
            if (CellBuf1->valueType == 'i') {
                if (CellBuf1->ivalue == value->ivalue) {
                    addRow(ExelBuf, y);
                    struct Cell *CellBuf2 = getCellByPos(ExelBuf->First->Bot, 0, y);
                    struct Cell *CellBuf4 = getCellByPos(Exel1->First->Bot, 0, y);
                    while (CellBuf4 != NULL) {
                        CellBuf2->valueType = CellBuf4->valueType;
                        if (CellBuf2->valueType == 'i') CellBuf2->ivalue = CellBuf4->ivalue;
                        if (CellBuf2->valueType == 's') {
                            CellBuf2->svalue = malloc(sizeof (struct string));
                            inits(CellBuf2->svalue, 1, ' ', ' ');
                            AddStrings(CellBuf2->svalue, CellBuf4->svalue, 0);
                            if(CellBuf4->SizeW>CellBuf2->SizeW){
                                while(CellBuf2->Top!=NULL)CellBuf2=CellBuf2->Top;
                                while(CellBuf2!=NULL){
                                    CellBuf2->SizeW = CellBuf4->SizeW;
                                    CellBuf2=CellBuf2->Bot;
                                }
                            }
                        }
                        CellBuf4 = CellBuf4->Right;
                        CellBuf2 = CellBuf2->Right;
                    }
                }
            }
            else if (CellBuf1->valueType == 's') {
                if ((CellBuf1->svalue == sortStrings(CellBuf1->svalue,value->svalue)) && (value->svalue == sortStrings(value->svalue,CellBuf1->svalue))) {
                    addRow(ExelBuf, y);
                    struct Cell *CellBuf2 = getCellByPos(ExelBuf->First->Bot, 0, y);
                    struct Cell *CellBuf4 = getCellByPos(Exel1->First->Bot, 0, y);
                    CellBuf2->valueType = CellBuf4->valueType;
                    if (CellBuf2->valueType == 'i') CellBuf2->ivalue = CellBuf4->ivalue;
                    if (CellBuf2->valueType == 's') {
                        CellBuf2->svalue = malloc(sizeof (struct string));
                        inits(CellBuf2->svalue, 1, ' ', ' ');
                        AddStrings(CellBuf2->svalue, CellBuf4->svalue, 0);
                    }
                }
            }
        }
    }

    printExel(ExelBuf);
    deleteExel(ExelBuf);
}

int findByValue(struct Exel *Exel1, struct string *value, int column) {
    struct Cell* Cell1 = initCell('n', 0, 0, 0, 0, 0, 0);
    AddCellValue(Cell1, value);
    findByCell(Exel1, Cell1, column);
    deleteCell(Cell1);
    return 0;
}



int main()
{
    int secst = 0;
    printf("Hello in my SUBD\n");
    struct Exel* Exel1 = getExelFromFile("txt.txt");
    while(secst!=10){
        printExel(Exel1);
        printf("Take the number of action from list:\n 1)Add a Row\n 2)Add a Collom\n 3)Set Value\n 4)Set Values\n 5)Delete a row\n 6)Delete a Collom\n 7)Save Dok\n 8)Sort by column\n 9)Find by value \n 10)End work\n");
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
            int Nam1=0,Nam2=0;
            struct string* s1;
            s1 = InputString("Set the number of row\n", '/n');
            Nam1 = stringToInt(s1);
            DeleteString(s1);
            s1 = InputString("Set the number of Collom\n", '/n');
            Nam2 = stringToInt(s1);
            DeleteString(s1);
            int f=0;
            while(f==0){
                s1 = InputString("Set the value\n",' ');
                f = AddCellValue(getCellByPos(Exel1->First, Nam2 - 1, Nam1), s1);
                if(f==0){
                    DeleteString(s1);
                }
            }
        }
        if(secst==4){
            int Nam1=0,Nam2=0,Nam3=0,Nam4=0, i,j;
            struct string* s1;
            s1=InputString("Set the row number of start Cell\n", '/n');
            Nam1 = stringToInt(s1);
            DeleteString(s1);
            s1=InputString("Set the collom number of start Cell\n", '/n');
            Nam2 = stringToInt(s1);
            DeleteString(s1);
            s1=InputString("Set the row number of end Cell\n", '/n');
            Nam3 = stringToInt(s1);
            DeleteString(s1);
            s1=InputString("Set the collom number of end Cell\n", '/n');
            Nam4 = stringToInt(s1);
            DeleteString(s1);
            for(i=Nam1;i<=Nam3;i++){
                for(j=Nam2;j<=Nam4;j++){
                    int f=0;
                    while(f==0){
                        s1 = InputString("Set the value\n",' ');
                        f = AddCellValue(getCellByPos(Exel1->First, j - 1, i), s1);
                        if(f==0){
                            DeleteString(s1);

                        }
                    }

                }
            }
        }
        if(secst==5){
            int Nam=0;
            struct string* s1;
            s1=InputString("Set the namber of row\n", '/n');
            Nam = stringToInt(s1);
            free(s1);
            Exel1 = deleteRow(Exel1, Nam+1);
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
            s1=InputString("Set file name\n", '\0');
            printExelToFile(Exel1, s1->matrix);
        }
        if (secst == 8) {
            struct string* s1;
            s1 = InputString("Set position\n", '\0');
            int num = stringToInt(s1);
            sortExel(Exel1, num-1);
            DeleteString(s1);
        }
        if (secst == 9) {
            struct string *s1;
            s1 = InputString("Input column\n", '\0');
            int num = stringToInt(s1);
            DeleteString(s1);
            s1 = InputString("Input value\n", '\0');
            findByValue(Exel1, s1, num-1);
        }
    }
    printExelToFile(Exel1, "txt.txt");
    deleteExel(Exel1);
    return 0;
}
