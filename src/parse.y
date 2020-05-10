/************************************************************************************/
/**                                                                                 */
/** parse.y - square's lexical and parser                                           */
/**                                                                                 */
/** How to complie:                                                                 */
/** Linux/Mac:                                                                      */
/** $ bison -d -y parse.y                                                           */
/**                                                                                 */
/** Windows:                                                                        */
/** $ win_bison -d -y parse.y                                                       */
/**                                                                                 */
/** Author:Stepfen Shawn                                                            */
/** Created at Apr 25 00:27:34 2020                                                 */
/**                                                                                 */
/** Copyright (c) 2020 Stepfen Shawn                                                */
/**                                                                                 */
/** Permission is hereby granted, free of charge, to any person obtaining a copy    */
/** of this software and associated documentation files (the "Software"), to deal   */
/** in the Software without restriction, including without limitation the rights    */
/** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell       */
/** copies of the Software, and to permit persons to whom the Software is           */
/** furnished to do so, subject to the following conditions:                        */
/**                                                                                 */
/** The above copyright notice and this permission notice shall be included in all  */
/** copies or substantial portions of the Software.                                 */
/**                                                                                 */
/** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      */
/** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        */
/** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     */
/** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          */
/** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   */
/** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   */
/** SOFTWARE.                                                                       */                                            
/************************************************************************************/

