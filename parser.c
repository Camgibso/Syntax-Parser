// Cameron Gibson (R#11424503) | Assignment #4 | 10/26/20
// 
//  A syntax parser with the following grammar rules:
// P::= S
// S::= V:=E | read (V) | write(V) | while C do S od | S;S
// C::= E< E | E> E | E= E | E<>E | E<= E | E>= E
// E::= T| E+T| E-T
// T::= F| T*F| T/F
// F::= (E)| N| V
// V::= a | b | ... | y | z | aV| bV| ... | yV| zV
// N::= 0 | 1 | ... | 8 | 9 | 0N| 1N| ... | 8N| 9N
// 
// Error codes on Linux: echo $? -return echo code
//  Note: If a user input file contains multiple syntax errors,
// your solution is only required to find and report the 1st syntax error

#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "front.h"

/* Local Variables */
static void error();

// P::= S
void input(char lexeme[], char tokenName[]){
	// Rules to follow:
	// S::= V:=E | read (V) | write(V) | while C do S od | S;S
	//printf("Enter <input> \n");

	// Make a switch case for the listed
	switch(nextToken){
		// Case for Variable name / IDENT
		case IDENT:
			lex(); /* Get the next token */
			// If next token is an assignment, parse the expression
			if(nextToken == ASSIGN_OP){
				lex(); // Get next token send to expression
				expression(lexeme, tokenName);
				//printf("lexeme: %s / token name: %s / nextToken: %d -here\n", lexeme, tokenName, nextToken);
			}
			// if the next token isn't keyod, semicolon or ~eof? error out
			if(nextToken != KEY_OD && nextToken != SEMICOLON){
				error(lexeme, tokenName);
			}
			break;

		// Case for read
		case KEY_READ:
			lex();
			expression(lexeme, tokenName);
			break;

		// Case for write
		case KEY_WRITE:
			lex();
			expression(lexeme, tokenName);
			break;

		// Case for while
		case KEY_WHILE:
			lex();
			if(nextToken == IDENT){
				lex();
			}	
			comparison(lexeme, tokenName);
			// Wait till key do
			if(nextToken == KEY_DO){
				lex(); // get next token
				input(lexeme, tokenName); // start back over still key od

				// while loop to run through till key od
				while(nextToken != KEY_OD){
					lex(); 
					input(lexeme, tokenName);
				}

				if(nextToken == KEY_OD){
					lex();
					if(nextToken == KEY_OD || nextToken == SEMICOLON){
						//error(lexeme, tokenName);
					}
					else{
						error(lexeme, tokenName);
					}
				}
				// ends in eof or semicolon
				//lex(); // get next token
				// if the next token isn't eof, semicolon error out
				/*if(nextToken != EOF || nextToken != SEMICOLON){
					printf("he!!!!!!!!!!!!!!!!!!!!!!re\n");
					error(lexeme, tokenName);
				}*/
			}
			else{
				error(lexeme, tokenName);
			}
			break;

		case KEY_OD:
			lex();
			if(nextToken != KEY_OD || nextToken != SEMICOLON){
				error(lexeme, tokenName);
			}
			else{
				lex();
			}
			break;

		// Case for semicolon
		// S::= V:=E | read (V) | write(V) | while C do S od | S;S
		case SEMICOLON:
			lex();
			//printf("lexeme: %s / token name: %s / nextToken: %d -here\n", lexeme, tokenName, nextToken);
			// Next input can only be key of or eof or ident
			if(nextToken == KEY_OD || nextToken == EOF || nextToken == IDENT){
				error(lexeme, tokenName);
			}
			break;
		case EOF:
			break;
		default:
			break;
	}
	
	//printf("Exit <input> \n");
}

void comparison(char lexeme[], char tokenName[]){
	// C::= E< E | E> E | E= E | E<>E | E<= E | E>= E
	//printf("Enter <comp> \n");
	lex();

	// While loop according to syntax above, send to expression
	while(nextToken == LESSER_OP || nextToken == GREATER_OP || nextToken ==  EQUAL_OP || 
		nextToken == NEQUAL_OP || nextToken == LEQUAL_OP || nextToken == GEQUAL_OP) {
		lex(); /* Get the next token */
		expression(lexeme, tokenName);
	}
	expression(lexeme, tokenName);
	//printf("Exit <comp> \n");
}

void expression(char lexeme[], char tokenName[]){
	// E::= T| E+T| E-T
	//printf("Enter <expr> \n");
	term(lexeme, tokenName);

	// Next token can be a + or -
	while(nextToken == ADD_OP || nextToken == SUB_OP){
		lex(); /* Get the next token */
		expression(lexeme, tokenName);
	}
	//printf("Exit <expr> \n");
}

void term(char lexeme[], char tokenName[]){
	// T::= F| T*F| T/F
	//printf("Enter <term> \n");
	factor(lexeme, tokenName);
	// Next token is * or /, get the next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
            lex(); /* Get the next token */
            factor(lexeme, tokenName);
    }
	//printf("Exit <term> \n");
}

void factor(char lexeme[], char tokenName[]){
	// F::= (E)| N| V
	printf("Enter <factor> \n");

  /* Determine which RHS */
	
  if (nextToken == IDENT || nextToken == INT_LIT) {
    //printf("\n lexeme: %s / token name: %s / nextToken: %d -in if loop", lexeme, tokenName, nextToken);
    lex(); /* Get the next token */
	} 
  else {
  	/* If the RHS is (<expr>), call lex to pass over the 
  	left parenthesis, call expr, and check for the right 
  	parenthesis */
  	if (nextToken == LEFT_PAREN) {
  	    lex(); 
  	    expression(lexeme, tokenName);

  	    if (nextToken == RIGHT_PAREN) {
  	        lex(); 
						if(nextToken != KEY_OD && nextToken != SEMICOLON && nextToken != EOF){
							error(lexeme, tokenName);
						}
  	    } 
  	    else { 
					error(lexeme, tokenName);
  	    }
  	} /* End of if (nextToken == ... */
  	/* It was not an id, an integer literal, or a left parenthesis */
  	else{ 
  	  error(lexeme, tokenName); 
  	}
 	} /* End of else */
	printf("Exit <factor> \n");
}

// If the provided user file contains syntax errors
static void error(char lexeme[], char tokenName[]){
	// Your solution must print out “Error encounter: 
	// The next lexeme was <lexeme>and the next token was <token>”
	printf("\nError encounter: The next lexeme was %s and the next token was %s \n", lexeme, tokenName);
	exit(1);
}