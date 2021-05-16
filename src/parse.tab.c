/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.7"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
/* Line 371 of yacc.c  */
#line 36 "parse.y"


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



/* Line 371 of yacc.c  */
#line 574 "parse.tab.c"

# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "parse.tab.h".  */
#ifndef YY_YY_PARSE_TAB_H_INCLUDED
# define YY_YY_PARSE_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
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
     keyword_func = 268,
     keyword_obj = 269,
     keyword_loop = 270,
     keyword_when = 271,
     keyword_is = 272,
     keyword_not = 273,
     keyword_lambda = 274,
     keyword_method = 275,
     keyword_self = 276,
     keyword_print = 277,
     op_add = 278,
     op_sub = 279,
     op_mul = 280,
     op_div = 281,
     op_mod = 282,
     op_eq = 283,
     op_neq = 284,
     op_lt = 285,
     op_le = 286,
     op_gt = 287,
     op_ge = 288,
     op_and = 289,
     op_or = 290,
     op_bar = 291,
     op_amper = 292,
     op_next = 293,
     op_assign = 294,
     op_lp = 295,
     op_rp = 296,
     op_flp = 297,
     op_frp = 298,
     op_comma = 299,
     op_colon = 300,
     ENDFILE = 301,
     ERROR = 302,
     lit_number = 303,
     lit_string = 304,
     lit_true = 305,
     lit_false = 306,
     lit_null = 307,
     identifier = 308,
     op_LOWEST = 309,
     op_HIGHEST = 310
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 387 of yacc.c  */
#line 546 "parse.y"

  node* nd;
  squ_string id;


/* Line 387 of yacc.c  */
#line 678 "parse.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (parser_state* p);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_PARSE_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */
/* Line 390 of yacc.c  */
#line 589 "parse.y"


static int yylex(YYSTYPE* yylval,parser_state* p);
static void yyerror(parser_state* p, const char* s);
static int yywarp(void);
static void yywarnning(parser_state* p,const char* s);


