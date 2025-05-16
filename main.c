#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // pro praci s textovymi retezci
#include <stdbool.h> // pro praci s typem bool a konstantami true a false
// #include <ctype.h>   // isalpha, isspace, islower, isupper, ...
#include <math.h>    // funkce z matematicke knihovny
// #include <float.h>   // konstanty pro racionalni typy DBL_MAX, DBL_DIG, ...
// #include <limits.h>  // konstanty pro celociselne typy INT_MAX, INT_MIN, ...
// #include <time.h>    // funkce time a dalsi pro praci s casem
#include <stdint.h>


typedef struct {
    float ** prvek;
    int radku;
    int sloupcu;
} Tmatice;

typedef struct {
    char nazev[21];
    int vyska;
    bool active;
} TData;

typedef struct {
    TData * data;
    int size;
} TArray;

typedef struct StackNode {
    char value;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode * top;
    StackNode * bottom;
    int height;
} TFronta;

typedef struct {
    char znak;
    char * retezec;
} TPole;

Tmatice * vyrobMatrix(int radku, int sloupcu) {
    Tmatice * m = malloc(sizeof(Tmatice));
    if (m == NULL) {
        return NULL;
    }

    m->radku = radku;
    m->sloupcu = sloupcu;

    m->prvek = malloc(m->radku * sizeof(float));
    if (m->prvek == NULL) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < m->radku; i++) {
        m->prvek[i] = malloc(m->sloupcu * sizeof(float *));
        if (m->prvek[i] == NULL) {
            for (int j = i - 1; j >= 0; j--) {
                free(m->prvek[j]);
            }

            free(m->prvek);
            free(m);
            return NULL;
        }
    }

    return m;
}

TArray * nactiData(FILE * in) {
    int size = 2;
    int counter = 0;
    char temp[5];

    TArray * arr = malloc(sizeof(TArray));
    if (arr == NULL) {
        return NULL;
    }
    
    arr->size = size;
    arr->data = malloc(size * sizeof(TData));
    if (arr->data == NULL) {
        return NULL;
    }

    while (fscanf(in, "%20s %d %4s", arr->data[counter].nazev, &arr->data[counter].vyska, temp) == 3) {
        if (strcmp(temp, "ano") == 0) {
            arr->data[counter].active = true;
        } else {
            arr->data[counter].active = false;
        }
        
        counter++;
        if (counter == size) {
            TData *new_data = realloc(arr->data, 2 * size * sizeof(TData));

            if (new_data != NULL) {
                arr->data = new_data;
                size *= 2;
            } else {
                free(arr->data);
                free(arr);
                return NULL;
            }
        }

        arr->size = counter;
    }

    return arr;
}

Tmatice * nactiMatrix(FILE * in) {
    Tmatice * m = malloc(sizeof(Tmatice));
    if (m == NULL) {
        return NULL;
    }

    if (fscanf(in, "%d %d", &m->radku, &m->sloupcu) != 2) {
        return NULL;
    }

    m->prvek = malloc(m->radku * sizeof(float));
    if (m->prvek == NULL) {
        free(m);
        return NULL;
    }

    for (int i = 0; i < m->radku; i++) {
        m->prvek[i] = malloc(m->sloupcu * sizeof(float *));
        if (m->prvek[i] == NULL) {
            for (int j = i - 1; j >= 0; j--) {
                free(m->prvek[j]);
            }

            free(m->prvek);
            free(m);
            return NULL;
        }
    }

    float plc;
    int r = 0;
    int s = 0;
    
    while(fscanf(in, "%f", &plc) == 1 && r < m->radku && s < m->sloupcu) {
        m->prvek[r][s] = plc;
        s++;
        if (s >= m->sloupcu) {
            s = 0;
            r++;
        }
    }

    return m;
}

void printMatrix(Tmatice * m) {
    printf("Matice:\n");
    printf("[\n");
    for (int r = 0; r < m->radku; r++)
    {
        for (int s = 0; s < m->sloupcu; s++) {
            printf("%g ", m->prvek[r][s]);
        }
        printf("\n");
    }
    printf("]\n");
}

void numOfEven(Tmatice * m) {
    int isEven;

    for (int s = 0; s < m->sloupcu; s++) {
        isEven = 0;
        for (int r = 0; r < m->radku - 1; r++) {
            if (fmod(m->prvek[r][s], 2) == 0) {
                isEven++;
            }
        }
        m->prvek[m->radku - 1][s] = isEven;
    }
}

