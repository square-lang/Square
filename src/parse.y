/*******************************************************************************************************************/
/**                                                                                                                */
/** parse.y - square's lexical and parser                                                                          */
/**                                                                                                                */
/** How to complie:                                                                                                */
/** Linux/Mac:                                                                                                     */
/** $ bison -d -y parse.y                                                                                          */
/**                                                                                                                */
/** Windows:                                                                                                       */
/** $ win_bison -d -y parse.y                                                                                      */
/**                                                                                                                */
/** Author:Stepfen Shawn                                                                                           */
/** Created at Apr 25 00:27:34 2020                                                                                */
/**                                                                                                                */
/** Copyright (c) 2020 Stepfen Shawn                                                                               */
/**                                                                                                                */
/** Permission is hereby granted, free of charge, to any person obtaining a copy                                   */
/** of this software and associated documentation files (the "Software"), to deal                                  */
/** in the Software without restriction, including without limitation the rights                                   */
/** to use, copy, modify, merge, publish, distribute, sublicense, and/or sell                                      */
/** copies of the Software, and to permit persons to whom the Software is                                          */
/** furnished to do so, subject to the following conditions:                                                       */
/**                                                                                                                */
/** The above copyright notice and this permission notice shall be included in all                                 */
/** copies or substantial portions of the Software.                                                                */
/**                                                                                                                */
/** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR                                     */
/** IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,                                       */
/** FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE                                    */
/** AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER                                         */
/** LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,                                  */
/** OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE                                  */
/** SOFTWARE.                                                                                                      */                                            
/*******************************************************************************************************************/

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


/* State type in Lexcial */
typedef enum
{
  BEGIN,       /* start */
  INASSIGN,    /* assign */
  INCOMMENT,   /* comment */
  INNUM,       /* number */
  INID,        /* identifier */
  INSTR,       /* string */
  INEQ,        /* == */
  INNEQ,       /* != */
  INLESS,      /* < or <= */
  INGREATER,   /* > or >= */
  INAND,       /* & or && */
  INOR,        /* | or || */
  INSUB,       /* - or -> */
  FINISH,      /* end */
}StateType;

#define MAXTOKENLEN 80
#define MAXFUNCNAME 50
#define MAXRESERVED 20

/* lexeme of identifier or reserved word */
char tokenString[MAXTOKENLEN+1];

#define BUFLEN 4096
int lineno = 1;

BOOL is_ident;

static char lineBuf[BUFLEN]; /* holds the current line */
static int linepos = 0; /* current position in LineBuf */
static int bufsize = 0; /* current size of buffer string */
static BOOL EOF_flag = FALSE; /* corrects ungetNextChar behavior on EOF */

#define YYLEX_DECL() yylex(YYSTYPE *yylval,parser_state* p)
#define TokenType YYTOKENTYPE