/* Line 390 of yacc.c  */
#line 714 "parse.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(N) (N)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  68
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   870

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  65
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNRULES -- Number of states.  */
#define YYNSTATES  225

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   310

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      64,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,    62,    55,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    60,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    63,
      61,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    58,     2,    59,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    56,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      57
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,     9,    11,    15,    18,    21,
      24,    26,    28,    30,    34,    38,    42,    46,    50,    54,
      58,    62,    66,    70,    74,    78,    82,    86,    91,    94,
      97,   100,   103,   106,   110,   114,   116,   120,   125,   129,
     133,   137,   141,   145,   149,   153,   157,   161,   165,   169,
     173,   177,   181,   186,   189,   192,   195,   198,   202,   206,
     208,   209,   218,   220,   227,   228,   230,   232,   236,   238,
     240,   242,   246,   250,   253,   261,   269,   271,   273,   275,
     277,   282,   289,   291,   293,   297,   302,   309,   321,   331,
     339,   344,   346,   353,   362,   363,   365,   370,   374,   376,
     379,   381,   385,   387,   389,   392,   393,   395,   397,   399
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      66,     0,    -1,    67,    -1,    68,    85,    -1,    -1,    69,
      -1,    68,    86,    69,    -1,     6,    75,    -1,    10,    53,
      -1,    22,    71,    -1,     5,    -1,    71,    -1,    53,    -1,
      71,    23,    71,    -1,    71,    24,    71,    -1,    71,    25,
      71,    -1,    71,    26,    71,    -1,    71,    27,    71,    -1,
      71,    36,    71,    -1,    71,    37,    71,    -1,    71,    32,
      71,    -1,    71,    33,    71,    -1,    71,    30,    71,    -1,
      71,    31,    71,    -1,    71,    28,    71,    -1,    71,    17,
      71,    -1,    71,    29,    71,    -1,    71,    17,    18,    71,
      -1,    23,    71,    -1,    24,    71,    -1,    55,    71,    -1,
      18,    71,    -1,    56,    71,    -1,    71,    34,    71,    -1,
      71,    35,    71,    -1,    79,    -1,    70,    39,    71,    -1,
      13,    70,    39,    71,    -1,    72,    23,    72,    -1,    72,
      24,    72,    -1,    72,    25,    72,    -1,    72,    26,    72,
      -1,    72,    27,    72,    -1,    72,    36,    72,    -1,    72,
      37,    72,    -1,    72,    32,    72,    -1,    72,    33,    72,
      -1,    72,    30,    72,    -1,    72,    31,    72,    -1,    72,
      28,    72,    -1,    72,    17,    72,    -1,    72,    29,    72,
      -1,    72,    17,    18,    72,    -1,    23,    72,    -1,    24,
      72,    -1,    55,    72,    -1,    56,    72,    -1,    72,    34,
      72,    -1,    72,    35,    72,    -1,    78,    -1,    -1,    73,
       4,     3,    72,    38,    42,    67,    43,    -1,    73,    -1,
      73,     4,    38,    42,    67,    43,    -1,    -1,    76,    -1,
      71,    -1,    76,    44,    71,    -1,    48,    -1,    49,    -1,
      53,    -1,    40,    71,    41,    -1,    58,    76,    59,    -1,
      58,    59,    -1,     3,    72,    38,    42,    67,    43,    74,
      -1,    16,    40,    72,    41,    42,    67,    43,    -1,     7,
      -1,     8,    -1,     9,    -1,    77,    -1,    53,    40,    75,
      41,    -1,    78,    60,    53,    40,    75,    41,    -1,    77,
      -1,    82,    -1,    12,    53,    82,    -1,    53,    40,    75,
      41,    -1,    15,    16,    72,    42,    67,    43,    -1,    13,
      53,    40,    75,    41,    38,    42,    67,    43,    27,    53,
      -1,    13,    53,    40,    75,    41,    38,    42,    67,    43,
      -1,    79,    60,    53,    40,    75,    41,    81,    -1,    79,
      60,    53,    81,    -1,    80,    -1,    40,    19,    75,    38,
      67,    41,    -1,    40,    19,    75,    38,    67,    41,    31,
      77,    -1,    -1,    82,    -1,    42,    83,    67,    43,    -1,
      42,    67,    43,    -1,    61,    -1,    84,    61,    -1,    53,
      -1,    84,    44,    53,    -1,    86,    -1,    87,    -1,    86,
      87,    -1,    -1,    62,    -1,    63,    -1,    64,    -1,    34,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   681,   681,   687,   691,   694,   699,   706,   710,   714,
     718,   722,   728,   734,   738,   742,   746,   750,   754,   758,
     762,   766,   770,   774,   778,   782,   786,   790,   794,   798,
     802,   806,   810,   814,   818,   822,   826,   830,   836,   840,
     844,   848,   852,   856,   860,   864,   868,   872,   876,   880,
     884,   888,   892,   896,   900,   904,   908,   912,   916,   920,
     927,   930,   936,   940,   947,   950,   956,   961,   968,   969,
     970,   974,   978,   982,   986,   990,   994,   998,  1002,  1008,
    1012,  1016,  1022,  1023,  1027,  1031,  1035,  1039,  1043,  1047,
    1051,  1055,  1061,  1065,  1072,  1075,  1081,  1085,  1091,  1095,
    1101,  1106,  1113,  1116,  1117,  1121,  1124,  1125,  1126,  1127
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "keyword_if", "keyword_else",
  "keyword_break", "keyword_return", "keyword_null", "keyword_true",
  "keyword_false", "keyword_import", "keyword_goto", "keyword_block",
  "keyword_func", "keyword_obj", "keyword_loop", "keyword_when",
  "keyword_is", "keyword_not", "keyword_lambda", "keyword_method",
  "keyword_self", "keyword_print", "op_add", "op_sub", "op_mul", "op_div",
  "op_mod", "op_eq", "op_neq", "op_lt", "op_le", "op_gt", "op_ge",
  "op_and", "op_or", "op_bar", "op_amper", "op_next", "op_assign", "op_lp",
  "op_rp", "op_flp", "op_frp", "op_comma", "op_colon", "ENDFILE", "ERROR",
  "lit_number", "lit_string", "lit_true", "lit_false", "lit_null",
  "identifier", "op_LOWEST", "'!'", "'~'", "op_HIGHEST", "'['", "']'",
  "'.'", "'<'", "' '", "';'", "'\\n'", "$accept", "program", "stmt_seq",
  "stmts", "stmt", "var", "expr", "condition", "opt_elsif", "opt_else",
  "opt_args", "args", "primary0", "cond", "primary", "lambda_stmt",
  "opt_block", "block", "bparam", "f_args", "opt_terms", "terms", "term", YY_NULL
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,    33,   126,   310,    91,    93,
      46,    60,    32,    59,    10
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    65,    66,    67,    68,    68,    68,    69,    69,    69,
      69,    69,    70,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      73,    73,    74,    74,    75,    75,    76,    76,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    77,    78,
      78,    78,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    79,    80,    80,    81,    81,    82,    82,    83,    83,
      84,    84,    85,    86,    86,    86,    87,    87,    87,    87
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     1,     3,     2,     2,     2,
       1,     1,     1,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     2,     2,
       2,     2,     2,     3,     3,     1,     3,     4,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     2,     2,     2,     2,     3,     3,     1,
       0,     8,     1,     6,     0,     1,     1,     3,     1,     1,
       1,     3,     3,     2,     7,     7,     1,     1,     1,     1,
       4,     6,     1,     1,     3,     4,     6,    11,     9,     7,
       4,     1,     6,     8,     0,     1,     4,     3,     1,     2,
       1,     3,     1,     1,     2,     0,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,    10,    64,    76,    77,    78,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     4,    68,    69,
      70,     0,     0,     0,     0,     2,   105,     5,     0,    11,
      82,    35,    91,    83,     0,     0,     0,    70,     0,     0,
       0,    79,    59,    66,     7,    65,     8,     0,    12,     0,
       0,     0,    31,     9,    28,    29,    64,     0,    70,    98,
       0,     4,     0,    64,    30,    32,    73,     0,     1,   109,
     106,   107,   108,     3,   102,   103,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    53,    54,    64,    55,    56,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    84,    64,
       0,     0,     0,     0,    71,    97,     0,     0,    99,     0,
      72,     6,   104,    36,     0,    25,    13,    14,    15,    16,
      17,    24,    26,    22,    23,    20,    21,    33,    34,    18,
      19,    94,     0,     0,    50,    38,    39,    40,    41,    42,
      49,    51,    47,    48,    45,    46,    57,    58,    43,    44,
       4,     0,    67,     0,    37,     4,     0,     4,    96,   101,
      85,    27,    64,    90,    95,    80,    52,     0,    64,     0,
       0,     4,     0,     0,    60,     0,     0,    86,     0,    92,
      94,    62,    74,    81,     4,    75,     0,    89,     0,     0,
      70,    93,     0,     0,    88,     0,     4,     0,     0,     0,
      87,     4,    63,     0,    61
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    24,    25,    26,    27,    28,    29,    40,   201,   202,
      44,    45,    30,    42,    31,    32,   183,    33,    61,    62,
      73,    74,    75
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -56
static const yytype_int16 yypact[] =
{
     368,   558,   -56,   445,   -56,   -56,   -56,   -28,   -26,   -21,
      19,     3,   445,   445,   445,   445,   422,   289,   -56,   -56,
       8,   445,   445,   345,    40,   -56,   -10,   -56,     6,   702,
     -56,    -9,   -56,   -56,   558,   558,   445,    15,   558,   558,
     658,   -56,    -3,   702,   -56,    14,   -56,    18,    21,    23,
     558,   558,   702,   702,   -56,   -56,   445,   616,   -33,   -56,
      50,   368,   -35,   445,   -56,   -56,   -56,   -39,   -56,   -56,
     -56,   -56,   -56,   -56,   227,   -56,   445,   499,   445,   445,
     445,   445,   445,   445,   445,   445,   445,   445,   445,   445,
     445,   445,   445,    41,   -56,   -56,   445,   -56,   -56,   522,
     558,   558,   558,   558,   558,   558,   558,   558,   558,   558,
     558,   558,   558,   558,   558,    53,    44,   445,   -56,   445,
     445,   595,   637,    77,   -56,   -56,    73,    65,   -56,    80,
     -56,   -56,   -56,   702,   445,    42,    -8,    -8,   -56,   -56,
     -56,    42,    42,   116,   116,   116,   116,   167,   784,   291,
     744,     2,    81,   558,   820,     4,     4,   -56,   -56,   -56,
     820,   820,   229,   229,   229,   229,   837,   802,   562,   764,
     368,    83,   702,    84,   702,   368,    89,   368,   -56,   -56,
     -56,   702,   445,   -56,   -56,   -56,   723,    92,   445,    88,
      93,   368,    91,    97,   -56,   103,   104,   -56,   102,   117,
      18,   143,   -56,   -56,   368,   -56,   121,   -56,     1,   108,
     -56,   -56,   558,   114,   130,   680,   368,   105,   122,   124,
     -56,   368,   -56,   125,   -56
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -56,   -56,   -15,   -56,    85,   162,     0,   115,   -56,   -56,
     -55,   149,    -1,   -56,   -56,   -56,   -27,   -37,   -56,   -56,
     -56,   -56,   101
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -101
static const yytype_int16 yytable[] =
{
      41,   123,    60,    43,   212,   117,   -12,    63,   129,   127,
     118,  -100,    52,    53,    54,    55,    57,    80,    81,    82,
     130,    64,    65,    43,    69,    46,   128,    47,  -100,   102,
     103,   104,    48,    41,    41,    50,    57,    41,    41,   213,
      68,   152,   182,    51,    17,    76,   126,   -12,    63,    41,
      41,    93,    70,    71,    72,    96,    43,   116,   117,  -101,
      17,   119,   120,    43,   173,    78,    79,    80,    81,    82,
    -101,  -101,    85,    86,    87,    88,   133,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   125,   151,   170,    43,   171,    41,    41,
      41,    41,    41,    41,    41,    41,    41,    41,    41,    41,
      41,    41,    41,    41,   184,   177,   178,   172,   179,    43,
     174,   180,   185,   188,     1,   189,   196,   193,     4,     5,
       6,   191,   199,   195,   181,   194,   197,    11,   200,    78,
      79,    80,    81,    82,   203,   205,   204,   208,   206,    94,
      95,   214,    41,    97,    98,   187,   216,   217,   220,   131,
     190,    36,   192,   184,   221,   121,   122,   222,   224,    18,
      19,    49,    67,   207,   210,   132,   198,     0,     0,    23,
       0,     0,    43,     0,    77,     0,     0,     0,    43,   209,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,   219,     0,     0,     0,   211,   223,     0,     0,     0,
       0,    41,     0,     0,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
       1,     0,     2,     3,     4,     5,     6,     7,     0,     8,
       9,     0,    10,    11,     0,    12,     0,     0,     0,    13,
      14,    15,   100,   101,   102,   103,   104,     0,     0,     0,
       0,    69,     0,     0,     0,     0,     0,    16,   186,    17,
       0,     0,     0,     0,     0,    18,    19,     0,     0,     0,
      20,     0,    21,    22,     0,    23,     0,     0,     0,    70,
      71,    72,     1,     0,     2,     3,     4,     5,     6,     7,
       0,     8,     9,     0,    10,    11,     0,    12,    77,     0,
       0,    13,    14,    15,    78,    79,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,   215,     0,    16,
       0,    17,     0,     0,     0,     0,     0,    18,    19,     0,
       0,     0,    58,     0,    21,    22,     0,    23,     1,     0,
      59,     0,     4,     5,     6,     0,     0,     8,     9,     0,
      10,    11,     0,    12,     0,     0,     0,     0,    14,    15,
       0,     1,     0,     2,     3,     4,     5,     6,     7,     0,
       8,     9,     0,    10,    11,    16,    12,    17,     0,     0,
      13,    14,    15,    18,    19,     0,     0,     0,    20,     0,
      21,    22,     0,    23,    66,     0,     0,     0,    16,     0,
      17,     0,     0,     0,     0,     0,    18,    19,     0,     0,
       0,    20,     0,    21,    22,     1,    23,     0,     0,     4,
       5,     6,     0,     0,     8,     9,     0,    10,    11,     0,
      12,    56,     0,     0,     0,    14,    15,     0,     1,     0,
       0,     0,     4,     5,     6,     0,     0,     8,     9,     0,
      10,    11,    16,    12,    17,     0,     0,     0,    14,    15,
      18,    19,     0,     0,     0,    20,     0,    21,    22,     0,
      23,     0,     0,     0,     0,    16,     0,    17,     0,     0,
       0,     0,     0,    18,    19,     0,     0,     0,    20,     0,
      21,    22,     1,    23,     0,     0,     4,     5,     6,     0,
       0,     8,     9,     0,    10,    11,     0,   134,     0,     0,
       0,     0,    14,    15,     0,     1,     0,     0,     0,     4,
       5,     6,     0,     0,     0,     0,     0,     0,    11,    16,
     153,    17,     0,     0,     0,    34,    35,    18,    19,     0,
       0,     0,    20,     0,    21,    22,     0,    23,     0,     0,
       0,     1,    36,     0,     0,     4,     5,     6,     0,     0,
      18,    19,     0,     0,    11,    37,     0,    38,    39,    99,
      23,    34,    35,     0,     0,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,    36,     0,
       0,     0,     0,     0,     0,     0,    18,    19,     0,     0,
       0,    37,    99,    38,    39,     0,    23,     0,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    77,     0,     0,     0,   175,     0,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,    99,     0,     0,   124,     0,     0,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    99,     0,     0,   176,     0,
       0,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,    99,     0,     0,
       0,     0,     0,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   218,    77,
       0,     0,     0,     0,     0,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      99,     0,     0,     0,     0,     0,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    99,     0,     0,     0,     0,     0,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,    77,     0,     0,     0,     0,     0,    78,    79,    80,
      81,    82,    83,    84,    85,    86,    87,    88,    89,    99,
       0,     0,     0,     0,     0,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,  -101,     0,     0,
       0,     0,     0,   100,   101,   102,   103,   104,  -101,  -101,
     107,   108,   109,   110,    99,     0,     0,     0,     0,     0,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110
};

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-56)))

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-101)))