void sumOfElements(Tmatice * m) {
    float sumPerimeter = 0;
    float sumAll = 0;
    float sumRest = 0;
    
    for (int r = 0; r < m->radku; r++) {
        if (r == 0 || r == m->radku - 1) {
            for (int s = 0; s < m->sloupcu; s++) {
                sumPerimeter += m->prvek[r][s];
            }
        } else {
            sumPerimeter += m->prvek[r][0] + m->prvek[r][m->radku];
        }
    }

    for (int r = 0; r < m->radku; r++) {
        for (int s = 0; s < m->sloupcu; s++) {
            sumAll += m->prvek[r][s];
        }
    }

    sumRest = sumAll - sumPerimeter;

    printMatrix(m);
    printf("sum of elements on the perimeter: %g\n", sumPerimeter);
    printf("sum of the rest of the elemnts: %g\n", sumRest);
}

//ctvrtiny

//vertikalne/horizontalne symetricka

void transposedMatrix(Tmatice * m) {
    Tmatice * n = vyrobMatrix(m->sloupcu, m->radku);
    if (n == NULL) {
        return;
    }

    for (int r = 0; r < n->radku; r++) {
        for (int s = 0; s < n->sloupcu; s++) {
            n->prvek[r][s] = m->prvek[s][r];
        }
    }

    printf("matrix m:\n");
    printMatrix(m);
    printf("matrix n:\n");
    printMatrix(n);
}

//otoceni o 90, 180, 270 stupnu
void turnMatrix(Tmatice * m, int degrees) {
    int n = degrees / 90;
    int counter = 0;

    /*while (counter < degrees) {
        transposedMatrix(m);         approximately like this, transposedMatrix should return the new matrix for this to work, could also be probably done recursively then
    }*/ //also could be simplified for use of square matrix, no need for the second one, just rewriting the first one with some clever s = r+1 logic and rest variable
}

void ulohyMatice(void) {
    char filename[128];

    printf("insert filename: ");
    scanf("%127s", filename);

    FILE * in = fopen(filename, "r");
    if (in == NULL) {
        printf("wrong file");
        return;
    }

    Tmatice * m = nactiMatrix(in);
    if (m == NULL) {
        printf("invalid matrix");
        return;
    }

    printMatrix(m);

    //numOfEven(m);
    //printMatrix(m);

    sumOfElements(m);
    transposedMatrix(m);
}

char * nactiRetezec(int n) {
    char character;
    int counter = 0;
    char * characters = malloc((n + 1) * sizeof(char));
    if (characters == NULL) {
        return NULL;
    }

    printf("insert characters: ");

    while ((character = getchar()) != EOF && counter < n) {
        characters[counter] = character;
        counter++;
        if (character == '\n' && counter > 1) break;
    }

    characters[counter] = '\0';

    return characters;
}

int numOfNum(char * characters) {
    int counter = 0;
    int numNum = 0;

    while (characters[counter] != '\0') {
        if (characters[counter] >= '0' && characters[counter] <= '9') {
            numNum++;
        }
        counter++;
    }

    return numNum;
}

void posOfNum(char * characters, int * start, int * end) {
    int counter = 0;
    int startPos = -1;
    int endPos = -1;

    while (characters[counter] != '\0') {
        if (characters[counter] >= '0' && characters[counter] <= '9' && startPos == -1) {
            startPos = counter;
        }

        if (characters[counter] >= '0' && characters[counter] <= '9') {
            endPos = counter;
        }

        counter++;
    }

    *start = startPos;
    *end = endPos;
}

void replaceXY(char * characters, char x, char y) {
    int counter = 0;

    while (characters[counter] != '\0') {
        if (characters[counter] == x) {
            characters[counter] = y;
        }

        counter++;
    }
}

void hideNum(char * characters) {
    int counter = 0;

    while (characters[counter] != '\0') {
        if (characters[counter] >= '0' && characters[counter] <= '9') {
            characters[counter] = '#';
        }

        counter++;
    }
}

int numOfWords(char * characters) {
    int counter = 0;
    int wordIndicator = 0;
    int wordIndicatorLast = 0;
    int wordCounter = 0;
    
    while (characters[counter] != '\0') {
        wordIndicatorLast = wordIndicator;

        if (characters[counter] != ' ' && characters[counter] != '\n') {
            wordIndicator = 1;
        } else {
            wordIndicator = 0;
        }

        if (wordIndicator == 1 && wordIndicatorLast == 0) {
            wordCounter++;
        }

        counter++;
    }

    return wordCounter;
}

void switchCapitals(char * characters) {
    int counter = 0;
    
    while (characters[counter] != '\0') {
        if (characters[counter] >= 'a' && characters[counter] <= 'z') {
            characters[counter] -= 'a' - 'A';
        } else if (characters[counter] >= 'A' && characters[counter] <= 'Z') {
            characters[counter] += 'a' - 'A';
        }

        counter++;
    }
}

