/*********************************************/
/** parse.y - square's lexical and parser    */
/**                                          */
/** How to complie:                          */
/** Linux/Mac:                               */
/** $ bison -d -y parse.y                    */
/**                                          */
/** Windows:                                 */
/** $ win_bison -d -y parse.y                */
/**                                          */
/** Author:Stepfen Shawn                     */
/** Created at Apr 25 00:27:34 2020          */
/**                                          */
/** Copyright (c) 2020 Stepfen Shawn         */
/*********************************************/

%{
#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include "square.h"
#include "node.h"

#include <string.h>
#include <ctype.h>

extern FILE *yyin, *yyout;
extern int yyparse(parser_state*);
extern int yydebug;

#ifndef TRUE
# define TRUE 1
#else
# error TRUE must be 1
#endif
#ifndef FALSE
# define FALSE 0
#else
# error FALSE must be 0
#endif
#ifndef BOOL
# define BOOL int
#endif

typedef enum{
  BEGIN,
  INASSIGN,
  INCOMMENT,
  INNUM,
  INID,
  INSTR,     /*TODO: support string type*/
  FINISH,
}StateType;

#define MAXTOKENLEN 40

#define MAXRESERVED 7

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

#define BUFLEN 4096
int lineno = 0;

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static int EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

#define YYLEX_DECL() yylex(YYSTYPE *yylval)

/*
**  In parse.tab.h:
**  Tokens:  
**  #ifndef YYTOKENTYPE
**  # define YYTOKENTYPE
**  enum yytokentype {
**     keyword_if = 258,
**     keyword_else = 259,
**     keyword_break = 260,
**     keyword_return = 261,
**     keyword_null = 262,
**     keyword_true = 263,
**     keyword_false = 264,
**     keyword_import = 265,
**     op_add = 266,
**     op_sub = 267,
**     op_mul = 268,
**     op_div = 269,
**     op_mod = 270,
**     op_eq = 271,
**     op_neq = 272,
**     op_lt = 273,
**     op_le = 274,
**     op_gt = 275,
**     op_ge = 276,
**     op_and = 277,
**     op_or = 278,
**     op_bar = 279,
**     op_amper = 280,
**     op_next = 281,
**     op_assign = 282,
**     ERROR = 283,
**     lit_number = 284,
**     lit_string = 285,
**     lit_true = 286,
**     lit_false = 287,
**     lit_null = 288,
**     identifier = 289,
**     op_LOWEST = 290,
**     op_HIGHEST = 291
**   };
**  #endif
*/

#define TokenType YYTOKENTYPE

%}

%language "C"

%union {
  node* nd;
  squ_id id;
  size_t int_val;
  char* string_val;
  double double_val;
  char char_val;
}

%type <nd> program stmt_seq
%type <nd> stmt expr condition block cond var primary primary0
%type <nd> stmts args opt_args opt_block f_args map map_args bparam
%type <nd> opt_else opt_elsif
%type <id> identifier

%pure-parser
%parse-param {parser_state *p}

%{
int yylex(YYSTYPE *lval);
static void yyerror(parser_state *p, const char *s);
static int yywarp();
%}

%token
        keyword_if
        keyword_else
        keyword_break
        keyword_return
        keyword_null
        keyword_true
        keyword_false
        keyword_import
%token
        op_add
        op_sub
        op_mul
        op_div
        op_mod
        op_eq
        op_neq
        op_lt
        op_le
        op_gt
        op_ge
        op_and
        op_or
        op_bar
        op_amper
        op_next
        op_assign
        ERROR

%token
        lit_number
        lit_string
        lit_true
        lit_false
        lit_null
        identifier

/*
** precedence table
*/
%nonassoc op_LOWEST

%left  op_amper
%left  op_bar
%left  op_or
%left  op_and
%nonassoc  op_eq op_neq
%left  op_lt op_le op_gt op_ge
%left  op_add op_sub
%left  op_mul op_div op_mod
%right '!' '~'

%token op_HIGHEST


%%

/*
** Grammar for square
*/

program         : stmt_seq
                    { 
                      p->lval = $1;  
                    }
                ;

stmt_seq        : stmts opt_terms
                ;

stmts           :
                    {
                      $$ = node_array_new();
                    }
                | stmt
                    {
                      $$ = node_array_new();
                      node_array_add($$, $1);
                    }
                | stmts terms stmt
                    {
                      $$ = $1;
                      node_array_add($1, $3);
                    }
                | error stmt
                    {
                    }
                ;

