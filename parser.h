// parser.h

#ifndef PARSER_H
#define PARSER_H

void input(char lexeme[], char tokenName[]);
void comparison(char lexeme[], char tokenName[]);
void expression(char lexeme[], char tokenName[]);
void term(char lexeme[], char tokenName[]);
void factor(char lexeme[], char tokenName[]);

extern int nextToken;
#endif