void reverseBasic(char * characters) {
    int n = 0;
    int counter = 0;
    char rest;
    
    while (characters[n] != '\0') {
        n++;
    }

    n = n - 1;

    while (counter < n / 2) {
        rest = characters[counter];
        characters[counter] = characters[n - counter];
        characters[n - counter] = rest;
        counter++;
    }
}

void reverseRecursively(char * characters, int counter, int n) {
    if (counter >= n / 2) {
        return;
    }

    char rest;
    rest = characters[counter];
    characters[counter] = characters[n - counter];
    characters[n - counter] = rest;

    reverseRecursively(characters, counter + 1, n);
}

void reverseComplex(char * characters) {
    int n = 0;
    int counter = 0;
    char rest;
    
    while (characters[n] != '\0') {
        n++;
    }

    n = n - 1;

    reverseRecursively(characters, 0, n);
}

//16bit nums

//2to36

float loadExponential() {
    int num;
    int exp;

    printf("insert number: ");
    if (scanf("%dE%d", &num, &exp) != 2) {
        return -1;
    }

    return num * powf(10, exp);
}

void ulohyRetezec() {
    char * characters = nactiRetezec(16);
    if (characters == NULL) {
        return;
    }

    printf("%s\n", characters);
    printf("there are %d numbers in the array\n", numOfNum(characters));

    int start;
    int end;

    posOfNum(characters, &start, &end);

    printf("first number was at %d and last was on %d\n", start, end);
    
    replaceXY(characters, 'a', 'b');
    printf("replaced string now looks like this: %s\n", characters);

    hideNum(characters);
    printf("string with hidden number now looks like this: %s\n", characters);

    printf("there are %d words in the string\n\n", numOfWords(characters));

    switchCapitals(characters);
    printf("string with switched capitality now looks like this: %s\n", characters);

    reverseBasic(characters);
    printf("string reversed with a while now looks like this: %s\n", characters);

    reverseComplex(characters);
    printf("string reversed recursively now looks like this: %s\n", characters);

    printf("the resulting number is: %.3f\n\n", loadExponential("124E-1"));
}

void replaceHashtag(FILE * in, FILE * out, char * string) {
    char temp;
    
    while (fscanf(in, "%c", &temp) == 1) {
        if (temp == '#') {
            fprintf(out, "%s", string);
        } else {
            fprintf(out, "%c", temp);
        }
    }
}

void ulohyRetezecSoubor() {
    char filenameIn[128];

    printf("insert filename of in file: ");
    scanf("%127s", filenameIn);

    FILE * in = fopen(filenameIn, "r");
    if (in == NULL) {
        return;
    }

    char filenameOut[128];

    printf("insert filename of out file: ");
    scanf("%127s", filenameOut);

    while (strcmp(filenameOut, filenameIn) == 0) {
        printf("that file is already in use, choose a different one");
        printf("insert filename of out file: ");
        scanf("%127s", filenameOut);
    }

    FILE * out = fopen(filenameOut, "w");
    if (out == NULL) {
        return;
    }

    char * string = nactiRetezec(16);
    if (string == NULL) {
        return;
    }

    string[0] = ' ';
    if (string[strlen(string) - 1] == '\n') {
        string[strlen(string) - 1] = '\0';
    }

    replaceHashtag(in, out, string);

    fclose(in);
    fclose(out);
}

bool isInString(char * alphabet, char character) {
    int counter = 0;

    while (alphabet[counter] != '\0') {
        if (alphabet[counter] == character) {
            return true;
        }

        counter++;
    }

    return false;
}

int jeVetouJazyka(char * alphabet) {
    char sentence[1024];
    int counter = 0;

    printf("enter a sentence of your language: ");
    scanf("%1023s", sentence);

    while (sentence[counter] != '\0') {
        if (isInString(alphabet, sentence[counter])) {
            counter++;
        } else {
            return counter + 1;
        }
    }
    
    return 0;
}

bool zavorkyJedny() {
    char string[1024];
    int counter = 0;

    int opening = 0;
    int closing = 0;
    
    printf("insert a string containing [ and ]: ");
    scanf("%1023s", string);

    while (string[counter] != '\0') {
        if (string[counter] == '[') {
            opening++;
        }

        if (string[counter] == ']') {
            if (closing < opening) {
                closing++;
            } else {
                return false;
            }
        }

        counter++;
    }

    return true;
}

/*
1 = 3
2 = 5
3 = 7
4 = 9
a_n = 3 + 2 * (n - 1)
*/

char * vetyJazyka(int n) {
    int size = 3 + 2 * (n - 1);
    int counter = 1;
    
    char * arr = malloc(size * sizeof(char));
    if (arr == NULL) {
        return NULL;
    }

    arr[0] = 'a';

    while (counter < size) {
        arr[counter]  = 'b';
        arr[counter + 1] = 'c';

        counter += 2;
    }

    return arr;
}