static int yylineno = 1;

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
   = {
      {"if",keyword_if},
      {"else",keyword_else},
      {"null",keyword_null},
      {"import",keyword_import},
      {"false",keyword_false},
      {"true",keyword_true},
      {"break",keyword_break},
      {"return",keyword_return},
      {"goto",keyword_goto},
      {"block",keyword_block},
      {"def",keyword_func},
      {"obj",keyword_obj},
      {"loop",keyword_loop},
      {"when",keyword_when},
      {"is",keyword_is},
      {"not",keyword_not},
      {"lambda",keyword_lambda},
      {"method",keyword_method},
      {"self",keyword_self},
      {"print", keyword_print}
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
        else if(c == '=')
        {
          state = INEQ;
        }
        else if(c == '!')
        {
          state = INNEQ;
        }
        else if(c == '<')
        {
          state = INLESS;
        }
        else if(c == '>')
        {
          state = INGREATER;
        }
        else if(c == '&')
        {
          state = INAND;
        }
        else if(c == '|')
        {
          state = INOR;
        }
        else if(c == '-')
        {
          state = INSUB;
        }
        else
        {
          state = FINISH;
          switch(c)
          {
            case '+':
              result = op_add;
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
            case ',':
              result = op_comma;
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
        }
        if(c == ':')
        {
          result = op_colon;
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
        if(!isalpha(c) && c != '_')
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
      case INEQ:
        if(c == '=')
        {
          result = op_eq;
        }
        else
        {
          result = op_assign;
        }
        state = FINISH;
      break;
      case INSUB:
        {
          if(c == '>')
          {
            state = FINISH;
            result = op_next;
          }
          else
          {
            result = op_sub;
          }
        }
      break;
      case INNEQ:
        {
          state = FINISH;
          result = op_neq;
        }
      break;
      case INLESS:
        {
          if(c == '=')
          {
            result = op_le;
          }
          else
          {
            result = op_lt;
          }
          state = FINISH;
        }
      break;
      case INGREATER:
        {
          if(c == '=')
          {
            result = op_ge;
          }
          else
          {
            result = op_gt;
          }
          state = FINISH;
        }
      break;
      case INAND:
        {
          if(c == '&')
          {
            result = op_and;
          }
          else
          {
            result = op_amper;
          }
          state = FINISH;
        }
      break;
      case INOR:
        {
          if(c == '|')
          {
            result = op_or;
          }
          else
          {
            result = op_bar;
          }
          state = FINISH;
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
      cond
      primary
      primary0
      lambda_stmt

%type <nd> 
      stmts 
      args 
      opt_args 
      opt_block 
      f_args 
      var
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
        keyword_obj
        keyword_loop
        keyword_when
        keyword_is
        keyword_not
        keyword_lambda
        keyword_method
        keyword_self
        keyword_print

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
        op_comma
        op_colon

%token
        ENDFILE
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
%nonassoc  op_eq op_neq keyword_is
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

stmts           :/* none */
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
                ;

stmt            : keyword_return opt_args
                    {
                      $$ = node_return_new($2);
                    }
                | keyword_import identifier
                    {
                      $$ = node_import_new($2);
                    }
                | keyword_print expr
                    {
                      $$ = node_print_new($2);
                    }
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
                | expr keyword_is expr
                    {
                      $$ = node_op_new("==", $1, $3);
                    }
                | expr op_neq expr
                    {
                      $$ = node_op_new("!=", $1, $3);
                    }
                | expr keyword_is keyword_not expr
                    {
                      $$ = node_op_new("!=", $1, $4);
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
                | keyword_not expr
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
                | var op_assign expr
                  {
                    $$ = node_let_new($1, $3);
                  }
                | keyword_func var op_assign expr
                  {
                    $$ = node_let_new($2, $4);
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
                | condition keyword_is condition
                    {
                      $$ = node_op_new("==", $1, $3);
                    }
                | condition op_neq condition
                    {
                      $$ = node_op_new("!=", $1, $3);
                    }
                | condition keyword_is keyword_not condition
                    {
                      $$ = node_op_new("!=", $1, $4);
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
                | opt_elsif keyword_else keyword_if condition op_next op_flp stmt_seq op_frp
                    {
                      $$ = node_if_new($4, $7, NULL);
                    }
                ;

opt_else        : opt_elsif
                    {
                      $$ = NULL;
                    }
                | opt_elsif keyword_else op_next op_flp stmt_seq op_frp
                    {
                      $$ = $5;
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
                | args op_comma expr
                    {
                      $$ = $1;
                      node_array_add($1, $3);
                    }
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
                | keyword_if condition op_next op_flp stmt_seq op_frp opt_else
                    {
                      $$ = node_if_new($2, $5, $7);
                    }
                | keyword_when op_lp condition op_rp op_flp stmt_seq op_frp
                    {
                     $$ = node_if_new($3, $6, NULL);
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
                | identifier op_lp opt_args op_rp
                    {
                      $$ = node_call_new(NULL, node_ident_new($1), $3, NULL);
                    }
                | keyword_loop keyword_when condition op_flp stmt_seq op_frp 
                    {
                      $$ = node_loop_new($5, $3);
                    }
                | keyword_func identifier op_lp opt_args op_rp op_next op_flp stmt_seq op_frp op_mod identifier
                    {
                      $$ = node_fdef_new(node_ident_new($2), $4, $8, node_ident_new($11));
                    }
                | keyword_func identifier op_lp opt_args op_rp op_next op_flp stmt_seq op_frp
                    {
                      $$ = node_fdef_new(node_ident_new($2), $4, $8, NULL);
                    }
                | primary '.' identifier op_lp opt_args op_rp opt_block
                    {
                      $$ = node_call_new($1, node_ident_new($3), $5, $7);
                    }
                | primary '.' identifier opt_block
                    {
                      $$ = node_call_new($1, node_ident_new($3), NULL, $4);
                    }
                | lambda_stmt
                    {
                      $$ = $1;
                    }
                ;

lambda_stmt     : op_lp keyword_lambda opt_args op_next stmt_seq op_rp 
                    {
                      $$ = node_lambda_new($3, $5, NULL, NULL);
                    }
                | op_lp keyword_lambda opt_args op_next stmt_seq op_rp op_le primary0 
                    {
                      $$ = node_lambda_new($3, $5, $8, NULL);
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
                | f_args op_comma identifier
                    {
                      $$ = $1;
                      node_array_add($$, node_ident_new($3));
                    }
                ;

opt_terms       : terms
                ;

terms           : term
                | terms term 
                  {
                    yyerrok;
                  }
                | /* none */
                ;

term            : ' ' 
                | ';'
                | '\n'
                | op_and
                ;
%%