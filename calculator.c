/*
    Data Structures and Algorithms - Semester Project

    ### Arithmetic Operations with Stack ###

    @author
    Student: Osman Araz
    Student NO: 16011020
    Delivery Date: 10.06.2018
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define stackSize 500
#define variableSize 10

// a structure to keep Stack and its elements
typedef struct
{
    int top;
    char st[stackSize][variableSize];
} Stack;

// array of variables, which represents English lowercase characters
int variables[26] = {0};

void printBanner();
void clearScreen();
void processCommand(char *);
void getCommand();
void getCommandsFromFile(char *);
void printVariables();
int main()
{
    int choice; // a variable which keeps the choice of the user

    do
    {
        printBanner();  // printing the banner of the program

        // printing the menu of the program:
        printf(" 1. Add Equation from the Terminal\n 2. Add Equations from the File\n");
        printf(" 3. Print All Variables\n 4. Exit\n\n");

        // getting choice from the user:
        printf(" Your choice: ");
        scanf("%d", &choice);

        printBanner();

        if (choice == 1)
            getCommand();
        else if (choice == 2)
        {
            printf("Enter the file name: ");
            char fileName[20];
            scanf("%s", fileName);
            printBanner();
            getCommandsFromFile(fileName);
        }
        else if (choice == 3)
            printVariables();

        if (choice > 0 && choice < 4)
        {
            printf("\n\n To back to the main menu, press any key on your keyboard: ");
            getchar();
            getchar();
        }
    }
    while (choice > 0 && choice < 4);
    
    return 0;
}

/*
    A function which gets equation from the user on the terminal screen.
*/
void getCommand()
{
    char line[500];
    getchar();
    printf(" Enter the equation: ");
    fgets(line, 500, stdin);    // getting the command
    strtok(line, "\n");         // erasing the new line character from the input command
    printf("\n");
    processCommand(line);       // processing the command
}

/*
    A function which gets equations from the given file.
    @param *fileName: the name of the input file
*/
void crashFile();
void getCommandsFromFile(char *fileName)
{
    FILE *fi = fopen(fileName, "r");    // opening the file

    if (!fi)    // if file could not be opened, then the program ends
        crashFile();

    char line[500];

    while (fgets(line, 500, fi))    // while there is a line in the input file
    {
        strtok(line, ";\n");        // erasing redundant characters from the line 
        processCommand(line);       // processing the line
    }

    fclose(fi); // closing the file
}

/*
    A function which generates postfix form of a given command and then solve the postfix form.
    @param *command: the given equation
*/
char *getPostfix(char *);
int solvePostfix(char *);
void processCommand(char *command)
{
    printf("-> Getting the Postfix Expression for %s:\n\n", command);
    char *postfix = getPostfix(command);    // getting the postfix expression
    printf("\n-> Solving the Postfix Expression%s :\n\n", postfix);
    int var = *command - 'a';   // the variable will be the first character of the equation
    variables[var] = solvePostfix(postfix);     // solving the postfix expression
    printf("\n-> Update #%c: %d\n\n", *command, variables[var]); // printing the updated value of the variable
    printf("#################################################\n\n");
}