bool grammarRegular(char * rule) {
    if (strlen(rule) < 4) {
        return false;
    }
    
    if (!(rule[0] >= 'A' && rule[0] <= 'Z')) {
        return false;
    }

    if (!(rule[1] == '-' && rule[2] == '>')) {
        return false;
    }

    if (!(rule[3] >= 'a' && rule[3] <= 'z')) {
        return false;
    }

    if (strlen(rule) == 5 && !(rule[4] >= 'A' && rule[4] <= 'Z')) {
        return false;
    }

    if (strlen(rule) > 5) {
        return false;
    }

    return true;
}

bool grammarSansContext(char * rule) {
    int counter = 3;
    
    if (strlen(rule) < 4) {
        return false;
    }
    
    if (!(rule[0] >= 'A' && rule[0] <= 'Z')) {
        return false;
    }

    if (!(rule[1] == '-' && rule[2] == '>')) {
        return false;
    }

    while (rule[counter] != '\0') {
        if (!((rule[counter] >= 'A' && rule[counter] <= 'Z') || (rule[counter] >= 'a' && rule[counter] <= 'z'))) {
            return false;
        }

        counter++;
    }

    return true;
}

void ulohyGramatika() {
    char alphabet[32];
    char rule[1024];

    printf("enter all characters of the alphabet in one continuous sequence: ");
    scanf("%31s", alphabet);

    int temp = jeVetouJazyka(alphabet);

    if (temp == 0) {
        printf("veta je vetou jazyka v jejim plnem zneni\n");
    } else {
        printf("veta je vetou jazyka do pozice %d\n", temp);
    }

    printf("zadany retezec ma zavorky %s\n", zavorkyJedny() ? "spravne" : "spatne");

    printf("5 vet jazyka vypada takto: %s\n", vetyJazyka(5));

    printf("enter a grammar rule: ");
    scanf("%1023s", rule);

    printf("the inputted rule %s a rule of regular grammar\n", grammarRegular(rule) ? "is" : "is not");
    printf("the inputted rule %s a rule of sans context grammar\n", grammarSansContext(rule) ? "is" : "is not");
}

StackNode * newNode(char val) {
    StackNode * node = malloc(sizeof(StackNode));
    if (node == NULL) {
        return NULL;
    }

    node->next = NULL;
    node->value = val;

    return node;
}

void push(StackNode ** stack, char val) {
    StackNode * node = newNode(val);
    if (node == NULL) {
        return;
    }

    node->next = *stack;
    *stack = node;
}

void pop(StackNode ** stack) {
    if (*stack == NULL) {
        return;
    }

    StackNode * temp = *stack;
    *stack = temp->next;
    free(temp);
}

void printStack(StackNode * stack) {
    if (stack == NULL) {
        return;
    }

    printf("%c\n", stack->value);
    printStack(stack->next);
}

bool testLIFO() {
    char filename[128];
    char temp;
    char top;

    printf("enter filename: ");
    scanf("%127s", filename);

    FILE * in = fopen(filename, "r");
    if (in == NULL) {
        return false;
    }

    StackNode * stack = NULL;

    while ((temp = fgetc(in)) != EOF) {
        if (temp == '(' || temp == '[' || temp == '{') {
            push(&stack, temp);
        }

        top = stack->value;

        if (temp == ')' || temp == ']' || temp == '}') {
            switch (temp)
            {
            case ')':
                if (top == '(') {
                    pop(&stack);
                } else {
                    return false;
                }

                break;

            case ']':
                if (top == '[') {
                    pop(&stack);
                } else {
                    return false;
                }
                break;
            
            case '}':
                if (top == '{') {
                    pop(&stack);
                } else {
                    return false;
                }
                break;

            default:
                break;
            }
        }
    }

    return true;
}

TFronta * vytvorFrontu() {
    TFronta * f = malloc(sizeof(TFronta));
    if (f == NULL) {
        return NULL;
    }

    f->top = NULL;
    f->bottom = NULL;
    f->height = 0;

    return f;
}

void vloz(TFronta * f, char val) {
    StackNode * node = newNode(val);
    if (node == NULL) {
        return;
    }

    if (f->top == NULL) {
        f->top = node;
        f->bottom = node;
        return;
    }

    f->bottom->next = node;
    f->bottom = node;
    f->height = f->height + 1;
}

void vyber(TFronta * f) {
    StackNode * node = f->top;
    f->top = f->top->next;

    f->height = f->height - 1;

    if (f->top == NULL) {
        f->bottom = NULL;
    }

    free(node);
}