static const yytype_int16 yycheck[] =
{
       1,    56,    17,     3,     3,    44,    39,    40,    63,    44,
      47,    44,    12,    13,    14,    15,    16,    25,    26,    27,
      59,    21,    22,    23,    34,    53,    61,    53,    61,    25,
      26,    27,    53,    34,    35,    16,    36,    38,    39,    38,
       0,    96,    40,    40,    42,    39,    61,    39,    40,    50,
      51,    60,    62,    63,    64,    40,    56,    60,    44,    17,
      42,    40,    39,    63,   119,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    76,    77,    78,    79,
      80,    81,    82,    83,    84,    85,    86,    87,    88,    89,
      90,    91,    92,    43,    53,    42,    96,    53,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   151,    38,    43,   117,    53,   119,
     120,    41,    41,    40,     3,    41,    38,   182,     7,     8,
       9,    42,    41,   188,   134,    43,    43,    16,    41,    23,
      24,    25,    26,    27,    41,    43,    42,     4,    31,    34,
      35,    43,   153,    38,    39,   170,    42,    27,    53,    74,
     175,    40,   177,   200,    42,    50,    51,    43,    43,    48,
      49,     9,    23,   200,    53,    74,   191,    -1,    -1,    58,
      -1,    -1,   182,    -1,    17,    -1,    -1,    -1,   188,   204,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,   216,    -1,    -1,    -1,   206,   221,    -1,    -1,    -1,
      -1,   212,    -1,    -1,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
       3,    -1,     5,     6,     7,     8,     9,    10,    -1,    12,
      13,    -1,    15,    16,    -1,    18,    -1,    -1,    -1,    22,
      23,    24,    23,    24,    25,    26,    27,    -1,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    -1,    -1,    40,   153,    42,
      -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,    -1,
      53,    -1,    55,    56,    -1,    58,    -1,    -1,    -1,    62,
      63,    64,     3,    -1,     5,     6,     7,     8,     9,    10,
      -1,    12,    13,    -1,    15,    16,    -1,    18,    17,    -1,
      -1,    22,    23,    24,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,   212,    -1,    40,
      -1,    42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,
      -1,    -1,    53,    -1,    55,    56,    -1,    58,     3,    -1,
      61,    -1,     7,     8,     9,    -1,    -1,    12,    13,    -1,
      15,    16,    -1,    18,    -1,    -1,    -1,    -1,    23,    24,
      -1,     3,    -1,     5,     6,     7,     8,     9,    10,    -1,
      12,    13,    -1,    15,    16,    40,    18,    42,    -1,    -1,
      22,    23,    24,    48,    49,    -1,    -1,    -1,    53,    -1,
      55,    56,    -1,    58,    59,    -1,    -1,    -1,    40,    -1,
      42,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,
      -1,    53,    -1,    55,    56,     3,    58,    -1,    -1,     7,
       8,     9,    -1,    -1,    12,    13,    -1,    15,    16,    -1,
      18,    19,    -1,    -1,    -1,    23,    24,    -1,     3,    -1,
      -1,    -1,     7,     8,     9,    -1,    -1,    12,    13,    -1,
      15,    16,    40,    18,    42,    -1,    -1,    -1,    23,    24,
      48,    49,    -1,    -1,    -1,    53,    -1,    55,    56,    -1,
      58,    -1,    -1,    -1,    -1,    40,    -1,    42,    -1,    -1,
      -1,    -1,    -1,    48,    49,    -1,    -1,    -1,    53,    -1,
      55,    56,     3,    58,    -1,    -1,     7,     8,     9,    -1,
      -1,    12,    13,    -1,    15,    16,    -1,    18,    -1,    -1,
      -1,    -1,    23,    24,    -1,     3,    -1,    -1,    -1,     7,
       8,     9,    -1,    -1,    -1,    -1,    -1,    -1,    16,    40,
      18,    42,    -1,    -1,    -1,    23,    24,    48,    49,    -1,
      -1,    -1,    53,    -1,    55,    56,    -1,    58,    -1,    -1,
      -1,     3,    40,    -1,    -1,     7,     8,     9,    -1,    -1,
      48,    49,    -1,    -1,    16,    53,    -1,    55,    56,    17,
      58,    23,    24,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    40,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    48,    49,    -1,    -1,
      -1,    53,    17,    55,    56,    -1,    58,    -1,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    17,    -1,    -1,    -1,    42,    -1,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    17,    -1,    -1,    41,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    17,    -1,    -1,    41,    -1,
      -1,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,    17,    -1,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    17,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    17,    -1,    -1,    -1,    -1,    -1,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    17,
      -1,    -1,    -1,    -1,    -1,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    17,    -1,    -1,
      -1,    -1,    -1,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    17,    -1,    -1,    -1,    -1,    -1,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     6,     7,     8,     9,    10,    12,    13,
      15,    16,    18,    22,    23,    24,    40,    42,    48,    49,
      53,    55,    56,    58,    66,    67,    68,    69,    70,    71,
      77,    79,    80,    82,    23,    24,    40,    53,    55,    56,
      72,    77,    78,    71,    75,    76,    53,    53,    53,    70,
      16,    40,    71,    71,    71,    71,    19,    71,    53,    61,
      67,    83,    84,    40,    71,    71,    59,    76,     0,    34,
      62,    63,    64,    85,    86,    87,    39,    17,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    60,    72,    72,    40,    72,    72,    17,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    60,    44,    82,    40,
      39,    72,    72,    75,    41,    43,    67,    44,    61,    75,
      59,    69,    87,    71,    18,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    53,    75,    18,    72,    72,    72,    72,    72,    72,
      72,    72,    72,    72,    72,    72,    72,    72,    72,    72,
      42,    53,    71,    75,    71,    42,    41,    38,    43,    53,
      41,    71,    40,    81,    82,    41,    72,    67,    40,    41,
      67,    42,    67,    75,    43,    75,    38,    43,    67,    41,
      41,    73,    74,    41,    42,    43,    31,    81,     4,    67,
      53,    77,     3,    38,    43,    72,    42,    27,    38,    67,
      53,    42,    43,    67,    43
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (p, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))