/*
    A function which generates postfix form of a given equation.
    @param *command: the given equation
    @return: generated postfix form
*/
void initializeStack(Stack *);
void push(Stack *, char *);
char *pop(Stack *);
char *top(Stack *);
bool isStackEmpty(Stack *);
bool isNumeric(char *str);
int getPrecedence(char *op);
void printStack(Stack *);
void crashMemory();
char *getPostfix(char *command)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack)); // creating the stack

    if (!stack) // if stack could not be created, then the program ends
        crashMemory();

    initializeStack(stack);

    char *postfix = (char *)malloc(500); // postfix string

    if (!postfix)
        crashMemory();

    memset(postfix, 0, 500); // cleaning data from the postfix string

    char var[variableSize]; // a variable to keep the words of the equation
    char *pp = command+4;   // a pointer, which points after the equality ('=') character of the equation

    while (*pp) // while the equation is not end
    {
        memset(var, 0, variableSize); // cleaning data from the string
        sscanf(pp, "%s", var);        // getting data from the equation to the string

        if (strcmp(var, "(") == 0)  // if the word is an open parenthesis, then it will be pushed onto the stack
            push(stack, var);
        else if (strcmp(var, ")") == 0)
        {
            // if the word is a close parenthesis, then all elements through to the open parenthesis will
            // be popped from the stack and added to the postfix expresion 
            while (strcmp(top(stack), "(") != 0)
                sprintf(postfix, "%s %s", postfix, pop(stack));

            pop(stack);
        }
        // if the word is numerical or a alphabetic character, then it will be 
        // added to the postfix expression
        else if (isNumeric(var) || isalpha(*var))
            sprintf(postfix, "%s %s", postfix, var);
        else // if the word is an arithmetic operator
        {
            // while the stack is not empty and the precedence of the operator is less than
            // the precedence of the top of the stack, than the top of the stack will be popped
            // and added to the postfix expression
            while (!isStackEmpty(stack) && getPrecedence(top(stack)) > getPrecedence(var))
                sprintf(postfix, "%s %s", postfix, pop(stack));

            push(stack, var); // pushing the operator onto the stack
        }

        printStack(stack);  // printing the stack elements
        printf("~ Postfix:%s\n", postfix);  // printing the postfix expression

        pp += strlen(var)+1; // traversing the next word of the equation
    }

    // the remained elements of the stack will be popped and added to the postfix expression
    while (top(stack))
    {
        sprintf(postfix, "%s %s", postfix, pop(stack));
        printStack(stack);
        printf("~ Postfix:%s\n", postfix);
    }

    free(stack);    // deallocating the stack

    return postfix; // returning the generated posfix expression
}

/*
    A function which solves the mathematical result of the postfix expression.
    @param *postfix: the postfix expression
    @return: the calculated value
*/
int calculate(int, int, char);
int stringToInteger(char *);
char *integerToString(int);
int solvePostfix(char *postfix)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack)); // creating the stack

    if (!stack) // if stack could not be created, then the program ends
        crashMemory();

    initializeStack(stack);

    char var[variableSize]; // a variable to keep the parts of the postfix expression
    char *pp = postfix;     // a pointer which points the postfix expression

    while (*pp)  // while the postfix expression is not end
    {
        memset(var, 0, variableSize);  // cleaning data from the string
        sscanf(pp, "%s", var);         // getting data from the postfix expresion to the string

        if (isNumeric(var)) // if the word is numerical, then it will be pushed onto the stack
            push(stack, var);
        else if (isalpha(*var)) // if the word is an alpahbetic character, then
        // the value of the character will be pushhed onto the stack
            push(stack, integerToString(variables[*var-'a']));
        else    // if the word is an arithmetic operator
        {
            int num1 = stringToInteger(pop(stack)); // popping the first number
            int num2 = stringToInteger(pop(stack)); // popping the second number

            // pushing the calculated value of the popped numbers onto the stack:
            push(stack, integerToString(calculate(num2, num1, *var)));
        }

        printStack(stack);     // printing the stack elements
        pp += strlen(var)+1;   // traversing the next word of the postfix expression
    }

    int ans = stringToInteger(pop(stack));

    free(stack);  // deallocating the stack

    return ans; // returning the last element of the stack
}

/*
    A function which converts a numerical string to its integer value.
    @param *str: the string which will be converted to the integer
    @return: the converted integer value
*/
int stringToInteger(char *str)
{
    int i, number = 0, firstDigit = 0, digit = 1;

    if (*str == '-')    // if the number is negative
        firstDigit = 1;

    for (i = strlen(str)-1; i >= firstDigit; --i)
    {
        number += (str[i]-'0')*digit;
        digit *= 10;
    }

    return (firstDigit == 0) ? number : -number;
}

/*
    A function which converts an integer to its string value.
    @param number: the integer which will be converted to string
    @return: the converted string value
*/
char *integerToString(int number)
{
    char *str = (char *)malloc(variableSize);
    sprintf(str, "%d", number);
    return str;
}