void printQueue(TFronta * f) {
    StackNode * node = f->top;

    while (node != NULL) {
        printf("%c\n", node->value);
        node = node->next;
    }
}

void testFIFO() {
    TFronta * f = vytvorFrontu();
    char temp;
    
    printf("enter a string: ");
    while ((temp = fgetc(stdin)) != '\n') {
        vloz(f, temp);
        if (f->height >= 5) {
            vyber(f);
        }
    }

    printf("\n");

    printQueue(f);
}

void ulohyFLIFO() {
    StackNode * stack = newNode('0');

    pop(&stack);
    push(&stack, '1');
    push(&stack, '0');
    push(&stack, '1');
    pop(&stack);
    push(&stack, '0');
    push(&stack, '1');

    printStack(stack);
    
    printf("the string in selected file is %s\n", testLIFO() ? "correct" : "incorrect");

    TFronta * f = vytvorFrontu();
    vloz(f, '0');
    vloz(f, '1');
    vloz(f, '0');
    vloz(f, '0');
    vloz(f, '1');
    vyber(f);

    printQueue(f);

    char temp;
    scanf("%c", &temp);

    testFIFO();
}

void printDB(TArray * arr) {
    int counter = 0;
    
    printf("\nnazev                vyska      active?\n");
    while (counter < arr->size) {
        printf("%20s %10d %7s\n", arr->data[counter].nazev, arr->data[counter].vyska, arr->data[counter].active ? "true" : "false");

        counter++;
    }
}

int averageHeight(TArray * arr, bool activity) {
    int counter = 0;
    int avg = 0;
    int counterAvg = 0;

    while (counter < arr->size) {
        if (arr->data[counter].active == activity) {
            avg += arr->data[counter].vyska;
            counterAvg++;
        }
        
        counter++;
    }

    return avg / counterAvg;
}

void higherThan(TArray * arr, int height) {
    int counter = 0;

    while (counter < arr->size) {
        if (arr->data[counter].vyska > height) {
            printf("%20s %10d %7s\n", arr->data[counter].nazev, arr->data[counter].vyska, arr->data[counter].active ? "true" : "false");
        }

        counter++;
    }
}

void ulohyDB() {
    char filename[128];

    printf("insert filename: ");
    scanf("%127s", filename);

    FILE * in = fopen(filename, "r");
    if (in == NULL) {
        printf("wrong file");
        return;
    }

    TArray * arr = nactiData(in);
    if (arr == NULL) {
        return;
    }

    printDB(arr);

    printf("\naverage height in the dataset of active volcanoes is %d\n", averageHeight(arr, true));
    printf("average height in the dataset of inactive volcanoes is %d\n", averageHeight(arr, false));

    printf("\nvolcanoes taller than 2750 m are:\n");
    higherThan(arr, 2750);
}

void mergingFiles(FILE * inOne, FILE * inTwo, FILE * out) {
    char temp;
    int endOne = 0, endTwo = 0;
    
    while (!endOne || !endTwo) {
        if (!endOne) {
            while ((temp = fgetc(inOne)) != EOF) {
                fputc(temp, out);
                if (temp == '\n') break;
            }
            if (temp == EOF) endOne = 1;
        }

        if (!endTwo) {
            while ((temp = fgetc(inTwo)) != EOF) {
                fputc(temp, out);
                if (temp == '\n') break;
            }
            if (temp == EOF) endTwo = 1;
        }
    }
}

void numOfWordsFile(FILE * in, FILE * out) {
    int temp;
    int isWord = 0;
    int wasWord = 0;
    int num = 0;

    while ((temp = fgetc(in)) != EOF) {
        if (temp == ' ') {
            wasWord = isWord;
            isWord = 0;
        } else {
            wasWord = isWord;
            isWord = 1;

            if (wasWord == 0 && isWord == 1) {
                num++;
            }
        }

        if (temp == '\n') {
            fprintf(out, "%d\n", num);
            isWord = 0;
            wasWord = 0;
            num = 0;
        }
    }

    if (num > 0 || isWord) {
        fprintf(out, "%d\n", num);
    }
}

int findInSorted(FILE * in) {
    int counter = 0;
    int temp;

    int searchedFor;
    printf("enter the number you want to search for: ");
    scanf("%d", &searchedFor);

    while (fscanf(in, "%d", &temp) == 1) {
        if (temp > searchedFor) {
            break;
        }

        if (temp == searchedFor) {
            return counter;
        }

        counter++;
    }

    while (fscanf(in, "%d", &temp) == 1) {
        if (temp == searchedFor) {
            return counter;
        }

        counter++;
    }

    return -1;
}

