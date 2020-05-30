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
#include <assert.h>

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
#define MAXFUNCNAME 50
#define MAXRESERVED 11

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

#define BUFLEN 4096
int lineno = 1;

BOOL is_ident;
/* function name */
squ_string fname;

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static BOOL EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

#define YYLEX_DECL() yylex(YYSTYPE *yylval,parser_state* p)

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

squ_string copyString(char* s)
{ 
  int n;
  char * t;
  if(s == NULL) 
    return NULL;
  n = strlen(s)+1;
  t = malloc(n);
  if (t == NULL)
    printf("Out of memory error at line %d\n",lineno);
  else 
    strcpy(t,s);
  return t;
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

static int yylineno = 1;

/* Type of an expr */
int expr_t;

typedef struct
{
  squ_int *vm_l;     /* A list to save the opcode */
  squ_int length;    /* Length of the list */
}List;

/* Init the list */
void 
Vm_List_Init(List* list)
{
  list->length = 0;
}

/* Add a opcode to the list */
void 
Vm_List_Append(List* list, squ_int opcode)
{
  list->vm_l[list->length] = opcode;
}

/* New a vm list */
List* vm_list;
/* Init the list of vm  */
/* Vm_List_Init(vm_list); */


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
  {
    linepos--;
  }
}

void
catToken(int c,int tokenStringIndex)
{
  if(tokenStringIndex < MAXTOKENLEN - 1)
  {
    tokenString[++tokenStringIndex] = c;
  }
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
      {"block",keyword_block},{"func",keyword_func},{"class",keyword_class}
    };

/* look for existing keyword*/
/* linear search */
/* TODO: use binary search */
static int 
reservedLookup(squ_string s,YYSTYPE* yylval)
{ 
  int i;
  for (i = 0;i<MAXRESERVED;i++)
    if (!strcmp(s,reservedWords[i].str))
    {
      is_ident = FALSE;
      return reservedWords[i].tok;
    }
  is_ident = TRUE;

  if(is_ident)
  {
    yylval -> nd = node_ident_new(s);
    yylval -> id = node_ident_of(s);
  }

  return identifier;
}

static squ_int
lex_return(squ_int c,parser_state* p)
{
  p->lineno = p->tline;
  p->tline = yylineno;
  return c;
}

#define RETURN(c) return lex_return(c,p) 

TokenType getToken(YYSTYPE* yylval,parser_state* p){
  int result;
  int c;
  int tokenStringIndex = 0;
  StateType state = BEGIN;
  BOOL save;
  int yyleng = 0;
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
        else if(isalpha(c))
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
          yylineno++;
          save = FALSE;
        }
        else if(c == '#')
        { 
          save = FALSE;
          state = INCOMMENT;
        }
        else if((c == '\"') || (c == '\''))
        {
          state = INSTR;
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
        if(!isalpha(c))
        {
          ungetNextChar();
          save = FALSE;
          state = FINISH;
          result = identifier;
        }
      break;
      case INSTR:
        if((c == '"') || (c == '\''))
        {
          state = FINISH;
          save = FALSE;
          squ_string s = strndup_(tokenString,yyleng + 1);
          if(s[0] == '\'' || s[0] == '\"')
          {
            int i;
            for(i = 0; i < yyleng + 1;i++)
            {
              s[i] = s[i + 1];
            }
          }
          yylval -> nd = node_string_new(s);
          result = lit_string;
        }
        else
        {
          catToken(c,tokenStringIndex);
          yyleng++;
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
      tokenString[tokenStringIndex++] = (char)c;
    if(state == FINISH)
    { 
      tokenString[tokenStringIndex] = '\0';
      if(result == identifier)
        result = reservedLookup(tokenString,yylval);
    }
  }
  RETURN(result);
}


static int
yylex(YYSTYPE *yylval,parser_state* p)
{
  return getToken(yylval,p);
}


%}

%language "C"

/* Types/values in association to grammar symbols. */