/* Error token number */
#define YYTERROR	1
#define YYERRCODE	256


/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */
#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval, p)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, p); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_state* p)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, p)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_state* p;
#endif
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
  YYUSE (p);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_state* p)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, p)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    parser_state* p;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, p);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, parser_state* p)
#else
static void
yy_reduce_print (yyvsp, yyrule, p)
    YYSTYPE *yyvsp;
    int yyrule;
    parser_state* p;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , p);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, p); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULL, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULL;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULL, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, parser_state* p)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, p)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    parser_state* p;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (p);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}




/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (parser_state* p)
#else
int
yyparse (p)
    parser_state* p;
#endif
#endif
{
/* The lookahead symbol.  */
int yychar;


#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
static YYSTYPE yyval_default;
# define YY_INITIAL_VALUE(Value) = Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval YY_INITIAL_VALUE(yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
/* Line 1792 of yacc.c  */
#line 682 "parse.y"
    { 
                      p->lval = (yyvsp[(1) - (1)].nd);  
                    }
    break;

  case 4:
/* Line 1792 of yacc.c  */
#line 691 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                    }
    break;

  case 5:
/* Line 1792 of yacc.c  */
#line 695 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                      node_array_add((yyval.nd), (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 6:
/* Line 1792 of yacc.c  */
#line 700 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 7:
/* Line 1792 of yacc.c  */
#line 707 "parse.y"
    {
                      (yyval.nd) = node_return_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 8:
/* Line 1792 of yacc.c  */
#line 711 "parse.y"
    {
                      (yyval.nd) = node_import_new((yyvsp[(2) - (2)].id));
                    }
    break;

  case 9:
/* Line 1792 of yacc.c  */
#line 715 "parse.y"
    {
                      (yyval.nd) = node_print_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 10:
/* Line 1792 of yacc.c  */
#line 719 "parse.y"
    {
                      (yyval.nd) = node_break_new();
                    }
    break;

  case 11:
/* Line 1792 of yacc.c  */
#line 723 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 12:
/* Line 1792 of yacc.c  */
#line 729 "parse.y"
    {
                        (yyval.nd) = node_ident_new((yyvsp[(1) - (1)].id));
                    }
    break;

  case 13:
/* Line 1792 of yacc.c  */
#line 735 "parse.y"
    {
                      (yyval.nd) = node_op_new("+", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 14:
/* Line 1792 of yacc.c  */
#line 739 "parse.y"
    {
                      (yyval.nd) = node_op_new("-", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 15:
/* Line 1792 of yacc.c  */
#line 743 "parse.y"
    {
                      (yyval.nd) = node_op_new("*", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 16:
/* Line 1792 of yacc.c  */
#line 747 "parse.y"
    {
                      (yyval.nd) = node_op_new("/", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 17:
/* Line 1792 of yacc.c  */
#line 751 "parse.y"
    {
                      (yyval.nd) = node_op_new("%", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 18:
/* Line 1792 of yacc.c  */
#line 755 "parse.y"
    {
                      (yyval.nd) = node_op_new("|", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 19:
/* Line 1792 of yacc.c  */
#line 759 "parse.y"
    {
                      (yyval.nd) = node_op_new("&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 20:
/* Line 1792 of yacc.c  */
#line 763 "parse.y"
    {
                      (yyval.nd) = node_op_new(">", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 21:
/* Line 1792 of yacc.c  */
#line 767 "parse.y"
    {
                      (yyval.nd) = node_op_new(">=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 22:
/* Line 1792 of yacc.c  */
#line 771 "parse.y"
    {
                      (yyval.nd) = node_op_new("<", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 23:
/* Line 1792 of yacc.c  */
#line 775 "parse.y"
    {
                      (yyval.nd) = node_op_new("<=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 24:
/* Line 1792 of yacc.c  */
#line 779 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 25:
/* Line 1792 of yacc.c  */
#line 783 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 26:
/* Line 1792 of yacc.c  */
#line 787 "parse.y"
    {
                      (yyval.nd) = node_op_new("!=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 27:
/* Line 1792 of yacc.c  */
#line 791 "parse.y"
    {
                      (yyval.nd) = node_op_new("!=", (yyvsp[(1) - (4)].nd), (yyvsp[(4) - (4)].nd));
                    }
    break;

  case 28:
/* Line 1792 of yacc.c  */
#line 795 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 29:
/* Line 1792 of yacc.c  */
#line 799 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 30:
/* Line 1792 of yacc.c  */
#line 803 "parse.y"
    {
                      (yyval.nd) = node_op_new("!", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 31:
/* Line 1792 of yacc.c  */
#line 807 "parse.y"
    {
                      (yyval.nd) = node_op_new("!", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 32:
/* Line 1792 of yacc.c  */
#line 811 "parse.y"
    {
                      (yyval.nd) = node_op_new("~", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 33:
/* Line 1792 of yacc.c  */
#line 815 "parse.y"
    {
                      (yyval.nd) = node_op_new("&&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 34:
/* Line 1792 of yacc.c  */
#line 819 "parse.y"
    {
                      (yyval.nd) = node_op_new("||", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 35:
/* Line 1792 of yacc.c  */
#line 823 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 36:
/* Line 1792 of yacc.c  */
#line 827 "parse.y"
    {
                    (yyval.nd) = node_let_new((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                  }
    break;

  case 37:
/* Line 1792 of yacc.c  */
#line 831 "parse.y"
    {
                    (yyval.nd) = node_let_new((yyvsp[(2) - (4)].nd), (yyvsp[(4) - (4)].nd));
                  }
    break;

  case 38:
/* Line 1792 of yacc.c  */
#line 837 "parse.y"
    {
                      (yyval.nd) = node_op_new("+", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 39:
/* Line 1792 of yacc.c  */
#line 841 "parse.y"
    {
                      (yyval.nd) = node_op_new("-", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 40:
/* Line 1792 of yacc.c  */
#line 845 "parse.y"
    {
                      (yyval.nd) = node_op_new("*", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 41:
/* Line 1792 of yacc.c  */
#line 849 "parse.y"
    {
                      (yyval.nd) = node_op_new("/", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 42:
/* Line 1792 of yacc.c  */
#line 853 "parse.y"
    {
                      (yyval.nd) = node_op_new("%", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 43:
/* Line 1792 of yacc.c  */
#line 857 "parse.y"
    {
                      (yyval.nd) = node_op_new("|", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 44:
/* Line 1792 of yacc.c  */
#line 861 "parse.y"
    {
                      (yyval.nd) = node_op_new("&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 45:
/* Line 1792 of yacc.c  */
#line 865 "parse.y"
    {
                      (yyval.nd) = node_op_new(">", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 46:
/* Line 1792 of yacc.c  */
#line 869 "parse.y"
    {
                      (yyval.nd) = node_op_new(">=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 47:
/* Line 1792 of yacc.c  */
#line 873 "parse.y"
    {
                      (yyval.nd) = node_op_new("<", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 48:
/* Line 1792 of yacc.c  */
#line 877 "parse.y"
    {
                      (yyval.nd) = node_op_new("<=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 49:
/* Line 1792 of yacc.c  */
#line 881 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 50:
/* Line 1792 of yacc.c  */
#line 885 "parse.y"
    {
                      (yyval.nd) = node_op_new("==", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 51:
/* Line 1792 of yacc.c  */
#line 889 "parse.y"
    {
                      (yyval.nd) = node_op_new("!=", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 52:
/* Line 1792 of yacc.c  */
#line 893 "parse.y"
    {
                      (yyval.nd) = node_op_new("!=", (yyvsp[(1) - (4)].nd), (yyvsp[(4) - (4)].nd));
                    }
    break;

  case 53:
/* Line 1792 of yacc.c  */
#line 897 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 54:
/* Line 1792 of yacc.c  */
#line 901 "parse.y"
    {
                      (yyval.nd) = node_value_new((yyvsp[(2) - (2)].nd));
                    }
    break;

  case 55:
/* Line 1792 of yacc.c  */
#line 905 "parse.y"
    {
                      (yyval.nd) = node_op_new("!", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 56:
/* Line 1792 of yacc.c  */
#line 909 "parse.y"
    {
                      (yyval.nd) = node_op_new("~", NULL, (yyvsp[(2) - (2)].nd));
                    }
    break;

  case 57:
/* Line 1792 of yacc.c  */
#line 913 "parse.y"
    {
                      (yyval.nd) = node_op_new("&&", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 58:
/* Line 1792 of yacc.c  */
#line 917 "parse.y"
    {
                      (yyval.nd) = node_op_new("||", (yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 59:
/* Line 1792 of yacc.c  */
#line 921 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 60:
/* Line 1792 of yacc.c  */
#line 927 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 61:
/* Line 1792 of yacc.c  */
#line 931 "parse.y"
    {
                      (yyval.nd) = node_if_new((yyvsp[(4) - (8)].nd), (yyvsp[(7) - (8)].nd), NULL);
                    }
    break;

  case 62:
/* Line 1792 of yacc.c  */
#line 937 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 63:
/* Line 1792 of yacc.c  */
#line 941 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(5) - (6)].nd);
                    }
    break;

  case 64:
/* Line 1792 of yacc.c  */
#line 947 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                    }
    break;

  case 65:
/* Line 1792 of yacc.c  */
#line 951 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 66:
/* Line 1792 of yacc.c  */
#line 957 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                      node_array_add((yyval.nd), (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 67:
/* Line 1792 of yacc.c  */
#line 962 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyvsp[(1) - (3)].nd), (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 70:
/* Line 1792 of yacc.c  */
#line 971 "parse.y"
    {
                    (yyval.nd) = node_ident_new((yyvsp[(1) - (1)].id));
                  }
    break;

  case 71:
/* Line 1792 of yacc.c  */
#line 975 "parse.y"
    {
                       (yyval.nd) = (yyvsp[(2) - (3)].nd);
                    }
    break;

  case 72:
/* Line 1792 of yacc.c  */
#line 979 "parse.y"
    {
                      (yyval.nd) = node_array_of((yyvsp[(2) - (3)].nd));
                    }
    break;

  case 73:
/* Line 1792 of yacc.c  */
#line 983 "parse.y"
    {
                      (yyval.nd) = node_array_of(NULL);
                    }
    break;

  case 74:
/* Line 1792 of yacc.c  */
#line 987 "parse.y"
    {
                      (yyval.nd) = node_if_new((yyvsp[(2) - (7)].nd), (yyvsp[(5) - (7)].nd), (yyvsp[(7) - (7)].nd));
                    }
    break;

  case 75:
/* Line 1792 of yacc.c  */
#line 991 "parse.y"
    {
                     (yyval.nd) = node_if_new((yyvsp[(3) - (7)].nd), (yyvsp[(6) - (7)].nd), NULL);
                    }
    break;

  case 76:
/* Line 1792 of yacc.c  */
#line 995 "parse.y"
    {
                      (yyval.nd) = node_null();
                    }
    break;

  case 77:
/* Line 1792 of yacc.c  */
#line 999 "parse.y"
    {
                      (yyval.nd) = node_true();
                    }
    break;

  case 78:
/* Line 1792 of yacc.c  */
#line 1003 "parse.y"
    {
                      (yyval.nd) = node_false();
                    }
    break;

  case 79:
/* Line 1792 of yacc.c  */
#line 1009 "parse.y"
    {
                       (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 80:
/* Line 1792 of yacc.c  */
#line 1013 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(1) - (4)].id)), (yyvsp[(3) - (4)].nd), NULL);
                    }
    break;

  case 81:
/* Line 1792 of yacc.c  */
#line 1017 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(3) - (6)].id)), (yyvsp[(5) - (6)].nd), NULL);
                    }
    break;

  case 83:
/* Line 1792 of yacc.c  */
#line 1024 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, NULL, NULL, (yyvsp[(1) - (1)].nd));
                    }
    break;

  case 84:
/* Line 1792 of yacc.c  */
#line 1028 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(2) - (3)].id)), NULL, (yyvsp[(3) - (3)].nd));
                    }
    break;

  case 85:
/* Line 1792 of yacc.c  */
#line 1032 "parse.y"
    {
                      (yyval.nd) = node_call_new(NULL, node_ident_new((yyvsp[(1) - (4)].id)), (yyvsp[(3) - (4)].nd), NULL);
                    }
    break;

  case 86:
/* Line 1792 of yacc.c  */
#line 1036 "parse.y"
    {
                      (yyval.nd) = node_loop_new((yyvsp[(5) - (6)].nd), (yyvsp[(3) - (6)].nd));
                    }
    break;

  case 87:
/* Line 1792 of yacc.c  */
#line 1040 "parse.y"
    {
                      (yyval.nd) = node_fdef_new(node_ident_new((yyvsp[(2) - (11)].id)), (yyvsp[(4) - (11)].nd), (yyvsp[(8) - (11)].nd), node_ident_new((yyvsp[(11) - (11)].id)));
                    }
    break;

  case 88:
/* Line 1792 of yacc.c  */
#line 1044 "parse.y"
    {
                      (yyval.nd) = node_fdef_new(node_ident_new((yyvsp[(2) - (9)].id)), (yyvsp[(4) - (9)].nd), (yyvsp[(8) - (9)].nd), NULL);
                    }
    break;

  case 89:
/* Line 1792 of yacc.c  */
#line 1048 "parse.y"
    {
                      (yyval.nd) = node_call_new((yyvsp[(1) - (7)].nd), node_ident_new((yyvsp[(3) - (7)].id)), (yyvsp[(5) - (7)].nd), (yyvsp[(7) - (7)].nd));
                    }
    break;

  case 90:
/* Line 1792 of yacc.c  */
#line 1052 "parse.y"
    {
                      (yyval.nd) = node_call_new((yyvsp[(1) - (4)].nd), node_ident_new((yyvsp[(3) - (4)].id)), NULL, (yyvsp[(4) - (4)].nd));
                    }
    break;

  case 91:
/* Line 1792 of yacc.c  */
#line 1056 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 92:
/* Line 1792 of yacc.c  */
#line 1062 "parse.y"
    {
                      (yyval.nd) = node_lambda_new((yyvsp[(3) - (6)].nd), (yyvsp[(5) - (6)].nd), NULL, NULL);
                    }
    break;

  case 93:
/* Line 1792 of yacc.c  */
#line 1066 "parse.y"
    {
                      (yyval.nd) = node_lambda_new((yyvsp[(3) - (8)].nd), (yyvsp[(5) - (8)].nd), (yyvsp[(8) - (8)].nd), NULL);
                    }
    break;

  case 94:
/* Line 1792 of yacc.c  */
#line 1072 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 95:
/* Line 1792 of yacc.c  */
#line 1076 "parse.y"
    {
                       (yyval.nd) = (yyvsp[(1) - (1)].nd);
                    }
    break;

  case 96:
/* Line 1792 of yacc.c  */
#line 1082 "parse.y"
    {
                      (yyval.nd) = node_block_new((yyvsp[(2) - (4)].nd), (yyvsp[(3) - (4)].nd));
                    }
    break;

  case 97:
/* Line 1792 of yacc.c  */
#line 1086 "parse.y"
    {
                      (yyval.nd) = node_block_new(NULL, (yyvsp[(2) - (3)].nd));
                    }
    break;

  case 98:
/* Line 1792 of yacc.c  */
#line 1092 "parse.y"
    {
                      (yyval.nd) = NULL;
                    }
    break;

  case 99:
/* Line 1792 of yacc.c  */
#line 1096 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (2)].nd);
                    }
    break;

  case 100:
/* Line 1792 of yacc.c  */
#line 1102 "parse.y"
    {
                      (yyval.nd) = node_array_new();
                      node_array_add((yyval.nd), node_ident_new((yyvsp[(1) - (1)].id)));
                    }
    break;

  case 101:
/* Line 1792 of yacc.c  */
#line 1107 "parse.y"
    {
                      (yyval.nd) = (yyvsp[(1) - (3)].nd);
                      node_array_add((yyval.nd), node_ident_new((yyvsp[(3) - (3)].id)));
                    }
    break;

  case 104:
/* Line 1792 of yacc.c  */
#line 1118 "parse.y"
    {
                    yyerrok;
                  }
    break;


/* Line 1792 of yacc.c  */
#line 3039 "parse.tab.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (p, YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (p, yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, p);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, p);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (p, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, p);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, p);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


/* Line 2055 of yacc.c  */
#line 1129 "parse.y"