void ulohyFiles() {
    printf("Bonjour madames!\n");

    FILE * inOne = fopen("file1.txt", "r");
    FILE * inTwo = fopen("file2.txt", "r");
    if (inOne == NULL || inTwo == NULL) {
        return;
    }

    FILE * out = fopen("file3.txt", "w");
    if (out == NULL) {
        return;
    }

    FILE * in = fopen("gpt.txt", "r");
    if (in == NULL) {
        return;
    }

    FILE * outTwo = fopen("gpt-out.txt", "w");
    if (outTwo == NULL) {
        return;
    }

    mergingFiles(inOne, inTwo, out);

    rewind(inOne);
    numOfWordsFile(inOne, outTwo);

    FILE * sort = fopen("sorted.txt", "r");
    if (sort == NULL) {
        return;
    }

    printf("the searched for number was found on index %d", findInSorted(sort));

    fclose(inOne);
    fclose(inTwo);
    fclose(out);
}

char * onlyBin(char * in) {
    int counter = 0;
    int counterOut = 0;
    int size = 2;
    char * out = malloc(size * sizeof(char));
    if (out == NULL) {
        return NULL;
    }
    
    while (in[counter] != '\0') {
        if (in[counter] == '0' || in[counter] == '1') {
            out[counterOut] = in[counter];
            counterOut++;
        }
        
        counter++;
        if (counter == size) {
            size *= 2;
            char * temp = realloc(out, size * sizeof(char));
            if (temp == NULL) {
                return NULL;
            }
            out = temp;
        }
    }

    return out;
}

int witchOS(char * string) {
    int highest = strlen(string) - 1;
    long long num = 0;
    int temp;
    int counter = 0;

    while (highest >= 0) {
        if (string[counter] >= '0' && string[counter] <= '9') {
            temp = string[counter] - '0';
        } else {
            temp = string[counter] - 'A' + 10;
        }
        
        num += temp * pow(16, highest);
        counter++;
        highest--;
    }

    if (num < pow(2, 8)) {
        return 8;
    }

    if (num < pow(2, 16)) {
        return 16;
    }

    if (num < pow(2, 32)) {
        return 32;
    }

    if (num < pow(2, 64)) {
        return 64;
    }

    return 0;
}

void ulohyStringBin() {
    char in[1024];
    printf("enter some binary adress (can b-dirty): ");
    scanf("%1023s", in);

    printf("the non-dirty binary adress is now this: %s\n", onlyBin(in));

    printf("enter some hexa adress: ");
    scanf("%1023s", in);

    printf("this adress can be used in %d-bit OS\n", witchOS(in));
}

bool checkMAC(char * in) {
    if (strlen(in) != 17) {
        printf("all the way out here\n");
        return false;
    }

    int counter = 0;

    while (counter < 17) {
        if (((counter + 1) % 3 == 1) || ((counter + 1) % 3 == 2)) {
            if (!((in[counter] >= '0' && in[counter] <= '9') || (in[counter] >= 'A' && in[counter] <= 'F'))) {
                printf("out here\n");
                return false;
            }
        } else {
            if (in[counter] != '-') {
                printf("out there\n");
                return false;
            }
        }

        counter++;
    }

    return true;
}

bool checkIPv4(char * in) {
    int counter = 0;
    int num;
    int numOnums = 0;

    while (in[counter] != '\0') {
        num = 0;

        while (in[counter] >= '0' && in[counter] <= '9') {
            num = num * 10 + (in[counter] - '0');
            counter++;
        }

        if (num > 255 || num < 0) {
            return false;
        }

        numOnums++;

        if ((num == 0 && in[counter - strlen(in + counter)] == '0') || (in[counter - 1] == '0' && counter - strlen(in + counter) > 1)) {
            return false;
        }

        if (in[counter] == '.') {
            counter++;
        }
    }

    if (numOnums != 4) {
        return false;
    }

    return true;
}

bool checkIPv6(char * in) {
    int counter = 0;
    char sub[8];
    int counterSub = 0;
    int counterDDD = 0;
    int groups = 1;

    while (in[counter] != '\0') {
        if ((in[counter] >= '0' && in[counter] <= '9') || (in[counter] >= 'a' && in[counter] <= 'f') || in[counter] >= 'A' && in[counter] <= 'F') {
            if (counterSub >= 4) {
                return false;
            }

            sub[counterSub] = in[counter];
            counterSub++;
        } else {
            if (in[counter] == ':' || in[counter] == '.') {
                if (in[counter - 1] == ':' && counter > 0) {
                    if (in[counter - 2] == ':' && counter > 1) {
                        return false;
                    }

                    if (counterDDD > 0) {
                        return false;
                    }

                    counterDDD++;
                    groups++;
                }
                counterSub = 0;
                groups++;
            } else {
                return false;
            }
        }

        counter++;
    }

    if (groups > 8) {
        return false;
    }

    return true;
}

