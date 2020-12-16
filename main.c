// Cameron Gibson (R#) | Assignment #4 | 10/26/20
// 
//  Lexical analyzer

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "front.h"

/* Global Variable */
int nextToken;

/* Local Variables */
static int charClass;
static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;
static char tokenName [100];
static char *keywords[] = {"read", "write", "while", "do", "od", "var"};

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();

/******************************************************/
/* main driver */
int main(int argc, char **argv) 
{
    char c;
    /* Open the input data file and process its contents */

    // If user didn't provide a file:
    // Display appropriate error message, exit code of 2
    
		if (argv[1] == NULL){
        printf("No file provided \n");
        exit(2);
    }
		

    // If user provided a file that DNE: 
    // Display appropriate error message, exit code of 3
		// Saved line for linux file input
		if ((in_fp = fopen(argv[1], "r")) == NULL) {
		// Used below for testing in repl.it
		//if ((in_fp = fopen("test.danc", "r")) == NULL) {
        printf("ERROR - cannot open file \n");
        exit(3);
    } 

    else {
        printf("DanC Parser :: R11424503 \n\n");
        getChar();
        do {
            lex();
            // Parser
            input(lexeme, tokenName);
        } while (nextToken != EOF);
    }

    // If provided file is free of syntax errors
    // Print out "Syntax Validated" at last line of output
    // Has exit code of 0

    printf("\nSyntax Validated\n");
    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the token */
static int lookup(char ch) {
    switch (ch) {
    	case '(':
    	    addChar();
    	    nextToken = LEFT_PAREN;
    	    strcpy(tokenName, "LEFT_PAREN");
    	    break;
    	case '=':
    	    addChar();
    	    nextToken = EQUAL_OP;
    	    strcpy(tokenName, "EQUAL_OP");
    	    break;
    	case ';':
    	    addChar();
    	    nextToken = SEMICOLON;
    	    strcpy(tokenName, "SEMICOLON");
    	    break;
    	case ')':
    	    addChar();
    	    nextToken = RIGHT_PAREN;
    	    strcpy(tokenName, "RIGHT_PAREN");
    	    break;
    	case '+':
    	    addChar();
    	    nextToken = ADD_OP;
    	    strcpy(tokenName, "ADD_OP");
    	    break;
    	case '-':
    	    addChar();
    	    nextToken = SUB_OP;
    	    strcpy(tokenName, "SUB_OP");
    	    break;
    	case '*':
    	    addChar();
    	    nextToken = MULT_OP;
    	    strcpy(tokenName, "MULT_OP");
    	    break;
    	case '/':
    	    addChar();
    	    nextToken = DIV_OP;
    	    strcpy(tokenName, "DIV_OP");
    	    break;
    	case '<':
    	    addChar();
    	    // implement this here instead of in loop if more than 1 call
    	    nextChar = getc(in_fp); 
    	    // if loop for <= case
    	    if (nextChar == '='){
    	        addChar();
    	        nextToken = GEQUAL_OP;
    	        strcpy(tokenName, "LEQUAL_OP");
    	    }
    	    // else if for NEQUAL_OP
    	    else if (nextChar == '>'){
    	        addChar();
    	        nextToken = NEQUAL_OP;
    	        strcpy(tokenName, "NEQUAL_OP");
    	    }
    	    // Default is LESSER_OP
    	    else{
    	        nextToken = LESSER_OP;
    	        strcpy(tokenName, "LESSER_OP"); 
    	    }
    	    break;
    	case '>':
    	    addChar();
    	    // If the next char is =, add and declare tokenName
    	    if ((nextChar = getc(in_fp)) == '='){
    	        addChar();
    	        nextToken = GEQUAL_OP;
    	        strcpy(tokenName, "GEQUAL_OP");
    	    }
    	    else{
    	        nextToken = GREATER_OP;
    	        strcpy(tokenName, "GREATER_OP");    
    	    }
    	    break;      
    	case ':':
    	    addChar();
    	    nextChar = getc(in_fp);
    	    if (nextChar == '='){
    	        addChar();
    	        nextToken = ASSIGN_OP;
    	        strcpy(tokenName, "ASSIGN_OP");
    	    }
    	    else{
    	        // use unget to put the next input back if not ==
    	        ungetc(nextChar, in_fp);
    	        nextToken = UNKNOWN;
    	        strcpy(tokenName, "UNKNOWN");
    	    }
    	    break;
    	default:
    	    addChar();
    	    break;
    }
    return nextToken;
}

/* Determine if a keyword or not */
static int keylookup(char ch) {
    switch (ch) {
        case 'd':
            addChar();
            nextChar = getc(in_fp);
            if (nextChar == 'o'){
                nextToken = KEY_DO;
                strcpy(tokenName, "KEY_DO");
            }
            else{
                nextToken = IDENT;
                strcpy(tokenName, "IDENT");
            }
            break;
        case 'o':
            addChar();
            nextChar = getc(in_fp);
            if (nextChar == 'd'){
                nextToken = KEY_OD;
                strcpy(tokenName, "KEY_OD");
            }
            else{
                nextToken = IDENT;
                strcpy(tokenName, "IDENT");
            }
            break;
        case 'r':
            addChar();
            nextChar = getc(in_fp);
            if (nextChar == 'e'){
                nextToken = KEY_READ;
                strcpy(tokenName, "KEY_READ");
            }
            else{
                nextToken = IDENT;
                strcpy(tokenName, "IDENT");
            }
            break;
        case 'w':
            addChar();
            nextChar = getc(in_fp);
            if (nextChar == 'r'){
                nextToken = KEY_READ;
                strcpy(tokenName, "KEY_WRITE");
            }
            else if (nextChar == 'h'){
                nextToken = KEY_WHILE;
                strcpy(tokenName, "KEY_WHILE");
            }
            else{
                nextToken = IDENT;
                strcpy(tokenName, "IDENT");
            }
            break;
          default:
          	nextToken = IDENT;
                strcpy(tokenName, "IDENT");
            break;
        }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar)){
          charClass = LETTER;
        }
        else if (isdigit(nextChar)){
          charClass = DIGIT;
        }
        else{
          charClass = UNKNOWN;
        }
    } 
    else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* Lex - a simple lexical analyzer for arithmetic expressions */
/*  Invalid lexemes should output UNKNOWN as their token group */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            keylookup(nextChar);
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            strcpy(tokenName, "INT_LIT");
            nextToken = INT_LIT;
            break;

        /* Parentheses and operators */
        case UNKNOWN:
            // default UNKNOWN, if listed operator will re-assign tokenName
            strcpy(tokenName, "UNKNOWN");
            lookup(nextChar);
            getChar();
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            strcpy(tokenName, "EOF");
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } 
        /* End of switch */
        // Program was producing dupe lines at end, if loop stops this
        if(nextToken == EOF){
            return nextToken;
        }
        else{
            printf("Next lexeme: %s        Next Token: %s \n", lexeme, tokenName);
        }
    return nextToken;
}
