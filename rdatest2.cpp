/* Recursive Descent Algorithm
 arithmetic expressions */
#include <stdio.h>

#include <ctype.h>
#include <iostream>
#include <regex>
using namespace std;

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char checkvalue[100];
char checknumber[20]; 
char nextChar;
int lexLen;
int numlen; 
int token;
int nextToken;
int firsttime;
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
int lex(void);
void ifstmt(void);
void whileloop(void);
void stmt (void);
void stmtlist (void);
void block (void);
void expr(void);
void term(void);
void bterm(void);
void error(void);
void boolexpr(void);
void assign (void);
void factor(void);
void declr(void);
void band (void);
void bor (void);
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
#define IF_CODE 70
#define ELSE_CODE 74
#define LEFT_CURLY 71
#define RIGHT_CURLY 75
#define SEMI_COLON 72
#define WHILE_CODE 73
#define BTERM_CODE 80
#define DATATYPE_CODE 81
#define NOT_EQUALS_OP 82
#define COMMA_CODE 83
/******************************************************/
/* main driver */
int main(void) {
  /* Open the input data file and process its contents */
  if ((in_fp = fopen("front.in", "r")) == NULL)
    printf("ERROR - cannot open front.in \n");
  else {
    firsttime = 1;
    getChar();

    do {
        if(firsttime == 1)
        {
       lex ();
        stmt(); 
        }
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
    
  case '{':
    addChar();
    nextToken = LEFT_PAREN;
    break;
    
  case '}':
    addChar();
    nextToken = RIGHT_CURLY;
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
  /* case '=':
    addChar();
    nextToken = ASSIGN_OP;
    break; */
  case '<':
    addChar();
    nextToken = LESS_OP;
    break;
  case '>':
    addChar();
    nextToken = MORE_OP;
    break;
  case '=':
    addChar();
    nextToken = EQUALS_OP;
    break;
  
  
   default:
    addChar();
    nextToken = EOF;
    break;
  }
  return nextToken;
}
/**********************************************************/

void error(void)
{
printf("Error - Not in the grammar \n");

}

// start sm 
void stmt (void) {
  printf("Enter <STMT>\n");
if(nextToken == IF_CODE)
    { 
    ifstmt();
    }

else if(nextToken == LEFT_CURLY)
     { block(); 
    }

else if(nextToken ==  IDENT )   
	{  
           assign() ;      
    }


else if(nextToken == DATATYPE_CODE)
     {declr(); 
    }

else if (nextToken ==  WHILE_CODE)
   {  whileloop() ; 
       
   }


else {
      error ();
	    } 
printf("Exit <STMT>\n");
}





void stmtlist (void) {
printf("Enter <STMT LIST >\n");
while (nextToken == IF_CODE || nextToken == LEFT_CURLY || 
  nextToken ==  IDENT || nextToken ==  DATATYPE_CODE || nextToken ==  WHILE_CODE) 

{
stmt ();

lex ();
if (nextToken != SEMI_COLON)
 error ();


} //endwhile

printf("Exit <STMT LIST >\n");
} //end statment list





void assign (void)

{

if (nextToken != IDENT)
    error ();
     else
     {
      lex ();
	  if (nextToken != EQUALS_OP)
                   error ();
				   else
				      {
				     lex();
					 expr();
					 }
    }
  printf("Exit <STMT LIST >\n");
}




void ifstmt(void) {
  printf("Enter <IF STMT>\n");
 if (nextToken != IF_CODE)
 	error();


else  {
	     		lex();
			if (nextToken != LEFT_PAREN)
 		           error();
         			 else {
 		       		lex();
 		       		boolexpr();
                             }
                   

			 lex();

                   if (nextToken != RIGHT_PAREN)
 			  error();

                     else 
                       {
                          lex ();
                          block ();
                       }


                   lex ();
                if (nextToken == ELSE_CODE) 
 				{
				lex();
                        block();
                          }
 			
}
  printf("Exit <STMT LIST >\n");

}



////////////////////////

void whileloop(void) {
  printf("Enter <WHILE LOOP >\n");
 if (nextToken !=   WHILE_CODE)
 	error();
		else  {
	    			 lex();
				if (nextToken != LEFT_PAREN)
 		     		error();
      				else {
 		       			lex();
 		       			boolexpr();
                                   }

                          lex();
      			if (nextToken != RIGHT_PAREN)
 				error();

                        else {
                              lex ();
 					block();
                              }
 			
                 }

  printf("Exit <WHILE lOOP >\n");  			
}



////////////////////////////////
void block (void)
{
  printf("Enter <BLOCK >\n");
if (nextToken != LEFT_CURLY)
       error();
  else
     {
       lex();
       stmtlist();
        lex ();
       if (nextToken != RIGHT_CURLY)
          error ();
     }
  printf("EXIT <BLOCK >\n");
}


/////////////////////
void band (void)
{
 printf("Enter <band>\n");
	/* Parse the first bor */
  
	 bor();
	/* As long as the next token is + or -, get
	 the next token and parse the next term */
        lex ();
	 while (nextToken == AND_OP) {
		 lex();
		 bor();
            
	 }
	 printf("Exit <band>\n");




}


/////////////////////////
void bor (void)
{

printf("Enter <bor>\n");
  //ops are not correct
	/* Parse the first bor */
	 expr();
	
        lex ();
	 while (nextToken == EQUALS_OP  || NOT_EQUALS_OP) {
		 lex();
		 expr();
            
	 }
	 printf("Exit <bor>\n");

}


//////* term
 
void term(void) {
	 printf("Enter <term>\n");
	/* Parse the first factor */
	 factor();
	/* As long as the next token is * or /, get the
	 next token and parse the next factor */
   lex ();
	 while (nextToken == MULT_OP || nextToken == DIV_OP  || nextToken == MOD_OP  ) {
		 lex();
		 factor();
	 }
	 printf("Exit <term>\n");
}
/////////////////////////////////


/////////////////////////////

void expr(void) {
	 printf("Enter <expr>\n");
	/* Parse the first term */
	 term();
	/* As long as the next token is + or -, get
	 the next token and parse the next term */
       lex ();
	 while (nextToken == ADD_OP || nextToken == SUB_OP) {
		 lex();
		 term();
	 }
	 printf("Exit <expr>\n");
}
////////////////////////////


void factor(void) {
	 printf("Enter <factor>\n");
	 if (nextToken != IDENT || nextToken != INT_LIT  || nextToken != FLT_PT  || 
     nextToken != LEFT_PAREN)
   {
		error ();
		
	  if (nextToken == LEFT_PAREN)
		 expr();
		
	 }
	 printf("Exit <factor>\n");
}

///////////////////////

void bterm (void)
{

 printf("Enter <bterm>\n");
	/* Parse the first term */
	 band();
	/* As long as the next token is + or -, get
	 the next token and parse the next term */
        lex ();
	 while (nextToken == EQUALS_OP  || NOT_EQUALS_OP) {
		 lex();
		 band();
	 }
	 printf("Exit <bterm>\n");

}





///////////////////////////////////
void boolexpr(void) {
	 printf("Enter <boolexpr>\n");
	/* Parse the first bterm */
	 bterm();
	/* As long as the next token is * or /, get the
	 next token and parse the next factor */
       lex ();
	 while (nextToken ==  LESS_OP  ||  nextToken != MORE_OP || nextToken != LESS_EQUAL_OP || nextToken !=  MORE_EQUAL_OP  ) 
	 {
		 lex();
		 bterm();
	 }
	 printf("Exit <boolexpr>\n");
}

/////////////////////////////
/* declaration
 Parses strings in the language generated by the rule:
 <DECLARE> -> `DataType` IDENT {`,` IDENT }
 */
void declr(void) {
	 printf("Enter <DECLARE>\n");

         if( nextToken != DATATYPE_CODE )
                   error ();
             else
                 {
	                                        lex();

                               if(nextToken != IDENT)
                                      error ();
                                   else
                                 {
                                    lex();
                                 while (nextToken == COMMA_CODE) 
                                              {
                                                 lex();
                                                 if(nextToken != IDENT)
                                                           error ();
										 else
										    lex();
                                               }


			               }
 	

                 }
                	
                                      		
	 printf("Exit <declare>\n");
}

/////////////////////////end sm 


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
  checknumber[numlen++] = nextChar; /*write first char*/
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

    if (regex_search(lexeme, regex("if")))
    
    {
      nextToken = IF_CODE;
    }

        else  if(regex_search(lexeme, regex("else")))
       {
         nextToken = ELSE_CODE; 
       }

     else  if(regex_search(lexeme, regex("while")))
       {
         nextToken = WHILE_CODE; 
       }
       
    else  if(regex_search(lexeme, regex("int")))
       {
         nextToken = DATATYPE_CODE; 
       }

      else  if(regex_search(lexeme, regex("float")))
       {
         nextToken = DATATYPE_CODE; 
       }
       else  
       {
         nextToken = IDENT;
       }
    
    //nextToken = IDENT;
    break;
    /* Integer literals */
  case DIGIT:
    numlen = 0; /* sm****/
     /* addChar(); 
      getChar(); */
    
    
      
     while ( charClass == DIGIT )
       
       /* || (charClass == UNKNOWN &&
       (regex_search(lexeme, regex("\\."))))) */
    { /* change here */
       
      addChar();
      getChar(); 
    }
 
     checknumber[numlen++] = nextChar;
      if(charClass == UNKNOWN &&
          (regex_search(checknumber, regex("\\."))))
        {
          addChar();

           while ( charClass == DIGIT  )
                    { // change here 
       
                  addChar();
              getChar();  }
       }  
    
    if (regex_search(lexeme, regex("\\.")))
    /*   if(regex_match(lexeme,
     regex("([0-7]+|[1-9][0-9]*|0[xX][0-9a-fA-F]+)((u|U)?(i64|I64)?(l|L)?)")))
          { nextToken = INT_LIT; }  */
    {
      nextToken = FLT_PT;
    } else {
      nextToken = INT_LIT;
    }

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
    
    while (charClass == UNKNOWN)
    { /* change here */
      addChar();
      getChar(); 
    }  
   
     if (regex_search(lexeme, regex("\\<\\=")))
    
    {
      nextToken = LESS_EQUAL_OP;
    }

     else  if(regex_search(lexeme, regex("\\>\\=")))
       {
         nextToken = MORE_EQUAL_OP; 
       }
       

      else  if(regex_search(lexeme, regex("\\=\\=")))
       {
         nextToken = ASSIGN_OP; 
       }


         else  if(regex_search(lexeme, regex("\\!\\=")))
       {
         nextToken = NOT_EQUALS_OP; 
       }
       
       
       
       else  if((isalpha(nextChar) == 1) ||  (isdigit(nextChar) == 1))
       {
         lookup(nextChar); 
       }
  
    
   //lookup(nextChar); 
   
    


    
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
  if (nextToken != INT_LIT && nextToken != FLT_PT) {
    printf("Next token is: %d,  Next lexeme is %s\n", nextToken, lexeme);
  } else {
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
  }

  return nextToken;
} /* End of function lex */