bool checkMail(char * in) {
    printf("didn't implement, slowly going insane hahahahahahahah\n");
}

void ulohyCheck() {
    char in[128];
    int choice;

    printf("enter some address: ");
    scanf("%127s", in);

    if (in[strlen(in) - 1] == '\n') {
        in[strlen(in) - 1] = '\0';
    }

    printf("check as:\n");
    printf("0 .. MAC\n");
    printf("1 .. IPv4\n");
    printf("2 .. IPv6\n");
    printf("3 .. mail\n");
    printf("your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 0:
            printf("it %s the correct form of the address", checkMAC(in) ? "is" : "is not");
            break;
        case 1:
            printf("it %s the correct form of the address", checkIPv4(in) ? "is" : "is not");
            break;
        case 2:
            printf("it %s the correct form of the address", checkIPv6(in) ? "is" : "is not");
            break;
        case 3:
            printf("it %s the correct form of the address", checkMail(in) ? "is" : "is not");
            break;
    
        default:
            break;
    }
}

int * lastN(int n) {
    char filename[128];
    int temp;
    int counter;
    int swap;

    printf("enter filename: ");
    scanf("%127s", filename);

    FILE * in = fopen(filename, "r");
    if (in == NULL) {
        return NULL;
    }

    int * arr = malloc(n * sizeof(int));
    if (arr == NULL) {
        return NULL;
    }

    while (fscanf(in, "%d", &temp) == 1) {
        counter = n - 1;

        swap = arr[counter + 1];
        while (counter > 0) {
            arr[counter] = arr[counter - 1];
            counter--;
        }

        arr[0] = temp;
    }

    return arr;
}

int * histogram() {
    char temp;
    int * hist = malloc(26 * sizeof(int));
    if (hist == NULL) {
        return NULL;
    }

    for (int i = 0; i < 26; i++) {
        hist[i] = 0;
    }

    scanf("%c", &temp);
    printf("insert a string: ");
    while ((temp = fgetc(stdin)) != '\0' && temp != '\n') {
        if (temp >= 'a' && temp <= 'z') {
            hist[temp - 'a'] += 1;
        }
    }

    return hist;
}

void choices(TPole * arr, size_t size) {
    char temp;
    char garbage[128];
    int counter;
    
    while (true) {
        for (int i = 0; i < size; i++) {
            printf("%c = %s\n", arr[i].znak, arr[i].retezec);
        }

        printf("your choice: ");
        scanf("%s", garbage);
        temp = garbage[0];

        if (temp == 'K') {
            return;
        } else {
            counter = 0;

            while (counter < size) {
                if (temp == arr[counter].znak) {
                    printf("%s\n", arr[counter].retezec);
                    break;
                }

                if (arr[counter].znak == 'X') {
                    printf("%s\n", arr[counter].retezec);
                    break;
                }

                counter++;
            }
        }
    }
}

void ulohyComplex() {
    int * arr = lastN(5);
    for (int i = 0; i < 5; i++) {
        printf("%d\n", arr[i]);
    }

    int sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += arr[i];
    }

    printf("sum of 5 last numbers in the array is: %d\n", sum);

    for (int i = 4; i >= 0; i--) {
        printf("%d\n", arr[i]);
    }

    int * hist = histogram();
    for (int i = 0; i < 26; i++) {
        printf("%c : %d\n", 'a' + i, hist[i]);
    }

    TPole pole[] = {
        {'A', "Choice A"},
        {'B', "Choice B"},
        {'K', "Choice K - End"},
        {'X', "Choice X - Unknown"},
    };

    size_t size = sizeof(pole) / sizeof(pole[0]);

    choices(pole, size);
}

uint16_t toBinary(char * string) {
    int counter = 0;
    uint16_t num = 0;

    while (string[counter] != '\0' && (string[counter] == '0' || string[counter] == '1')) {
        num = num << 1;
        num += string[counter] - '0';
        counter++;
    }

    return num;
}

void printBinary(uint16_t num) {
    for (int i = 15; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0');
    }

    putchar('\n');
}

uint16_t bitProduct(uint16_t numOne, uint16_t numTwo) {
    return numOne & numTwo;
}

uint16_t bitSum(uint16_t numOne, uint16_t numTwo) {
    return numOne | numTwo;
}

uint16_t bitXOR(uint16_t numOne, uint16_t numTwo) {
    return numOne ^ numTwo;
}

uint16_t bitShift(uint16_t num, int n) {
    if (n > 0) {
        return num >> n;
    } else {
        return num << (-n);
    }
}

