/* front.c - a lexical analyzer system for simple
 arithmetic expressions */
#include <stdio.h>
#include <ctype.h>
#include <regex>
#include<iostream>
using namespace std;


/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char checknumber [20];  /*sm sixe to make number*/
char nextChar;
int lexLen;
int numlen; /* sm***/
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define FLT_PT 30
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define MOD_OP 40
#define EQUALS_OP 50
#define LESS_OP 51
#define MORE_OP 52
#define AND_OP 60
#define OR_OP 62
#define LESS_EQUAL_OP 64
#define MORE_EQUAL_OP 66
/******************************************************/
/* main driver */
int main(void) {
	/* Open the input data file and process its contents */
	 if ((in_fp = fopen("front.in", "r")) == NULL)
	 	printf("ERROR - cannot open front.in \n");
	 else {
	 	getChar();
	 do {
	 	lex();
	 } while (nextToken != EOF);
	 }
	 return 0;
}
/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 nextToken = DIV_OP;
			 break;
      case '%':
			 addChar();
			 nextToken = MOD_OP;
			 break;
     case '=':
			 addChar();
			 nextToken = ASSIGN_OP;
			 break;
     case '<':
			 addChar();
			 nextToken = LESS_OP;
			 break;
     case '>':
			 addChar();
			 nextToken = MORE_OP;
			 break;
     case '~':
			 addChar();
			 nextToken = EQUALS_OP;
			 break;
     case '&':
			 addChar();
			 nextToken = AND_OP;
			 break;
     case '$':
			 addChar();
			 nextToken = OR_OP;
			 break;
    case '#':
			 addChar();
			 nextToken = OR_OP;
			 break;
     case '^':
			 addChar();
			 nextToken = OR_OP;
			 break;
     
		 default:
			 addChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
}


/***new function*******************/
void getNumber(void) {
  checknumber[numlen++] = nextChar;  /*write first char*/
	 if ((nextChar = getc(in_fp)) != EOF) {
		checknumber[numlen++] = nextChar; 
	 } else
	 	charClass = EOF;
}


/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}

/******************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex(void) {
	 lexLen = 0;
    
	 getNonBlank();
	 switch (charClass) {
		/* Identifiers */
		 case LETTER:
			 addChar();
			 getChar();
			 while (charClass == LETTER || charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			 nextToken = IDENT;
			 break;
		/* Integer literals */
		 case DIGIT:
       numlen = 0;  /* sm****/
			 addChar();
			 /* getChar(); */
       checknumber[numlen++] = nextChar;  /*write first char*/
			 while (isspace(nextChar) == 0) {       /* change here */
         getNumber();
				 addChar();
         
         }  
          if(regex_search(lexeme,
        regex("\\.")))  
       /*   if(regex_match(lexeme,
        regex("([0-7]+|[1-9][0-9]*|0[xX][0-9a-fA-F]+)((u|U)?(i64|I64)?(l|L)?)"))) 
             { nextToken = INT_LIT; }  */
         {  
           nextToken = FLT_PT;
        }
       else 
         { nextToken = INT_LIT; }   
        
     /*    else 
        if(regex_match(lexeme,
        regex("([0-9]*\\.[0-9]+|[0-9]+\\.)([eE][-\\+]?[0-9]+)?([lLfF]?)|[0-9]+[eE][-\\+]?[0-9]+[lLfF]?"))) 
         {
           nextToken = FLT_PT;
        }   
        */
         
		 	break;
		/* Parentheses and operators */
		 case UNKNOWN:
			 lookup(nextChar);
			 getChar();
			 break;
			/* EOF */
		case EOF:
			 nextToken = EOF;
			 lexeme[0] = 'E';
			 lexeme[1] = 'O';
			 lexeme[2] = 'F';
			 lexeme[3] = '\0';
		 	break;
	 } /* End of switch */
  if(nextToken != INT_LIT && nextToken != FLT_PT)
  { printf("Next token is: %d,  Next lexeme is %s\n",
  	 nextToken, lexeme); }
  else {printf("Next token is: %d, Next lexeme is %s\n",
  	nextToken, lexeme); }
   
	 return nextToken;
} /* End of function lex */