%union {
  node* nd;
  squ_string id;
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
      primary
      primary0

%type <nd> 
      stmts 
      args 
      opt_args 
      opt_block 
      f_args 
      map 
      var
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
%parse-param {parser_state* p}
%lex-param {p}

%{

static int yylex(YYSTYPE* yylval,parser_state* p);
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
        keyword_func
        keyword_class

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
                  {

                  }
                | keyword_class identifier opt_block
                  {

                  }
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
                | keyword_func identifier op_lp opt_args op_rp opt_block
                    {
                      $$ = node_call_new(NULL,node_ident_new($2), $4, $6);
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

squ_value
set_squ_ptr_value(void *p)
{
  squ_value v;

  v.t = SQU_VALUE_PTR;
  v.v.p = p;
  return v;
}

squ_value
set_squ_bool_value(int b)
{
  squ_value v;

  v.t = SQU_VALUE_BOOL;
  v.v.b = b ? TRUE : FALSE;
  return v;
}

squ_value
set_squ_int_value(squ_int i)
{
  squ_value v;

  v.t = SQU_VALUE_INT;
  v.v.i = i;
  return v;
}

squ_value
set_squ_double_value(squ_double d)
{
  squ_value v;

  v.t = SQU_VALUE_DOUBLE;
  v.v.d = d;
  return v;
}

void*
get_squ_value_ptr(squ_value v)
{
  assert(v.t == SQU_VALUE_PTR);
  return v.v.p;
}

squ_bool
get_squ_value_bool(squ_value v)
{
  assert(v.t == SQU_VALUE_BOOL);
  return v.v.i ? TRUE : FALSE;
}

long
get_squ_value_int(squ_value v)
{
  assert(v.t == SQU_VALUE_INT);
  return v.v.i;
}

double
get_squ_value_double(squ_value v)
{
  assert(v.t == SQU_VALUE_DOUBLE);
  return v.v.d;
}


squ_value* node_expr(squ_ctx*, node*);

squ_value*
node_expr_stmt(squ_ctx* ctx, node* np)
{
  int i;
  node_array* arr = np->value.v.p;
  squ_value* v = NULL;
  for (i = 0; i < arr->len; i++) {
    if (ctx->exc != NULL) {
      return NULL;
    }
    
    v = node_expr(ctx, arr->data[i]);
  }
  return v;
}

void 
squ_fun_def(parser_state* p,squ_string func_name, void* func_p)
{
  int r;
  khiter_t k;

  static squ_value v;
  k = kh_put(value,p->ctx.env,func_name,&r);
  v.t = SQU_VALUE_CFUNC;
  v.v.p = func_p;
  kh_value(p->ctx.env,k) = &v;
}

void
squ_var_def(squ_ctx* ctx,squ_string var_name,squ_value* v)
{
  int ret;
  khiter_t k;
  k = kh_put(value,ctx->env,var_name,&ret);
  if(ret <= 0)
  {
    return;
  }
  kh_value(ctx->env,k) = v;
}

squ_value*
node_expr(squ_ctx* ctx, node* np)
{
  if (ctx->exc != NULL) {
    return NULL;
  }

  if (np == NULL) {
    return NULL;
  }

  switch (np->type) 
  {
  case NODE_IF:
    {
      node_if* nif = np->value.v.p;
      squ_value* v = node_expr(ctx, nif->cond);
      if (ctx->exc != NULL) 
      {
        return NULL;
      }
      if (v->t == SQU_VALUE_NULL || v->v.p == NULL ||
          (v->t == SQU_VALUE_STRING && *v->v.s == 0)) 
          {
            if (nif->opt_else != NULL)
            node_expr_stmt(ctx, nif->opt_else);
          }
      else 
      {
        node_expr_stmt(ctx, nif->stmt_seq);
      }
    }
    break;
  case NODE_OP:
    {
      node_op* nop = np->value.v.p;
      squ_value* lhs = node_expr(ctx, nop->lhs);
      if (ctx->exc != NULL) 
        return NULL;
      if (*nop->op == '+' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) return NULL;
        if (lhs->t == SQU_VALUE_STRING && rhs->t == SQU_VALUE_STRING) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          char *p = malloc(strlen(lhs->v.s) + strlen(rhs->v.s) + 1);
          strcpy(p, lhs->v.s);
          strcat(p, rhs->v.s);
          new->t = SQU_VALUE_STRING;
          new->v.s = p;
          return new;
        } 
        else if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          new->v.d = lhs->v.d + rhs->v.d;
          return new;
        }
        else if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i + rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '-' && *(nop->op+1) == '\0') {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          new->v.d = lhs->v.d - rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i - rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '*' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          new->v.d = lhs->v.d * rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i * rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '/' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          
          new->v.d = lhs->v.d / rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i / rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '%' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;

          new->v.i = lhs->v.i % rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '<') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.d <= rhs->v.d;
          else
            new->v.b = lhs->v.d < rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.i <= rhs->v.i;
          else
            new->v.b = lhs->v.i < rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '>') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.d >= rhs->v.d;
          else
            new->v.b = lhs->v.d > rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.i >= rhs->v.i;
          else
            new->v.b = lhs->v.i > rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '=' && (*(nop->op+1)) == '=') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.d == rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.i == rhs->v.i;
          return new;
        }
      }
      if (*nop->op == '!' && (*(nop->op+1)) == '=') {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.d != rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.i != rhs->v.i;
          return new;
        }
      }
      
      squ_raise(ctx, "invalid operator");
    }
    break;
  case NODE_CALL:
    {
      node_call* ncall = np->value.v.p;
      if (ncall->ident != NULL) 
      {
        khint_t k = kh_get(value, ctx->env, ncall->ident->value.v.id);
        if (k != kh_end(ctx->env)) 
        {
          squ_value* v = kh_value(ctx->env, k);
          if (v->t == SQU_VALUE_CFUNC) 
          {
            node_array* arr0 = ncall->args->value.v.p;
            squ_array* arr1 = squ_array_new();
            int i;
            for (i = 0; i < arr0->len; i++)
              squ_array_add(arr1, node_expr(ctx, arr0->data[i]));
            ((squ_cfunc) v->v.p)(ctx, arr1);
          }
        }
        else 
        {
          squ_raise(ctx, "function not found!");
        }
      } 
      if(ncall->blk != NULL)
      {
        node_block* nblk = ncall->blk->value.v.p;
        node_expr_stmt(ctx, nblk->stmt_seq);
        if (ctx->exc != NULL) {
          squ_value* arg = ctx->exc->arg;
          free(ctx->exc);
          ctx->exc = NULL;
          return arg;
        }
      }
    }
    break;
  case NODE_RETURN:
    {
      node_return* nreturn = np->value.v.p;
      ctx->exc = malloc(sizeof(squ_error));
      ctx->exc->arg = node_expr(ctx, nreturn->rv);
      return NULL;
    }
    break;
  case NODE_LET:
    {
      node_let* nlet = np->value.v.p;
      squ_value* v_r;  /* right */
      squ_value* v_l;  /* left */
      khiter_t k;
      v_r = node_expr(ctx, nlet->rhs);
      v_l = node_expr(ctx, nlet->lhs);
      if(v_l != NULL)
      {
        squ_var_def(ctx,v_l->v.id,v_r);
      }
    }
    break;
  case NODE_VALUE:
    return &np->value;
    break;
  case NODE_IDENT:
    return &np->value;
    break;
  default:
    break;
  }
  return NULL;
}