uint16_t bitRotation(uint16_t num, int n) {
    if (n > 0) {
        return num >> n | num << (16 - n);
    } else {
        return num << (-n) | num >> (16 + n);
    }
}

void ulohyBinary() {
    char binCharOne[] = "1001011001101110";
    char binCharTwo[] = "0000000000001001";

    uint16_t numOne = toBinary(binCharOne);
    uint16_t numTwo = toBinary(binCharTwo);

    printf("binary number 1: %d\n", numOne);
    printf("binary number 2: %d\n", numTwo);

    printBinary(numOne);
    printBinary(numTwo);

    printf("bit operations\n");
    printf("bitwise AND: ");
    printBinary(bitProduct(numOne, numTwo));
    printf("bitwise OR: ");
    printBinary(bitSum(numOne, numTwo));
    printf("bitwise XOR: ");
    printBinary(bitXOR(numOne, numTwo));
    printf("bit shift left: ");
    printBinary(bitShift(numTwo, -3));
    printf("bit shift right: ");
    printBinary(bitShift(numTwo, 3));
    printf("bit rotation left: ");
    printBinary(bitRotation(numOne, -1));
    printf("bit rotation right: ");
    printBinary(bitRotation(numTwo, 1));
    printf("arithmetic sum: ");
    printBinary(bitXOR(numOne, numTwo));
}

void printArray(int * arr, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");
}

void insertionSort(int * arr, int size) {
    int vkladany;
    int i;
    
    for (int d = 1; d < size; d++) {
        vkladany = arr[d];
        i = d;

        while (i > 0 && arr[i - 1] > vkladany) {
            arr[i] = arr[i - 1];
            i--;
        }

        arr[i] = vkladany;
    }
}

void insertionSortStopper(int * arr, int size) {
    int vkladany;
    int i;
    
    for (int d = 2; d < size; d++) {
        vkladany = arr[d];
        i = d;
        arr[0] = vkladany;

        while (arr[i - 1] > vkladany) {
            arr[i] = arr[i - 1];
            i--;
        }

        arr[i] = vkladany;
    }
}

void selectionSort(int * arr, int size) {
    int minIndex;
    int temp;
    int i;
    
    for (int d = 0; d < size - 1; d++) {
        minIndex = d;

        for (i = d + 1; i < size; i++) {
            if (arr[minIndex] > arr[i]) {
                minIndex = i;
            }
        }

        temp = arr[minIndex];
        arr[minIndex] = arr[d];
        arr[d] = temp;
    }
}

void bubbleSort(int * arr, int size) {
    int temp;
    
    for (int d = 0; d < size - 1; d++) {
        for (int i = size - 1; i > d; i--) {
            if (arr[i - 1] > arr[i]) {
                temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
            }
        }
    }
}

void bubbleSortStopper(int * arr, int size) {
    int temp;
    int rem = 0;
    
    for (int d = rem; d < size - 1; d++) {
        for (int i = size - 1; i > d; i--) {
            if (arr[i - 1] > arr[i]) {
                temp = arr[i];
                arr[i] = arr[i - 1];
                arr[i - 1] = temp;
                rem = i;
            }
        }
    }
}

void merge(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    if (!L || !R) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Copy data to temporary arrays L[] and R[]
    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    // Merge the temp arrays back into arr[]
    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    // Copy the remaining elements
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int mid = (left + right) / 2;

        mergeSort(arr, left, mid);      // Sort first half
        mergeSort(arr, mid + 1, right); // Sort second half
        merge(arr, left, mid, right);   // Merge sorted halves
    }
}

void ulohySorting() {
    int arr[] = {42, 17, 8, 99, 23, 4, 55, 1, 76, 31, 9};
    int arr_stopper[] = {0, 42, 17, 8, 99, 23, 4, 55, 1, 76, 31, 9};
    int size = sizeof(arr) / sizeof(arr[0]);

    /*insertionSort(arr, size);
    printArray(arr, size);

    insertionSortStopper(arr_stopper, size + 1);
    printArray(arr_stopper, size + 1);*/

    /*selectionSort(arr, size);
    printArray(arr, size);*/

    /*bubbleSort(arr, size);
    printArray(arr, size);*/

    /*bubbleSortStopper(arr, size);
    printArray(arr, size);*/

    mergeSort(arr, 0, size - 1);
    printArray(arr, size);
}

int main(void) {
    //ulohyMatice();
    //ulohyRetezec();
    //ulohyRetezecSoubor();
    //ulohyGramatika();
    //ulohyFLIFO();
    //ulohyDB();
    //ulohyFiles();
    //ulohyStringBin();
    //ulohyCheck();
    //ulohyComplex();
    //ulohyBinary();
    //ulohySorting();

    return 0;
}