/*
    A function which calculates the result of two integers according to the given operator.
    @param x: the first number
    @param y: the second number
    @param op: the operator
    return: the result of the calculation
*/
int calculate(int x, int y, char op)
{
    if (op == '+')
        return x+y;
    else if (op == '-')
        return x-y;
    else if (op == '*')
        return x*y;

    return x/y;
}

/*
    A function which initializes a given stack.
    @param *stack: the pointer of the stack
*/
void initializeStack(Stack *stack)
{
    stack->top = 0; // initializing the top of the stack as 0
}

/*
    A function which checks whether the stack is empty or not.
    @param *stack: the pointer of the stack
    @return: the boolean value which represents the stack is empty or not
*/
bool isStackEmpty(Stack *stack)
{
    return (stack->top == 0) ? true : false;
}

/*
    A function which checks whether the stack is full or not.
    @param *stack: the pointer of the stack
    @return: the boolean value which represents the stack is full or not
*/
bool isStackFull(Stack *stack)
{
    return (stack->top == stackSize) ? true : false;
}

/*
    A function which pushes an element onto the stack.
    @param *stack: the pointer of the stack
    @param *value: the string which will be pushed onto the stack
*/
void push(Stack *stack, char *value)
{
    if (isStackFull(stack)) // if the stack is full, then the program ends
    {
        clearScreen();
        printf("Stack is full.\n");
        exit(1);
    }
    else
        strcpy(stack->st[stack->top++], value); // pushing
}

/*
    A function which pops elements from the stack.
    @param *stack: the pointer of the stack
    @return: the popped string
*/
char *pop(Stack *stack)
{
    if (isStackEmpty(stack)) // if the stack is empty, then the program ends
    {
        clearScreen();
        printf("Stack is empty.\n");
        exit(1);
    }

    return stack->st[--stack->top]; // popping
}

/*
    A function which returns the top element of the stack, without pop it.
    @param *stack: the pointer of the stack
    @return: the top element of the stack
*/
char *top(Stack *stack)
{
    return stack->top ? stack->st[stack->top-1] : 0;
}

/*
    A function which cheks a string whether it is numerical or not.
    @param *str: the string
    @return: a boolean value which represents the string is numerical or not.
*/
bool isNumeric(char *str)
{
    // if the first character of the string is '-' and the word has more numerical characters:
    if (*str == '-' && *(str+1) && isdigit(*(str+1)))
        ++str;

    while (*str)
        if (!isdigit(*str++))
            return false;

    return true;
}

/*
    A function which determines precedence of a given operator.
    @param *op: the operator
    @return: the precedence of the operator
*/
int getPrecedence(char *op)
{
    if (*op == '+' || *op == '-')
        return 1;
    else if (*op == '*' || *op == '/')
        return 2;

    return 0;
}

/*
    A function which prints the elements of the stack.
    @param *stack: the pointer of the stack
*/
void printStack(Stack *stack)
{
    int i;

    printf("~ Stack: ");

    for (i = 0; i < stack->top; ++i)
        printf("%s ", stack->st[i]);

    printf("\n");
}

/*
    A function which prints the all variables.
*/
void printVariables()
{
    char ch;

    printf(" Variables: \n\n");

    for (ch = 'a'; ch <= 'z'; ++ch)
        printf(" #%c = %d\n", ch, variables[ch-'a']);

    printf("\n");
}

/*
    A function which prints the banner of the program.
*/
void printBanner()
{
    clearScreen();
    printf("\n\t########################################");
    printf("\n\t############## CALCULATOR ##############");
    printf("\n\t########################################\n\n\n");
}

/*
    A function which clear the terminal screen according to the operating system.
*/
void clearScreen()
{
    #ifdef _WIN32           // for Windows OS
        system("cls");
    #else
        system("clear");    // for Linux and Mac OS
    #endif
}

/*
    A function which displays memory allocation problem to the user.
*/
void crashMemory()
{
    clearScreen();
    perror("Not enough space");
    exit(1);
}

/*
    A function which displays file opening problem to the user.
*/
void crashFile()
{
    clearScreen();
    perror("File could not be opened");
    exit(1);
}