stmt            : var op_assign expr
                    {
                      $$ = node_let_new($1, $3);
                    }
                | keyword_return opt_args
                    {
                      $$ = node_return_new($2);
                    }
                | keyword_import identifier
                    {
                      $$ = node_import_new($2);
                    }
                | stmt op_next stmt op_next stmt
                | keyword_break
                    {
                      $$ = node_break_new();
                    }
                | expr
                    {
                      $$ = $1;
                    }
                ;

var             : identifier
                    {
                        $$ = node_ident_new($1);
                    }
                ;

expr            : expr op_add expr
                    {
                      $$ = node_op_new("+", $1, $3);
                    }
                | expr op_sub expr
                    {
                      $$ = node_op_new("-", $1, $3);
                    }
                | expr op_mul expr
                    {
                      $$ = node_op_new("*", $1, $3);
                    }
                | expr op_div expr
                    {
                      $$ = node_op_new("/", $1, $3);
                    }
                | expr op_mod expr
                    {
                      $$ = node_op_new("%", $1, $3);
                    }
                | expr op_bar expr
                    {
                      $$ = node_op_new("|", $1, $3);
                    }
                | expr op_amper expr
                    {
                      $$ = node_op_new("&", $1, $3);
                    }
                | expr op_gt expr
                    {
                      $$ = node_op_new(">", $1, $3);
                    }
                | expr op_ge expr
                    {
                      $$ = node_op_new(">=", $1, $3);
                    }
                | expr op_lt expr
                    {
                      $$ = node_op_new("<", $1, $3);
                    }
                | expr op_le expr
                    {
                      $$ = node_op_new("<=", $1, $3);
                    }
                | expr op_eq expr
                    {
                      $$ = node_op_new("==", $1, $3);
                    }
                | expr op_neq expr
                    {
                      $$ = node_op_new("!=", $1, $3);
                    }
                | op_add expr                 %prec '!'
                    {
                      $$ = node_value_new($2);
                    }
                | op_sub expr                %prec '!'
                    {
                      $$ = node_value_new($2);
                    }
                | '!' expr
                    {
                      $$ = node_op_new("!", NULL, $2);
                    }
                | '~' expr
                    {
                      $$ = node_op_new("~", NULL, $2);
                    }
                | expr op_and expr
                    {
                      $$ = node_op_new("&&", $1, $3);
                    }
                | expr op_or expr
                    {
                      $$ = node_op_new("||", $1, $3);
                    }
                | primary
                    {
                      $$ = $1;
                    }
                ;

condition       : condition op_add condition
                    {
                      $$ = node_op_new("+", $1, $3);
                    }
                | condition op_sub condition
                    {
                      $$ = node_op_new("-", $1, $3);
                    }
                | condition op_mul condition
                    {
                      $$ = node_op_new("*", $1, $3);
                    }
                | condition op_div condition
                    {
                      $$ = node_op_new("/", $1, $3);
                    }
                | condition op_mod condition
                    {
                      $$ = node_op_new("%", $1, $3);
                    }
                | condition op_bar condition
                    {
                      $$ = node_op_new("|", $1, $3);
                    }
                | condition op_amper condition
                    {
                      $$ = node_op_new("&", $1, $3);
                    }
                | condition op_gt condition
                    {
                      $$ = node_op_new(">", $1, $3);
                    }
                | condition op_ge condition
                    {
                      $$ = node_op_new(">=", $1, $3);
                    }
                | condition op_lt condition
                    {
                      $$ = node_op_new("<", $1, $3);
                    }
                | condition op_le condition
                    {
                      $$ = node_op_new("<=", $1, $3);
                    }
                | condition op_eq condition
                    {
                      $$ = node_op_new("==", $1, $3);
                    }
                | condition op_neq condition
                    {
                      $$ = node_op_new("!=", $1, $3);
                    }
                | op_add condition            %prec '!'
                    {
                      $$ = node_value_new($2);
                    }
                | op_sub condition           %prec '!'
                    {
                      $$ = node_value_new($2);
                    }
                | '!' condition
                    {
                      $$ = node_op_new("!", NULL, $2);
                    }
                | '~' condition
                    {
                      $$ = node_op_new("~", NULL, $2);
                    }
                | condition op_and condition
                    {
                      $$ = node_op_new("&&", $1, $3);
                    }
                | condition op_or condition
                    {
                      $$ = node_op_new("||", $1, $3);
                    }
                | cond
                    {
                      $$ = $1;
                    }
                ;