%{

#define YYDEBUG 1
#define YYERROR_VERBOSE 1

#include "square.h"
#include "node.h"

#include <string.h>
#include <ctype.h>
#include <malloc.h> 

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

/* State type in Lexcial */
typedef enum
{
  BEGIN,       /* start */
  INASSIGN,    /* assign */
  INCOMMENT,   /* comment */
  INNUM,       /* number */
  INID,        /* identifier */
  INSTR,       /* string */
  FINISH,      /* end */
}StateType;

#define MAXTOKENLEN 80

#define MAXRESERVED 10

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

#define BUFLEN 4096
int lineno = 0;

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static BOOL EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

#define YYLEX_DECL() yylex(YYSTYPE *yylval)

static char*
strdup_(const char *str)
{
    //Space for length plus nul
    char *new_str = malloc(strlen (str) + 1);  
    // No memory
    if (new_str == NULL)
        return NULL;
    //Copy the characters          
    strcpy(new_str, str);       
    //Return the new string               
    return new_str;                            
}


static char*
strndup_(const char *str, size_t chars)
{
    char* buffer;
    size_t n;

    buffer = (char *)malloc(chars + 1);
    if(buffer)
    {
        for (n = 0;((n < chars) && (str[n] != 0));n++)
        {
          buffer[n] = str[n];
        }
        buffer[n] = 0;
    }

    return buffer;
}

/*
   enum yytokentype {
     keyword_if = 258,
     keyword_else = 259,
     keyword_break = 260,
     keyword_return = 261,
     keyword_null = 262,
     keyword_true = 263,
     keyword_false = 264,
     keyword_import = 265,
     keyword_goto = 266,
     keyword_block = 267,
     op_add = 268,
     op_sub = 269,
     op_mul = 270,
     op_div = 271,
     op_mod = 272,
     op_eq = 273,
     op_neq = 274,
     op_lt = 275,
     op_le = 276,
     op_gt = 277,
     op_ge = 278,
     op_and = 279,
     op_or = 280,
     op_bar = 281,
     op_amper = 282,
     op_next = 283,
     op_assign = 284,
     op_lp = 285,
     op_rp = 286,
     op_flp = 287,
     op_frp = 288,
     ERROR = 289,
     ENDFILE = 290,
     lit_number = 291,
     lit_string = 292,
     lit_true = 293,
     lit_false = 294,
     lit_null = 295,
     identifier = 296,
     op_LOWEST = 297,
     op_HIGHEST = 298
   };
*/

#define TokenType YYTOKENTYPE

/* define the string list */
/* use list to save the string value */
typedef struct
{
    squ_string stringtable;
    size_t n;
}STRList;


void List_Init(STRList *list);
void List_Append(STRList* list, int value);


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

%type <nd> 
      program 
      stmt_seq

%type <nd> 
      stmt
      expr
      condition
      block
      grade
      cond
      var
      primary
      primary0

%type <nd> 
      stmts 
      args 
      opt_args 
      opt_block 
      f_args 
      map 
      map_args 
      bparam

%type <nd> 
      opt_else 
      opt_elsif

%type <id> 
      identifier

%type <nd>
      lit_number
      lit_string

%pure-parser
%parse-param {parser_state *p}

%{
static int yylex(YYSTYPE *lval);
static void yyerror(parser_state* p, const char* s);
static int yywarp(void);
static void yywarnning(parser_state* p,const char* s);
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
        keyword_goto
        keyword_block
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
        op_lp
        op_rp
        op_flp
        op_frp

%token
        ERROR
        ENDFILE

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
                | keyword_goto grade
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
                | opt_elsif keyword_else keyword_if op_lp condition op_rp op_flp stmt_seq op_frp
                    {
                      $$ = node_if_new($5, $8, NULL);
                    }
                ;

opt_else        : opt_elsif
                    {
                      $$ = NULL;
                    }
                | opt_elsif keyword_else op_flp stmt_seq op_frp
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

grade           : /* none */
                | '@' identifier
                ;

primary0        : lit_number
                | lit_string
                | identifier
                    {
                      $$ = node_ident_new($1);
                    }
                | op_lp expr op_rp
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
                | keyword_if op_lp condition op_rp op_flp stmt_seq op_frp opt_else
                    {
                      $$ = node_if_new($3, $6, $8);
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
                | identifier op_lp opt_args op_rp
                    {
                      $$ = node_call_new(NULL, node_ident_new($1), $3, NULL);
                    }
                | cond '.' identifier op_lp opt_args op_rp
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
                | keyword_block identifier block
                    {
                      $$ = node_call_new(NULL, node_ident_new($2), NULL, $3);
                    }
                | identifier op_lp opt_args op_rp opt_block
                    {
                      $$ = node_call_new(NULL, node_ident_new($1), $3, $5);
                    }
                | primary '.' identifier op_lp opt_args op_rp opt_block
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

block           : op_flp bparam stmt_seq op_frp
                    {
                      $$ = node_block_new($2, $3);
                    }
                | op_flp stmt_seq op_frp
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

opt_terms       : terms
                ;

terms           : term
                | terms term {yyerrok;}
                | /* none */
                ;

term            : ' ' 
                | op_and
                | ';'
                | '\n'
                ;
%%
//#define yylval  (*((YYSTYPE*)(p->lval)))
#include "parse.tab.h"

static void
yyerror(parser_state *p, const char *s)
{
  p->nerr++;
  if (p->file_name)
  {
    fprintf(stderr, "%s[Error]:%d:%s\n", p->file_name, p->lineno, s);
  }
  else 
  {
    fprintf(stderr, "%s\n", s);
  }
  exit(1);
}

static void
yywarnning(parser_state* p,const char* s)
{
  if(p->file_name)
  {
    printf("%s[Warnning]:%d:%s\n",p ->file_name,p->lineno,s);
  }
  else
  {
    printf("[Warnning]:%s",s);
  }
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
    { 
      char* str;
      int tok;
    } reservedWords[MAXRESERVED]
   = {{"if",keyword_if},{"else",keyword_else},{"null",keyword_null},
      {"import",keyword_import},{"false",keyword_false},{"true",keyword_false},
      {"break",keyword_break},{"return",keyword_return},{"goto",keyword_goto},
      {"block",keyword_block}};

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

/* init the list */
void 
List_Init(STRList *list) { 
  list->n = 0;
}

/* Like append() in python */
/* Add a value to the string list */
void 
List_Append(STRList* list, int value) {
  list -> n++;
  list->stringtable[list->n] = (squ_string)value;
}

static squ_int
string_escape(char* s, size_t len)
{
  char* t = s;
  char* tend = t + len;
  char* p = s;

  while (t < tend) {
    switch (*t) {
    case '\\':
      t++;
      if (t == tend) break;
      switch (*t) {
      case 'n':
        *p++ = '\n'; break;
      case 'r':
        *p++ = '\r'; break;
      case 't':
        *p++ = '\t'; break;
      case 'e':
        *p++ = 033; break;
      case '0':
        *p++ = '\0'; break;
      case 'x':
        {
          unsigned char c = 0;
          char* xend = t+3;

          t++;
          while (t < tend && t < xend) {
            switch (*t) {
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
              c *= 16;
              c += *t - '0';
              break;
            case 'a': case 'b': case 'c':
            case 'd': case 'e': case 'f':
              c *= 16;
              c += *t - 'a' + 10;
              break;
            default:
              xend = t;
              break;
            }
            t++;
          }
          *p++ = (char)c;
          t--;
        }
        break;
      default:
        *p++ = *t; break;
      }
      t++;
      break;
    default:
      *p++ = *t++;
      break;
    }
  }
  return (squ_int)(p - s);
}


TokenType getToken(YYSTYPE* yylval){
  int result;
  int c;
  int tokenStringIndex = 0;
  StateType state = BEGIN;
  BOOL save;
  int yyleng;
  while(state != FINISH)
  {
    c = getNextChar();
    save = TRUE;
    switch(state)
    {
      case BEGIN:
        if(isdigit(c))
        { 
          state = INNUM;
        }
        else if(isalpha(c) || c == '_')
        {
          state = INID;
        }
        else if(c == ':')
        {
          state = INASSIGN;
        }
        else if((c == ' ') || (c == '\t'))
        {
           save = FALSE;
        }
        else if((c == '\n') || (c == '\r'))
        {
          save = FALSE;
        }
        else if(c == '#')
        { 
          save = FALSE;
          state = INCOMMENT;
        }
        else if(c == '"')
        {
          state = INSTR;
          yyleng = 0;
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
            case '(':
              result = op_lp;
            break;
            case ')':
              result = op_rp;
            break;
            case '{':
              result = op_flp;
            break;
            case '}':
              result = op_frp;
            break;
          }
        }
      break;
      case INCOMMENT:
        save = FALSE;
        if(c == EOF)
        { 
          state = FINISH;
          result = ENDFILE;
        }
        else if(c == '#') 
        {
          state = BEGIN;
        }
      break;
      case INASSIGN:
        state = FINISH;
        if(c == '=')
        {
          result = op_assign;
        }
        else
        {
          ungetNextChar();
          save = FALSE;
          result = ERROR;
        }
      break;
      case INNUM:
        if(!isdigit(c))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = lit_number;
          yylval -> nd = node_int_new(atoi(tokenString));
        }
      break;
      case INID:
        if(!isalpha(c) || (c != '_'))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = identifier;
        }
      break;
      case INSTR:
        if(c == '"')
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = lit_string;
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

static int
yylex(YYSTYPE *yylval)
{
  return getToken(yylval);
}