squ_value*
squ_cputs(squ_ctx* ctx, FILE* out, squ_array* args)
{
  
  int i;
  for (i = 0; i < args->len; i++) 
  {
    squ_value* v;
    if (i != 0)
      fprintf(out, ", ");
    v = args->data[i];
    if (v != NULL) 
    {
      switch (v->t) 
      {
      case SQU_VALUE_DOUBLE:
        fprintf(out, "%f\n", v->v.d);
        break;
      case SQU_VALUE_STRING:
        fprintf(out, "%s\n", v->v.s);
        break;
      case SQU_VALUE_IDENT:
        {
          khint_t k = kh_get(value, ctx->env, v->v.id);
          squ_value* v1 = kh_value(ctx->env, k);
          switch(v1->t)
          {
            case SQU_VALUE_INT:
              fprintf(out,"%d\n",v1->v.i);
              break;
            case SQU_VALUE_DOUBLE:
              fprintf(out,"%f\n",v1->v.d);
              break;
            case SQU_VALUE_NULL:
              fprintf(out,"null\n");
              break;
            case SQU_VALUE_STRING:
              fprintf(out,"%s\n",v1->v.s);
              break;
            case SQU_VALUE_BOOL:
              fprintf(out, v1->v.b ? "true\n" : "false\n");
              break;
          }
          break;
        }
      case SQU_VALUE_NULL:
        fprintf(out, "null\n");
        break;
      case SQU_VALUE_BOOL:
        fprintf(out, v->v.b ? "true\n" : "false\n");
        break;
      case SQU_VALUE_INT:
        fprintf(out,"%d\n",v->v.i);
        break;
      case SQU_VALUE_ERROR:
        fprintf(out, "%s\n", v->v.s);
        break;
      case SQU_VALUE_CFUNC:
        fprintf(out,"<%p>\n",v->v.p);
        break;
      default:
        fprintf(out, "<%p>\n", v->v.p);
        break;
      }
    }
    else
    {
      fprintf(out,"null");
    }
  }
  return NULL;
}

squ_value*
squ_puts(squ_ctx* ctx, squ_array* args) {
  return squ_cputs(ctx, stdout, args);
}

int
squ_run(parser_state* p)
{
  squ_fun_def(p,"cat",squ_puts);
  squ_fun_def(p,"print",squ_puts);
  node_expr_stmt(&p->ctx, (node*)p->lval);
  if (p->ctx.exc != NULL) 
  {
    squ_array* arr = squ_array_new();
    squ_array_add(arr, p->ctx.exc->arg);
    p->ctx.exc = NULL;
  }
  return 0;
}

void
squ_raise(squ_ctx* ctx, const char* msg) {
  ctx->exc = malloc(sizeof(squ_error));
  ctx->exc->arg = malloc(sizeof(squ_value));
  ctx->exc->arg->v.s = strdup(msg);
}