opt_elsif       : /* none */
                    {
                      $$ = NULL;
                    }
                | opt_elsif keyword_else keyword_if condition '{' stmt_seq '}'
                    {
                      $$ = node_if_new($4, $6, NULL);
                    }
                ;

opt_else        : opt_elsif
                    {
                      $$ = NULL;
                    }
                | opt_elsif keyword_else '{' stmt_seq '}'
                    {
                      $$ = $4;
                    }
                ;

opt_args        : /* none */
                    {
                      $$ = node_array_new();
                    }
                | args
                    {
                      $$ = $1;
                    }
                ;

args            : expr
                    {
                      $$ = node_array_new();
                      node_array_add($$, $1);
                    }
                | args ',' expr
                    {
                      $$ = $1;
                      node_array_add($1, $3);
                    }
                ;

primary0        : lit_number
                    {
                      $$ = $<nd>1;
                    }
                | lit_string
                    {
                      $$ = $<nd>1;
					          }
                | identifier
                    {
                      $$ = node_ident_new($1);
                    }
                | '(' expr ')'
                    {
                       $$ = $2;
                    }
                | '[' args ']'
                    {
                      $$ = node_array_of($2);
                    }
                | '[' ']'
                    {
                      $$ = node_array_of(NULL);
                    }
                | '[' map_args ']'
                    {
                      $$ = node_map_of($2);
                    }
                | '[' ':' ']'
                    {
                      $$ = node_map_of(NULL);
                    }
                | keyword_if condition '{' stmt_seq '}' opt_else
                    {
                      $$ = node_if_new($2, $4, $6);
                    }
                | keyword_null
                    {
                      $$ = node_null();
                    }
                | keyword_true
                    {
                      $$ = node_true();
                    }
                | keyword_false
                    {
                      $$ = node_false();
                    }
                ;

cond            : primary0
                    {
                       $$ = $1;
                    }
                | identifier '(' opt_args ')'
                    {
                      $$ = node_call_new(NULL, node_ident_new($1), $3, NULL);
                    }
                | cond '.' identifier '(' opt_args ')'
                    {
                      $$ = node_call_new(NULL, node_ident_new($3), $5, NULL);
                    }
                | cond '.' identifier
                    {
                      $$ = node_call_new($1, node_ident_new($3), NULL, NULL);
                    }
                ;

primary         : primary0
                | block
                    {
                      $$ = node_call_new(NULL, NULL, NULL, $1);
                    }
                | identifier block
                    {
                      $$ = node_call_new(NULL, node_ident_new($1), NULL, $2);
                    }
                | identifier '(' opt_args ')' opt_block
                    {
                      $$ = node_call_new(NULL, node_ident_new($1), $3, $5);
                    }
                | primary '.' identifier '(' opt_args ')' opt_block
                    {
                      $$ = node_call_new($1, node_ident_new($3), $5, $7);
                    }
                | primary '.' identifier opt_block
                    {
                      $$ = node_call_new($1, node_ident_new($3), NULL, $4);
                    }
                ;

map             : lit_string ':' expr
                    {
                      $$ = node_pair_new($<nd>1, $3);
                    }
                | identifier ':' expr
                    {
                      $$ = node_pair_new(node_ident_new($1), $3);
                    }
                ;

map_args        : map
                    {
                      $$ = node_map_new();
                      node_array_add($$, $1);
                    }
                | map_args ',' map
                    {
                      $$ = $1;
                      node_array_add($$, $3);
                    }
                ;

opt_block       : /* none */
                    {
                      $$ = NULL;
                    }
                | block
                    {
                       $$ = $1;
                    }
                ;

block           : '{' bparam stmt_seq '}'
                    {
                      $$ = node_block_new($2, $3);
                    }
                | '{' stmt_seq '}'
                    {
                      $$ = node_block_new(NULL, $2);
                    }
                ;

bparam          : '<-'
                    {
                      $$ = NULL;
                    }
                | f_args '<-'
                    {
                      $$ = $1;
                    }
                ;

f_args          : identifier
                    {
                      $$ = node_array_new();
                      node_array_add($$, node_ident_new($1));
                    }
                | f_args ',' identifier
                    {
                      $$ = $1;
                      node_array_add($$, node_ident_new($3));
                    }
                ;

opt_terms       : /* none */
                | terms
                ;

terms           : term
                | terms term {yyerrok;}
                ;

term            : ';' {yyerrok;}
                | '\n'
                | op_and
                ;
%%
//#define yylval  (*((YYSTYPE*)(p->lval)))
#include "parse.tab.h"

static void
yyerror(parser_state *p, const char *s)
{
  p->nerr++;
  if (p->fname) {
    fprintf(stderr, "%s:%d:%s\n", p->fname, p->lineno, s);
  }
  else {
    fprintf(stderr, "%s\n", s);
  }
  exit(1);
}

static int
yywarp(void){
  return 1;
}

static int
getNextChar(void)
{ 
  if(!(linepos < bufsize))
  { 
    lineno++;
    if(fgets(lineBuf,BUFLEN-1,yyin))
    { 
      bufsize = strlen(lineBuf);
      linepos = 0;
      return lineBuf[linepos++];
    }
    else
    { 
      EOF_flag = TRUE;
      return EOF;
    }
  }
  else
    return lineBuf[linepos++];
}


static void 
ungetNextChar(void)
{ 
  if (!EOF_flag) 
    linepos--;
}

/* lookup table of reserved words */
static struct
    { char* str;
      int tok;
    } reservedWords[MAXRESERVED]
   = {{"if",keyword_if},{"else",keyword_else},{"null",keyword_null},
      {"import",keyword_import},{"false",keyword_false},{"true",keyword_false},
      {"break",keyword_break},{"return",keyword_return}};

/* look for existing keyword*/
/* linear search */
/* TODO: use binary search */
static int 
reservedLookup(char* s)
{ 
  int i;
  for (i = 0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
      return reservedWords[i].tok;
  return identifier;
}


TokenType getToken(){
  int result;
  int c;
  int tokenStringIndex = 0;
  StateType state = BEGIN;
  BOOL save;
  while(state != FINISH)
  {
    c = getNextChar();
    save = TRUE;
    switch(state)
    {
      case BEGIN:
        if(isdigit(c)) state = INNUM;
        else if(isalpha(c)) state = INID;
        else if(c == ':') state = INASSIGN;
        else if((c == ' ') || (c == '\t'))
          save = FALSE;
        else if(c == '\n')
        {
          linepos++;
        }
        else if((c == '#'))
        { 
           save = FALSE;
           state = INCOMMENT;
        }
        else
        {
          state = FINISH;
          switch(c)
          {
            case '==':
              result = op_eq;
            break;
            case '!=':
              result = op_neq;
            break;
            case '<':
              result = op_lt;
            break;
            case '<=':
              result = op_le;
            break;
            case '>':
              result = op_gt;
            break;
            case '>=':
              result = op_ge;
            break;
            case '+':
              result = op_add;
            break;
            case '-':
              result = op_sub;
            break;
            case '*':
              result = op_mul;
            break;
            case '/':
              result = op_div;
            break;
            case '%':
              result = op_mod;
            break;
            case '&&':
              result = op_and;
            break;
            case '||':
              result = op_or;
            break;
            case '&':
              result = op_amper;
            break;
            case '|':
              result = op_bar;
            break;
            case '->':
              result = op_next;
            break;
          }
        }
      break;
      case INCOMMENT:
        save = FALSE;
        if(c == '#') state = BEGIN;
        if(c == EOF)
        { 
          state = FINISH;
        }
      break;
      case INASSIGN:
        if(c == '='){
          state = FINISH;
          result = op_assign;
        }
      break;
      case INNUM:
        if(!isdigit(c))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = lit_number;
        }
      break;
      case INID:
        if(!isalpha(c))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = identifier;
        }
      break;
      case FINISH:
      default:
        printf("Lexical Error: State : %d\n",state);
        state = FINISH;
        result = ERROR;
      break;
    }
    if((save) && (tokenStringIndex <= MAXTOKENLEN))
      tokenString[tokenStringIndex++] = (char) c;
    if(state == FINISH)
    { 
      tokenString[tokenStringIndex] = '\0';
      if(result == identifier)
        result = reservedLookup(tokenString);
    }
  }
  return result;
}

int
yylex(YYSTYPE *yylval)
{
  return getToken();
}